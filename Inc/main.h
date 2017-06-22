/**
 * @file main.h
 */
#pragma once

#define GYRO_CS_Pin GPIO_PIN_13
#define GYRO_CS_GPIO_Port GPIOC
#define ENCODER1_A_Pin GPIO_PIN_0
#define ENCODER1_A_GPIO_Port GPIOA
#define ENCODER1_B_Pin GPIO_PIN_1
#define ENCODER1_B_GPIO_Port GPIOA
#define SPEAKER_Pin GPIO_PIN_2
#define SPEAKER_GPIO_Port GPIOA
#define FRAM_SCK_Pin GPIO_PIN_12
#define FRAM_SCK_GPIO_Port GPIOB
#define GYRO_SCK_Pin GPIO_PIN_13
#define GYRO_SCK_GPIO_Port GPIOB
#define GYRO_MISO_Pin GPIO_PIN_14
#define GYRO_MISO_GPIO_Port GPIOB
#define GYRO_MOSI_Pin GPIO_PIN_15
#define GYRO_MOSI_GPIO_Port GPIOB
#define LED1_PWM_Pin GPIO_PIN_8
#define LED1_PWM_GPIO_Port GPIOA
#define WRITER_RX_Pin GPIO_PIN_9
#define WRITER_RX_GPIO_Port GPIOA
#define WRITER_TX_Pin GPIO_PIN_10
#define WRITER_TX_GPIO_Port GPIOA
#define LED2_PWM_Pin GPIO_PIN_11
#define LED2_PWM_GPIO_Port GPIOA
#define FRAM_CS_Pin GPIO_PIN_14
#define FRAM_CS_GPIO_Port GPIOA
#define ENCODER2_B_Pin GPIO_PIN_15
#define ENCODER2_B_GPIO_Port GPIOA
#define ENCODER2_A_Pin GPIO_PIN_3
#define ENCODER2_A_GPIO_Port GPIOB
#define FRAM_MISO_Pin GPIO_PIN_4
#define FRAM_MISO_GPIO_Port GPIOB
#define FRAM_MOSI_Pin GPIO_PIN_5
#define FRAM_MOSI_GPIO_Port GPIOB
#define MOTOR2_ENABLE_Pin GPIO_PIN_6
#define MOTOR2_ENABLE_GPIO_Port GPIOB
#define MOTOR2_PHASE_Pin GPIO_PIN_7
#define MOTOR2_PHASE_GPIO_Port GPIOB
#define MOTOR1_ENABLE_Pin GPIO_PIN_8
#define MOTOR1_ENABLE_GPIO_Port GPIOB
#define MOTOR1_PHASE_Pin GPIO_PIN_9
#define MOTOR1_PHASE_GPIO_Port GPIOB

/* #include "Led.h" */
/* #include "ComPc.h" */


extern "C" {
	void _Error_Handler(char *, int);
#define Error_Handler() _Error_Handler(__FILE__, __LINE__)
}
