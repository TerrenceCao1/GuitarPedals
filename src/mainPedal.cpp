// mainPedal.cpp
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
	#include "stm32f4xx_hal_rcc.h"
	#include "main.h"
}

#include "AudioFrame.h"
#include "Effects.h"

/**I2S3 GPIO Configuration
PB3      ------> I2S3_CK
PA15     ------> I2S3_WS
PB10     ------> I2S3_MCK
PB4      ------> I2S3_ext_SD (Data In, from ADC)
PB5      ------> I2S3_SD     (Data Out, to DAC)
*/

/* Private variables ---------------------------------------------------------*/
I2S_HandleTypeDef hi2s3;
DMA_HandleTypeDef hdma_i2s3_ext_rx;
DMA_HandleTypeDef hdma_spi3_tx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2S3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define AUDIO_BLOCK_SIZE	64
#define AUDIO_BUFFER_LEN	(AUDIO_BLOCK_SIZE * 2) // double buffered for ping pong

static int16_t rxBuffer[AUDIO_BUFFER_LEN];
static int16_t txBuffer[AUDIO_BUFFER_LEN];

static void ProcessBlock(int16_t* rxHalf, int16_t* txHalf, size_t blockSize = AUDIO_BLOCK_SIZE)
{
    // Simple bypass - just copy input to output
    for(size_t i = 0; i < blockSize; i++)
        txHalf[i] = rxHalf[i];
}

void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef* hi2s)
{
	ProcessBlock(&rxBuffer[0], &txBuffer[0], AUDIO_BLOCK_SIZE);
}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef* hi2s)
{
	ProcessBlock(&rxBuffer[AUDIO_BLOCK_SIZE], &txBuffer[AUDIO_BLOCK_SIZE], AUDIO_BLOCK_SIZE);
}

static void BlinkN(int n)
{
    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIOC->MODER &= ~(3 << 26);
    GPIOC->MODER |= (1 << 26);
    for(int i = 0; i < n; i++)
    {
        GPIOC->BSRR = (1 << 29);  // ON
        for(volatile int d = 0; d < 2000000; d++);
        GPIOC->BSRR = (1 << 13);  // OFF
        for(volatile int d = 0; d < 2000000; d++);
    }
    // pause between groups
    for(volatile int d = 0; d < 5000000; d++);
}

/* USER CODE END 0 */

/**
	* @brief  The application entry point.
	* @retval int
	*/
int main(void)
{

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	MX_GPIO_Init();
	SystemClock_Config();
	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_DMA_Init();
	MX_I2S3_Init();

	/* USER CODE BEGIN 2 */
	if(HAL_I2SEx_TransmitReceive_DMA(&hi2s3, (uint16_t*)txBuffer, (uint16_t*)rxBuffer, AUDIO_BUFFER_LEN) != HAL_OK)
	{
		BlinkN(9);
		Error_Handler();
	}

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
	  /* USER CODE END WHILE */

	  /* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
	* @brief System Clock Configuration
	* @retval None
	*/
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	*/
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	* in the RCC_OscInitTypeDef structure.
	*/
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 25;
	RCC_OscInitStruct.PLL.PLLN = 196;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
	  Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	*/
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
	                            |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
	{
	  Error_Handler();
	}
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_PLLCLK, RCC_MCODIV_4);
}

/**
	* @brief I2S2 Initialization Function
	* @param None
	* @retval None
	*/
static void MX_I2S3_Init(void)
{

	/* USER CODE BEGIN I2S3_Init 0 */

	/* USER CODE END I2S3_Init 0 */

	/* USER CODE BEGIN I2S3_Init 1 */

	/* USER CODE END I2S3_Init 1 */
	hi2s3.Instance = SPI3;
	hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
	hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
	hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B_EXTENDED;
	hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
	hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_48K;
	hi2s3.Init.CPOL = I2S_CPOL_LOW;
	hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
	hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_ENABLE;
	if (HAL_I2S_Init(&hi2s3) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN I2S3_Init 2 */

	/* USER CODE END I2S3_Init 2 */

}

/**
	* Enable DMA controller clock
	*/
static void MX_DMA_Init(void)
{

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Stream0_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);
	/* DMA1_Stream5_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
	* @brief GPIO Initialization Function
	* @param None
	* @retval None
	*/
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */

	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin : PA8 */
	GPIO_InitStruct.Pin = GPIO_PIN_8;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */

	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
	* @brief  This function is executed in case of error occurrence.
	* @retval None
	*/
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}
