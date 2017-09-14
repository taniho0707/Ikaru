/*
 * @file main.cpp
 */

#include "main.h"
#include "stm32f4xx_hal.h"

#include "Led.h"
#include "ComPc.h"
#include "Battery.h"
#include "Speaker.h"
#include "Gyro.h"
#include "Fram.h"

#include "Motor.h"
#include "Encoder.h"
#include "WallSensor.h"

#include "MotorControl.h"

#include "Walldata.h"


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);


int main(void) {
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();

	Led* led = Led::getInstance();
	led->on(LedNumbers::FRONT);
	HAL_Delay(10);
	led->flickAsync(LedNumbers::FRONT, 5, 200000);
	Speaker* speaker = Speaker::getInstance();
	speaker->playMusic(MusicNumber::KIRBY3_POWERON);
	ComPc* compc = ComPc::getInstance();
	compc->printf("Hello Ikaru!\n");

	Battery* battery = Battery::getInstance();
	battery->scan();
	compc->printf("%f [V]\n", battery->getVoltage());

	Gyro* gyro = Gyro::getInstance();
	HAL_Delay(10);
	if (gyro->whoami()) {
		compc->printf("Gyro WhoAmI was SUCCEEDED!!!\n");
	} else {
		compc->printf("Gyro WhoAmI was failed...\n");
	}

	// Fram* fram = Fram::getInstance();
	// HAL_Delay(10);
	// if (fram->readDeviceId()) {
	// 	compc->printf("Fram RDID was SUCCEEDED!!!\n");
	// } else {
	// 	compc->printf("Fram RDID was failed...\n");
	// }

	// // FRAM
	// __HAL_RCC_SPI3_CLK_ENABLE();
	// __HAL_RCC_GPIOA_CLK_ENABLE();
	// __HAL_RCC_GPIOB_CLK_ENABLE();

	// GPIO_InitTypeDef GPIO_InitStruct;
	// GPIO_InitStruct.Pin = GPIO_PIN_14;
	// GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	// GPIO_InitStruct.Pull = GPIO_NOPULL;
	// GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	// HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	// HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, GPIO_PIN_SET);

	// GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
	// GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	// GPIO_InitStruct.Pull = GPIO_NOPULL;
	// GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	// GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
	// HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// GPIO_InitStruct.Pin = GPIO_PIN_12;
	// GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	// GPIO_InitStruct.Pull = GPIO_NOPULL;
	// GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	// GPIO_InitStruct.Alternate = GPIO_AF7_SPI3;
	// HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// SPI_HandleTypeDef port;
	// port.Instance = SPI3;
	// port.Init.Mode = SPI_MODE_MASTER;
	// port.Init.Direction = SPI_DIRECTION_2LINES;
	// port.Init.DataSize = SPI_DATASIZE_8BIT;
	// port.Init.CLKPolarity = SPI_POLARITY_HIGH;
	// port.Init.CLKPhase = SPI_PHASE_2EDGE;
	// port.Init.NSS = SPI_NSS_SOFT;
	// port.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	// port.Init.FirstBit = SPI_FIRSTBIT_MSB;
	// port.Init.TIMode = SPI_TIMODE_DISABLE;
	// port.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	// port.Init.CRCPolynomial = 10;
	// if (HAL_SPI_Init(&port) != HAL_OK) compc->printf("Error3\n");

	// // AD1_4-8
	// __HAL_RCC_ADC1_CLK_ENABLE();
	// __HAL_RCC_GPIOA_CLK_ENABLE();
	// __HAL_RCC_GPIOB_CLK_ENABLE();
	// GPIO_InitTypeDef GPIO_InitStructADC;
	// GPIO_InitStructADC.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	// GPIO_InitStructADC.Mode = GPIO_MODE_ANALOG;
	// GPIO_InitStructADC.Pull = GPIO_NOPULL;
	// HAL_GPIO_Init(GPIOA, &GPIO_InitStructADC);
	// GPIO_InitStructADC.Pin = GPIO_PIN_0;
	// HAL_GPIO_Init(GPIOB, &GPIO_InitStructADC);

	// ADC_HandleTypeDef hadc1_6;
	// ADC_ChannelConfTypeDef s_config;
	// hadc1_6.Instance = ADC1;
	// hadc1_6.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	// hadc1_6.Init.Resolution = ADC_RESOLUTION_12B;
	// hadc1_6.Init.ScanConvMode = DISABLE;
	// hadc1_6.Init.ContinuousConvMode = DISABLE;
	// hadc1_6.Init.DiscontinuousConvMode = DISABLE;
	// hadc1_6.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	// hadc1_6.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	// hadc1_6.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	// hadc1_6.Init.NbrOfConversion = 1;
	// hadc1_6.Init.DMAContinuousRequests = DISABLE;
	// hadc1_6.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	// HAL_ADC_Init(&hadc1_6);
	// s_config.Rank = 1;
	// s_config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	// s_config.Channel = ADC_CHANNEL_4|ADC_CHANNEL_5|ADC_CHANNEL_6|ADC_CHANNEL_7|ADC_CHANNEL_8;
	// HAL_ADC_ConfigChannel(&hadc1_6, &s_config);

	// __HAL_RCC_GPIOA_CLK_ENABLE();
	// GPIO_InitTypeDef GPIO_InitStruct3;
	// GPIO_InitStruct3.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
	// GPIO_InitStruct3.Mode = GPIO_MODE_OUTPUT_PP;
	// GPIO_InitStruct3.Pull = GPIO_NOPULL;
	// GPIO_InitStruct3.Speed = GPIO_SPEED_FREQ_LOW;
	// HAL_GPIO_Init(GPIOA, &GPIO_InitStruct3);
	// GPIO_InitStruct3.Pin = GPIO_PIN_10|GPIO_PIN_2;
	// HAL_GPIO_Init(GPIOB, &GPIO_InitStruct3);

	Encoder* encoder = Encoder::getInstance();

	// WallSensor* wallsensor = WallSensor::getInstance();
	// wallsensor->start();
	ADC_HandleTypeDef hadc1_6;
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	ADC_ChannelConfTypeDef s_config;
	hadc1_6.Instance = ADC1;
	hadc1_6.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	hadc1_6.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1_6.Init.ScanConvMode = DISABLE;
	hadc1_6.Init.ContinuousConvMode = DISABLE;
	hadc1_6.Init.DiscontinuousConvMode = DISABLE;
	hadc1_6.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1_6.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1_6.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1_6.Init.NbrOfConversion = 1;
	hadc1_6.Init.DMAContinuousRequests = DISABLE;
	hadc1_6.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	HAL_ADC_Init(&hadc1_6);
	s_config.Rank = 1;
	s_config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	s_config.Channel = ADC_CHANNEL_4|ADC_CHANNEL_5|ADC_CHANNEL_6|ADC_CHANNEL_7|ADC_CHANNEL_8;
	HAL_ADC_ConfigChannel(&hadc1_6, &s_config);

	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct3;
	GPIO_InitStruct3.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
	GPIO_InitStruct3.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct3.Pull = GPIO_NOPULL;
	GPIO_InitStruct3.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct3);
	GPIO_InitStruct3.Pin = GPIO_PIN_10|GPIO_PIN_2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct3);
	int sensorvalue[5];

	// MotorControl* mc = MotorControl::getInstance();
	// mc->stay();

	/* Infinite loop */
	while (1) {
		led->off(LedNumbers::FRONT);
		HAL_Delay(99);
		led->on(LedNumbers::FRONT);

		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);

		for (int i=0; i<100; ++i);

		ADC_ChannelConfTypeDef s_config;
		s_config.Channel = ADC_CHANNEL_4;
		HAL_ADC_ConfigChannel(&hadc1_6, &s_config);
		HAL_ADC_Start(&hadc1_6);
		HAL_ADC_PollForConversion(&hadc1_6, 10);
		sensorvalue[0] = HAL_ADC_GetValue(&hadc1_6);
		s_config.Channel = ADC_CHANNEL_5;
		HAL_ADC_ConfigChannel(&hadc1_6, &s_config);
		HAL_ADC_Start(&hadc1_6);
		HAL_ADC_PollForConversion(&hadc1_6, 10);
		sensorvalue[1] = HAL_ADC_GetValue(&hadc1_6);
		s_config.Channel = ADC_CHANNEL_6;
		HAL_ADC_ConfigChannel(&hadc1_6, &s_config);
		HAL_ADC_Start(&hadc1_6);
		HAL_ADC_PollForConversion(&hadc1_6, 10);
		sensorvalue[2] = HAL_ADC_GetValue(&hadc1_6);
		s_config.Channel = ADC_CHANNEL_7;
		HAL_ADC_ConfigChannel(&hadc1_6, &s_config);
		HAL_ADC_Start(&hadc1_6);
		HAL_ADC_PollForConversion(&hadc1_6, 10);
		sensorvalue[3] = HAL_ADC_GetValue(&hadc1_6);
		s_config.Channel = ADC_CHANNEL_8;
		HAL_ADC_ConfigChannel(&hadc1_6, &s_config);
		HAL_ADC_Start(&hadc1_6);
		HAL_ADC_PollForConversion(&hadc1_6, 10);
		sensorvalue[4] = HAL_ADC_GetValue(&hadc1_6);

		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2,  GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);

		// compc->printf("%4d %4d %4d %4d %4d, %f, %f %f\n", wallsensor->getValue(SensorPosition::FLeft), wallsensor->getValue(SensorPosition::Left), wallsensor->getValue(SensorPosition::Front), wallsensor->getValue(SensorPosition::Right), wallsensor->getValue(SensorPosition::FRight), gyro->getGyroYaw(), encoder->getVelocity(EncoderSide::LEFT), encoder->getVelocity(EncoderSide::RIGHT));
		compc->printf("%6d %6d %6d %6d %6d\n", sensorvalue[4], sensorvalue[3], sensorvalue[2], sensorvalue[1], sensorvalue[0], battery->getValue());
	}
}

/** System Clock Configuration
*/
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = 16;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 100;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

    /**Initializes the CPU, AHB and APB busses clocks 
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
		|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

    /**Configure the Systick interrupt time 
	 */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
	 */
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

	/* SysTick_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}



/** NVIC Configuration
*/
static void MX_NVIC_Init(void) {
	/* DMA1_Stream3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
	/* DMA1_Stream4_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
	/* ADC_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADC_IRQn);
	/* SPI2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SPI2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(SPI2_IRQn);
	/* SPI3_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(SPI3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(SPI3_IRQn);
	/* DMA2_Stream0_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
	/* DMA2_Stream7_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream7_IRQn);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	while(1) {
	}
	/* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
