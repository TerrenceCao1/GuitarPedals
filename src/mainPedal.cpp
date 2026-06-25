// mainPedal.cpp
extern "C" {
    #include "stm32f4xx_hal.h"
	#include "stm32f4xx_hal_gpio.h"
}

#include <iostream>
#include "AudioFrame.h"
#include "Effects.h"

// Pin Definitions:
#define I2S2_CK			GPIO_PIN_13
#define I2S2_WS			GPIO_PIN_12
#define I2S_DATA_IN		GPIO_PIN_14 // Data comes in from the ADC
#define I2S_DATA_OUT	GPIO_PIN_15 // Data goes out to the DAC

#define I2S_GPIO_PORT	GPIOB	

static void I2S_GPIO_Init(void)
{
	// Output
	GPIO_InitTypeDef gpio = {0};
	gpio.Pin	= I2S2_CK | I2S2_WS | I2S_DATA_OUT;
	gpio.Mode	= GPIO_MODE_OUTPUT_PP;
	gpio.Pull	= GPIO_NOPULL;
	gpio.Speed	= GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(I2S_GPIO_PORT, &gpio);

	// Input
	gpio.Pin	= I2S_DATA_IN;
	gpio.Mode	= GPIO_MODE_INPUT;
	gpio.Pull	= GPIO_NOPULL;
	HAL_GPIO_Init(I2S_GPIO_PORT, &gpio);
}

int main(void)
{
    HAL_Init();
	I2S_GPIO_Init();

    while (1)
    {
    }
}
