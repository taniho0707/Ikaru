/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
// #include "adc.h"
// #include "dma.h"
// #include "spi.h"
// #include "tim.h"
// #include "usart.h"
// #include "gpio.h"
#include "Led.h"
#include "ComPc.h"
#include "Battery.h"
#include "Gyro.h"

#include "Walldata.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void) {

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration----------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */
	
	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	// MX_GPIO_Init();
	// MX_DMA_Init();
	// MX_USART1_UART_Init();
	// MX_SPI2_Init();
	// MX_TIM4_Init();
	// MX_SPI3_Init();
	// MX_TIM1_Init();
	// MX_ADC1_Init();
	// MX_TIM9_Init();
	// MX_TIM2_Init();
	// MX_TIM5_Init();

	/* Initialize interrupts */
	// MX_NVIC_Init();


	Led* led = Led::getInstance();
	led->flickSync(LedNumbers::FRONT, 2, 2000);
	led->flickAsync(LedNumbers::FRONT, 5, 2000);
	led->on(LedNumbers::FRONT);
	ComPc* compc = ComPc::getInstance();
	compc->printf("Hello Ikaru!\n");

	Battery* battery = Battery::getInstance();
	battery->scan();
	compc->printf("%f [V]\n", battery->getVoltage());

	compc->printf("Gyro Setting...\n");
	// Gyro* gyro = Gyro::getInstance();

	// if (gyro->whoami()) {
	// 	compc->printf("Gyro WhoAmI was SUCCEEDED!!!\n");
	// } else {
	// 	compc->printf("Gyro WhoAmI was failed...\n");
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

	// GYRO
	__HAL_RCC_SPI2_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);

	GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	SPI_HandleTypeDef port;
	port.Instance = SPI2;
	port.Init.Mode = SPI_MODE_MASTER;
	port.Init.Direction = SPI_DIRECTION_2LINES;
	port.Init.DataSize = SPI_DATASIZE_8BIT;
	port.Init.CLKPolarity = SPI_POLARITY_HIGH;
	port.Init.CLKPhase = SPI_PHASE_2EDGE;
	port.Init.NSS = SPI_NSS_SOFT;
	port.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_64;
	port.Init.FirstBit = SPI_FIRSTBIT_MSB;
	port.Init.TIMode = SPI_TIMODE_DISABLE;
	port.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	port.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&port) != HAL_OK) compc->printf("Error3\n");

	uint8_t recv[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t data[5] = {0x8F, 0x00, 0x00, 0x00, 0x00};
	if (HAL_SPI_Transmit(&port, data, 1, 1000) != HAL_OK) compc->printf("Error4\n");
	// HAL_SPI_Receive(&port, bulk, 1, 1000);

	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	// HAL_Delay(1);
	if (HAL_SPI_TransmitReceive(&port, data, recv, 2, 1000) != HAL_OK) compc->printf("Error5\n");
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
	compc->printf("write: %2x, %2x, %2x, %2x, %2x\n", data[0], data[1], data[2], data[3], data[4]);
	compc->printf("read : %2x, %2x, %2x, %2x, %2x\n", recv[0], recv[1], recv[2], recv[3], recv[4]);


	// AD1_4-8
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStructADC;
    GPIO_InitStructADC.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStructADC.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStructADC.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStructADC);
    GPIO_InitStructADC.Pin = GPIO_PIN_0;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructADC);
	ADC_HandleTypeDef hadc1_6;
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
	HAL_Delay(1000);

	// // motor_left right
	// GPIO_InitTypeDef GPIO_InitStruct4;
	// GPIO_InitStruct4.Pin = GPIO_PIN_8|GPIO_PIN_9 | GPIO_PIN_6|GPIO_PIN_7;
	// GPIO_InitStruct4.Mode = GPIO_MODE_OUTPUT_PP;
	// GPIO_InitStruct4.Pull = GPIO_NOPULL;
	// GPIO_InitStruct4.Speed = GPIO_SPEED_FREQ_LOW;
	// HAL_GPIO_Init(GPIOB, &GPIO_InitStruct4);
	// HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
	// HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);
	// HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
	// HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

	// speaker
	GPIO_InitTypeDef GPIO_InitStruct5;
	GPIO_InitStruct5.Pin = GPIO_PIN_2;
	GPIO_InitStruct5.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct5.Pull = GPIO_NOPULL;
	GPIO_InitStruct5.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct5);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);

	// encoder
	__TIM5_CLK_ENABLE();
	__TIM2_CLK_ENABLE();
	__HAL_RCC_TIM5_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct6;
    GPIO_InitStruct6.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct6.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct6.Pull = GPIO_PULLUP; // NOPULL
    GPIO_InitStruct6.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct6.Alternate = GPIO_AF2_TIM5;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct6);
    GPIO_InitStruct6.Alternate = GPIO_AF1_TIM2;
    GPIO_InitStruct6.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct6);
	GPIO_InitStruct6.Pin = GPIO_PIN_15;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct6);
	TIM_HandleTypeDef htim2;
	TIM_HandleTypeDef htim5;
	TIM_Encoder_InitTypeDef sConfig_enl;
	TIM_MasterConfigTypeDef sMasterConfig;
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 0;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim5.Instance = TIM5;
	htim5.Init.Prescaler = 0;
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim5.Init.Period = 0;
	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	sConfig_enl.EncoderMode = TIM_ENCODERMODE_TI12;
	sConfig_enl.IC1Polarity = TIM_ICPOLARITY_BOTHEDGE;
	sConfig_enl.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig_enl.IC1Prescaler = TIM_ICPSC_DIV1;
	sConfig_enl.IC1Filter = 0;
	sConfig_enl.IC2Polarity = TIM_ICPOLARITY_BOTHEDGE;
	sConfig_enl.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig_enl.IC2Prescaler = TIM_ICPSC_DIV1;
	sConfig_enl.IC2Filter = 0;
	if (HAL_TIM_Encoder_Init(&htim2, &sConfig_enl) != HAL_OK) compc->printf("EN_ERROR_1");
	if (HAL_TIM_Encoder_Init(&htim5, &sConfig_enl) != HAL_OK) compc->printf("EN_ERROR_1");
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) compc->printf("EN_ERROR_2");;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK) compc->printf("EN_ERROR_2");;
	TIM2->CNT = 30000;
	TIM5->CNT = 30000;
	HAL_Delay(500);
	if (HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL) != HAL_OK) compc->printf("EN_ERROR_3");
	if (HAL_TIM_Encoder_Start(&htim5, TIM_CHANNEL_ALL) != HAL_OK) compc->printf("EN_ERROR_3");

	int sensorvalue[5];

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
		HAL_Delay(1);
		//
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

		// HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);
		// HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_RESET);
		// HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_RESET);
		// HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2,  GPIO_PIN_RESET);
		// HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
		compc->printf("%4d %4d %4d %4d %4d, %6d %6d\n", sensorvalue[0], sensorvalue[1], sensorvalue[2], sensorvalue[3], sensorvalue[4], __HAL_TIM_GetCounter(&htim2), __HAL_TIM_GetCounter(&htim5));

		// while(1){
		// 	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
		// 	HAL_Delay(1);
		// 	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
		// 	HAL_Delay(1);
		// };
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
