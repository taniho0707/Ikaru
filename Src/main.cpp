/*
 * @file main.cpp
 */

#include "main.h"
#include "stm32f4xx_hal.h"

#include "SlalomParams.h"

#include "Led.h"
#include "ComPc.h"
#include "Battery.h"
#include "Speaker.h"
#include "Gyro.h"
#include "Fram.h"
#include "Datalog.h"

#include "Motor.h"
#include "Encoder.h"
#include "WallSensor.h"

#include "MotorControl.h"
#include "VelocityControl.h"
#include "ModeSelect.h"
#include "MotorCollection.h"

#include "Walldata.h"

#include "Position.h"
#include "Map.h"
#include "Footmap.h"

#include "MethodAdachi.h"

using namespace slalomparams;

void SystemClock_Config(void);
static void MX_NVIC_Init(void);


void frontcorrection(){
	Led* led = Led::getInstance();
	MotorControl* mc = MotorControl::getInstance();
	MotorCollection* motorcollection = MotorCollection::getInstance();
	led->flickAsync(LedNumbers::FRONT, 5.0f, 1500);
	mc->disableWallControl();
	motorcollection->collectionByFrontDuringStop(0.03f);
	mc->stay();
	mc->resetRadIntegral();
	mc->resetLinIntegral();
	led->flickStop(LedNumbers::FRONT);
	HAL_Delay(300);
}

int main(void) {
	HAL_Init();
	SystemClock_Config();

	Led* led = Led::getInstance();
	led->on(LedNumbers::FRONT);
	HAL_Delay(10);
	led->flickAsync(LedNumbers::FRONT, 5, 1000);
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

	HAL_Delay(2000);
	led->flickSync(LedNumbers::FRONT, 10, 1000);
	led->flickAsync(LedNumbers::FRONT, 4, 2000);
	gyro->resetCalibration();

	Encoder* encoder = Encoder::getInstance();

	WallSensor* wallsensor = WallSensor::getInstance();
	wallsensor->start();

	HAL_Delay(50);
	if (wallsensor->getValue(SensorPosition::Front) > 1000) {
		Datalog *log = Datalog::getInstance();
		constexpr auto num = 10;
		for(auto i=0; i<log->getSize()/num; ++i){
			compc->printf("%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", i,
						  log->readFloat(num*i),
						  log->readFloat(num*i+1),
						  log->readFloat(num*i+2),
						  log->readFloat(num*i+3),
						  log->readFloat(num*i+4),
						  log->readFloat(num*i+5),
						  log->readFloat(num*i+6),
						  log->readFloat(num*i+7),
						  log->readFloat(num*i+8),
						  log->readFloat(num*i+9)
				);
		}
	}

	ModeSelect* mode = ModeSelect::getInstance();
	speaker->playMusic(MusicNumber::KIRBY3_CURSOR);
	mode::StructMode decided_mode = mode->select();
	bool enabled_mae = false;
	
	switch(decided_mode.prime){
	case static_cast<uint8_t>(mode::MODE_PRIME::HIDARITE):
	{
		switch(decided_mode.sub){
		case static_cast<uint8_t>(mode::MODE_HIDARITE::WITHOUT_MAE):
			enabled_mae = false;
			break;
		case static_cast<uint8_t>(mode::MODE_HIDARITE::WITH_MAE):
			enabled_mae = true;
			break;
		default:
			break;
		}
		// ここから左手法で走る
		mode->startcheck(0);
		led->initPort(LedNumbers::TOP1);
		led->flickAsync(LedNumbers::TOP1, 4.0f, 2000);
		speaker->playMusic(MusicNumber::HIRAPA);
		HAL_Delay(1000);

		MotorControl* mc = MotorControl::getInstance();
		mc->stay();
		led->on(LedNumbers::FRONT);
		VelocityControl* vc = VelocityControl::getInstance();
		vc->disableWallgap();

		MotorCollection* motorcollection = MotorCollection::getInstance();
		vc->runTrapAccel(0.0f, 0.3f, 0.3f, 0.045f, 3.0f);
		// vc->enableWallgap();
		mc->enableWallControl();
		while(vc->isRunning());

		while (1) {
			// vc->runTrapAccel(0.3f, 0.3f, 0.3f, 0.09f, 5.0f);
			// // vc->disableWallgap();
			// while(vc->isRunning());
			if(wallsensor->isExistWall(SensorPosition::Left) == false){
				vc->runSlalom(RunType::SLALOM90SML_LEFT, 0.3f);
				// vc->disableWallgap();
				mc->disableWallControl();
				while(vc->isRunning());
			} else if(wallsensor->isExistWall(SensorPosition::Front) == false){
				vc->runTrapAccel(0.3f, 0.3f, 0.3f, 0.09f, 5.0f);
				// vc->disableWallgap();
				mc->enableWallControl();
				while(vc->isRunning());
			} else if(wallsensor->isExistWall(SensorPosition::Right) == false){
				vc->runSlalom(RunType::SLALOM90SML_RIGHT, 0.3f);
				// vc->disableWallgap();
				mc->disableWallControl();
				while(vc->isRunning());
			} else {
				vc->runTrapAccel(0.3f, 0.3f, 0.0f, 0.045f, 5.0f);
				// vc->disableWallgap();
				mc->disableWallControl();
				while(vc->isRunning());

				if(enabled_mae){
					led->flickAsync(LedNumbers::FRONT, 5.0f, 1500);
					mc->disableWallControl();
					motorcollection->collectionByFrontDuringStop(0.03f);
					mc->stay();
					mc->resetRadIntegral();
					mc->resetLinIntegral();
					led->flickStop(LedNumbers::FRONT);
					HAL_Delay(300);
					vc->runPivotTurn(500, 90, 1000);
					mc->disableWallControl();
					while(vc->isRunning());
					led->flickAsync(LedNumbers::FRONT, 5.0f, 1500);
					mc->disableWallControl();
					motorcollection->collectionByFrontDuringStop(0.03f);
					mc->stay();
					mc->resetRadIntegral();
					mc->resetLinIntegral();
					led->flickStop(LedNumbers::FRONT);
					led->on(LedNumbers::FRONT);
					HAL_Delay(300);
					vc->runPivotTurn(500, 90, 1000);
					mc->disableWallControl();
					while(vc->isRunning());
					HAL_Delay(300);
				} else {
					vc->runPivotTurn(500, 180, 5000);
					mc->disableWallControl();
					while(vc->isRunning());
				}
			
				vc->runTrapAccel(0.0f, 0.3f, 0.3f, 0.045f, 5.0f);
				// vc->disableWallgap();
				mc->disableWallControl();
				while(vc->isRunning());
			}
		}
	}
	break;
	case static_cast<uint8_t>(mode::MODE_PRIME::EXPR):
	{
		switch(decided_mode.sub){
		case static_cast<uint8_t>(mode::MODE_EXPR::WITHOUT_MAE):
			enabled_mae = false;
			break;
		case static_cast<uint8_t>(mode::MODE_EXPR::WITH_MAE):
			enabled_mae = true;
			break;
		default:
			break;
		}

		mode->startcheck(0);
		led->initPort(LedNumbers::TOP1);
		led->flickAsync(LedNumbers::TOP1, 4.0f, 2000);
		speaker->playMusic(MusicNumber::HIRAPA);
		HAL_Delay(1000);

		MotorControl* mc = MotorControl::getInstance();
		mc->stay();
		led->on(LedNumbers::FRONT);
		VelocityControl* vc = VelocityControl::getInstance();
		vc->disableWallgap();
		MotorCollection* motorcollection = MotorCollection::getInstance();

		mc->stay();
		vc->runTrapAccel(0.0f, 0.3f, 0.3f, 0.045f, 5.0f);
		while(vc->isRunning());
		vc->startTrapAccel(0.3f, 0.3f, 0.09f, 5.0f);

		Walldata walldata;
		Map map;
		Position pos;
		MethodAdachi adachi;
		map.setReached(0, 0);
		adachi.setGoal(6, 7);

		while(true){
			led->off(LedNumbers::FRONT);
			walldata = wallsensor->getWall();
			map.setWall(pos.getPositionX(), pos.getPositionY(), pos.getAngle(), walldata);
			map.setReached(pos.getPositionX(), pos.getPositionY());
			adachi.setCurrent(pos.getPositionX(), pos.getPositionY());
			adachi.setMap(map);
			adachi.renewFootmap();
			RunType runtype = adachi.getNextMotion(pos.getPositionX(), pos.getPositionY(), pos.getAngle(), walldata);
			led->on(LedNumbers::FRONT);
		
			if(runtype == slalomparams::RunType::TRAPACCEL){
				vc->runTrapAccel(0.3f, 0.3f, 0.3f, 0.09f, 3.0f);
				while(vc->isRunning());
			} else if(runtype == slalomparams::RunType::PIVOTTURN){
				vc->runTrapAccel(0.3f, 0.3f, 0.0f, 0.045f, 5.0f);
				mc->disableWallControl();
				while(vc->isRunning());
				if(walldata.isExistWall(MouseAngle::FRONT)){
					frontcorrection();
				}
				if(walldata.isExistWall(MouseAngle::RIGHT)){
					vc->runPivotTurn(500, 90, 1000);
					while(vc->isRunning());
					frontcorrection();
					vc->runPivotTurn(500, 90, 1000);
					while(vc->isRunning());
					mc->resetRadIntegral();
					mc->resetLinIntegral();
				} else {
					vc->runPivotTurn(500, 180, 1000);
					while(vc->isRunning());
					mc->disableWallControl();
					mc->resetRadIntegral();
					mc->resetLinIntegral();
				}

				mc->resetDistanceFromGap();
				mc->resetDistanceFromGapDiago();
				vc->runTrapAccel(0.0f, 0.30f, 0.30f, 0.045f, 5.0f);
				mc->disableWallControl();
				while(vc->isRunning());
			} else if(runtype == slalomparams::RunType::SLALOM90SML_RIGHT){
				vc->runSlalom(RunType::SLALOM90SML_RIGHT, 0.3f);
				while(vc->isRunning());
			} else if(runtype == slalomparams::RunType::SLALOM90SML_LEFT){
				vc->runSlalom(RunType::SLALOM90SML_LEFT, 0.3f);
				while(vc->isRunning());
			} else {
				vc->runTrapAccel(0.3f, 0.3f, 0.0f, 0.045f, 4.0f);
				while(vc->isRunning());
				break;
			}
		
			pos.setNextPosition(runtype);
			vc->startTrapAccel(0.3f, 0.3f, 0.09f, 5.0f);
		
			static bool is_first_goal = true; // = has_goal
			if(pos.getPositionX() == 6 && pos.getPositionY() == 7){ // && is_first_goal){
				is_first_goal = false;
				led->on(LedNumbers::TOP1);
				walldata = wallsensor->getWall();
				map.setWall(pos.getPositionX(), pos.getPositionY(), pos.getAngle(), walldata);
				map.setReached(pos.getPositionX(), pos.getPositionY());
				led->off(LedNumbers::TOP1);

				led->on(LedNumbers::TOP1);
				adachi.setGoal(0, 0);
			} else if(pos.getPositionX() == 0 && pos.getPositionY() == 0){
				vc->runTrapAccel(0.3f, 0.3f, 0.0f, 0.045f, 5.0f);
				while(vc->isRunning());
				mc->disableWallControl();
				break;
			}
		}
	}
	break;
	}

	
	// // vc->runTrapAccel(0.0f, 0.3f, 0.3f, 0.135f, 5.0f);
	// // while(vc->isRunning());
	// while(1){
	// 	// vc->runTrapAccel(0.0f, 0.3f, 0.3f, 0.09f, 5.0f);
	// 	// while(vc->isRunning());
	// 	// vc->runSlalom(RunType::SLALOM90SML_RIGHT, 0.3f);
	// 	// while(vc->isRunning());
	// 	vc->runPivotTurn(500, 180, 2000);
	// 	while(vc->isRunning());
	// 	HAL_Delay(1000);
	// }

	// vc->runTrapAccel(0.0f, 0.1f, 0.0f, 0.27f, 5.0f);
	// while(vc->isRunning());
	// HAL_Delay(10000);
	// vc->runTrapAccel(0.0f, 0.3f, 0.3f, 0.135f, 5.0f);
	// while(vc->isRunning());
	// vc->runSlalom(RunType::SLALOM90SML_LEFT, 0.3f);
	// while(vc->isRunning());
	// vc->runTrapAccel(0.3f, 0.3f, 0.0f, 0.135f, 5.0f);
	// mc->disableWallControl();
	// while(vc->isRunning());

	// vc->runTrapAccel(0.0f, 0.3f, 0.0f, 1.26f, 3.0f);
	// vc->disableWallgap();
	// mc->disableWallControl();
	// mc->stay();
	// Motor* motor = Motor::getInstance();
	// motor->disable();
	// HAL_Delay(10000);
	// speaker->playMusic(MusicNumber::KIRBY_GOURMETRACE);
	// Datalog *log = Datalog::getInstance();
	// constexpr auto num = 10;
	// for(auto i=0; i<log->getSize()/num; ++i){
	// 	compc->printf("%d\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", i,
	// 				  log->readFloat(num*i),
	// 				  log->readFloat(num*i+1),
	// 				  log->readFloat(num*i+2),
	// 				  log->readFloat(num*i+3),
	// 				  log->readFloat(num*i+4),
	// 				  log->readFloat(num*i+5),
	// 				  log->readFloat(num*i+6),
	// 				  log->readFloat(num*i+7),
	// 				  log->readFloat(num*i+8),
	// 				  log->readFloat(num*i+9)
	// 		);
	// }
	


	// if (gyro->readAccelZ() < 0) {
	while(1){
		led->off(LedNumbers::FRONT);
		HAL_Delay(99);
		led->on(LedNumbers::FRONT);
		HAL_Delay(1);
		compc->printf("[%4d %4d %4d %4d %4d ] %+7d %+7d %+7d : %+7d %+7d %+7d, %f %f\n", wallsensor->getValue(SensorPosition::FLeft), wallsensor->getValue(SensorPosition::Left), wallsensor->getValue(SensorPosition::Front), wallsensor->getValue(SensorPosition::Right), wallsensor->getValue(SensorPosition::FRight), gyro->readGyroX(), gyro->readGyroY(), gyro->readGyroZ(), gyro->readAccelX(), gyro->readAccelY(), gyro->readAccelZ(), encoder->getVelocity(EncoderSide::LEFT), encoder->getVelocity(EncoderSide::RIGHT));
	}
	// }
}




/** System Clock Configuration */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks */
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

    /**Initializes the CPU, AHB and APB busses clocks */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
		|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK) {
		_Error_Handler(__FILE__, __LINE__);
	}

    /**Configure the Systick interrupt time */
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick */
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
