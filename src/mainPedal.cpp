// mainPedal.cpp
#include "stm32f4xx_hal_rcc.h"
extern "C" {
    #include "stm32f4xx_hal.h"
	#include "stm32f4xx_hal_gpio.h"
	#include "stm32f4xx_hal_i2s.h"
	#include "stm32f4xx_hal_dma.h"
	#include "Legacy/stm32_hal_legacy.h"
	#include "stm32f411xe.h"
	#include "stm32f4xx_hal_cortex.h"
	#include "stm32f4xx_hal_def.h"
#include "stm32f4xx_hal_i2s_ex.h"
}

#include <iostream>
#include "AudioFrame.h"
#include "Effects.h"

// Pin Definitions:
// I2S2 (master, TX to the DAC)
// I2S2ext (slave, RX from the ADC)

#define I2S2_CK			GPIO_PIN_13
#define I2S2_WS			GPIO_PIN_12
#define I2S_DATA_RX		GPIO_PIN_14 // Data comes in from the ADC
#define I2S_DATA_TX		GPIO_PIN_15 // Data goes out to the DAC
#define I2S_GPIO_PORT	GPIOB	

#define AUDIO_BLOCK_SIZE	64
#define AUDIO_BUFFER_LEN	(AUDIO_BLOCK_SIZE * 2) // double buffered for ping pong

// Handling all firmware GPIO's, I2S, DMA stuff:
I2S_HandleTypeDef hi2s2;
DMA_HandleTypeDef hdma_i2s2_rx;
DMA_HandleTypeDef hdma_i2s2ext_tx;

static void Error_Handler(void)
{
	while(1) {};
}

static void I2S_GPIO_Init(void)
{
	__HAL_RCC_GPIOB_CLK_ENABLE();

	// For WS/CK/TX using SPI2 alt function
	GPIO_InitTypeDef gpio = {0};
	gpio.Pin		= I2S2_CK | I2S2_WS | I2S_DATA_TX;
	gpio.Mode		= GPIO_MODE_AF_PP;
	gpio.Pull		= GPIO_NOPULL;
	gpio.Speed		= GPIO_SPEED_FREQ_HIGH;
	gpio.Alternate	= GPIO_AF5_SPI2;
	HAL_GPIO_Init(I2S_GPIO_PORT, &gpio);

	// For RX using I2Sext 
	gpio.Pin		= I2S_DATA_RX;
	gpio.Mode		= GPIO_MODE_AF_PP;
	gpio.Pull		= GPIO_NOPULL;
	gpio.Speed		= GPIO_SPEED_FREQ_HIGH;
	gpio.Alternate	= GPIO_AF6_I2S2ext;
	HAL_GPIO_Init(I2S_GPIO_PORT, &gpio);
}

static void I2S_DMA_Init(void)
{
	__HAL_RCC_DMA1_CLK_ENABLE();

	// RX - from the ADC, DMA1 Stream 3, Channel 0
	hdma_i2s2_rx.Instance					= DMA1_Stream3;
	hdma_i2s2_rx.Init.Channel				= DMA_CHANNEL_0;
	hdma_i2s2_rx.Init.Direction				= DMA_PERIPH_TO_MEMORY;
	hdma_i2s2_rx.Init.MemInc				= DMA_MINC_ENABLE;
	hdma_i2s2_rx.Init.PeriphInc				= DMA_PINC_DISABLE;
	hdma_i2s2_rx.Init.PeriphDataAlignment	= DMA_PDATAALIGN_HALFWORD;
	hdma_i2s2_rx.Init.MemDataAlignment		= DMA_MDATAALIGN_HALFWORD;
	hdma_i2s2_rx.Init.Mode					= DMA_CIRCULAR;
	hdma_i2s2_rx.Init.Priority				= DMA_PRIORITY_HIGH;
	hdma_i2s2_rx.Init.FIFOMode				= DMA_FIFOMODE_DISABLE;

	if(HAL_DMA_Init(&hdma_i2s2_rx) != HAL_OK) Error_Handler();
	__HAL_LINKDMA(&hi2s2, hdmarx, hdma_i2s2_rx);


	// TX - to the DAC, DMA1 Stream 4, Channel 2
	hdma_i2s2ext_tx.Instance					= DMA1_Stream4;
	hdma_i2s2ext_tx.Init.Channel				= DMA_CHANNEL_2;
	hdma_i2s2ext_tx.Init.Direction				= DMA_MEMORY_TO_PERIPH;
	hdma_i2s2ext_tx.Init.MemInc					= DMA_MINC_ENABLE;
	hdma_i2s2ext_tx.Init.PeriphInc				= DMA_PINC_DISABLE;
	hdma_i2s2ext_tx.Init.PeriphDataAlignment	= DMA_PDATAALIGN_HALFWORD;
	hdma_i2s2ext_tx.Init.MemDataAlignment		= DMA_MDATAALIGN_HALFWORD;
	hdma_i2s2ext_tx.Init.Mode					= DMA_CIRCULAR;
	hdma_i2s2ext_tx.Init.Priority				= DMA_PRIORITY_HIGH;
	hdma_i2s2ext_tx.Init.FIFOMode				= DMA_FIFOMODE_DISABLE;

	if(HAL_DMA_Init(&hdma_i2s2ext_tx) != HAL_OK) Error_Handler();
	__HAL_LINKDMA(&hi2s2, hdmatx, hdma_i2s2ext_tx);

	// DMA Interrupts
	HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
	HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
}

static void I2S_Init(void)
{
	__HAL_RCC_SPI2_CLK_ENABLE();
	hi2s2.Instance				= SPI2;
	hi2s2.Init.Mode				= I2S_MODE_MASTER_TX;
	hi2s2.Init.Standard			= I2S_STANDARD_PHILIPS;
	hi2s2.Init.DataFormat		= I2S_DATAFORMAT_16B;
	hi2s2.Init.MCLKOutput		= I2S_MCLKOUTPUT_DISABLE;
	hi2s2.Init.AudioFreq		= I2S_AUDIOFREQ_48K;
	hi2s2.Init.CPOL				= I2S_CPOL_LOW;
	hi2s2.Init.ClockSource		= I2S_CLOCK_PLL;
	hi2s2.Init.FullDuplexMode	= I2S_FULLDUPLEXMODE_ENABLE;

	if(HAL_I2S_Init(&hi2s2) != HAL_OK) Error_Handler();
}

// For driving the SCK
static void Setup_MC01_Clock(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin			= GPIO_PIN_8;
	GPIO_InitStruct.Mode		= GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull		= GPIO_NOPULL;
	GPIO_InitStruct.Speed		= GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate	= GPIO_AF0_MCO;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_4);
}

// ISR Handling:
static int16_t rxBuffer[AUDIO_BUFFER_LEN];
static int16_t txBuffer[AUDIO_BUFFER_LEN];

static void ProcessBlock(int16_t* rxHalf, int16_t* txHalf, size_t blockSize)
{
	AudioFrame inFrame(48000, blockSize);
	auto& inData = inFrame.getDataMutable();
	for(size_t i = 0; i < blockSize; i++)
		inData[i] = rxHalf[i] / 32768.0;

	AudioFrame outFrame(48000, blockSize);
	Effects::Bypass(inFrame, outFrame);

	const auto& outData = outFrame.getData();
	for(size_t i = 0; i < blockSize; i++)
		txHalf[i] = static_cast<int16_t>(outData[i] * 32768.0);
}

// ISR when first half of rxBuffer is ready
// Process rxBuffer[0, ..., AUDIO_BLOCK_SIZE]
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef* hi2s)
{
	ProcessBlock(&rxBuffer[0], &txBuffer[0], AUDIO_BLOCK_SIZE);
}

// ISR when second half of rxBuffer is ready
// Process rxBuffer[AUDIO_BLOCK_SIZE, ..., AUDIO_BUFFER_LEN]
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef* hi2s)
{
	ProcessBlock(&rxBuffer[AUDIO_BLOCK_SIZE], &txBuffer[AUDIO_BLOCK_SIZE], AUDIO_BLOCK_SIZE);
}

extern "C" void DMA1_Stream3_IRQHandler(void) { HAL_DMA_IRQHandler(&hdma_i2s2_rx);}
extern "C" void DMA1_Stream4_IRQHandler(void) { HAL_DMA_IRQHandler(&hdma_i2s2ext_tx);}

int main(void)
{
    HAL_Init();
	I2S_GPIO_Init();
	I2S_DMA_Init();
	I2S_Init();
	Setup_MC01_Clock();

	if(HAL_I2SEx_TransmitReceive(&hi2s2, (uint16_t*)txBuffer, (uint16_t*)rxBuffer, AUDIO_BUFFER_LEN, 10000) != HAL_OK)
		Error_Handler();
    while (1)
    {
    }
}
