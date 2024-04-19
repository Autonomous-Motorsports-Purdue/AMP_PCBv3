/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define STR_EN__Pin GPIO_PIN_0
#define STR_EN__GPIO_Port GPIOC
#define STR_INPUTA__Pin GPIO_PIN_1
#define STR_INPUTA__GPIO_Port GPIOC
#define STR_INPUTA__DIR_Pin GPIO_PIN_2
#define STR_INPUTA__DIR_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define STR_INPUTB__DIR_Pin GPIO_PIN_4
#define STR_INPUTB__DIR_GPIO_Port GPIOA
#define STAT3_Pin GPIO_PIN_5
#define STAT3_GPIO_Port GPIOA
#define IMU_INT1_Pin GPIO_PIN_4
#define IMU_INT1_GPIO_Port GPIOC
#define IMU_INT1_EXTI_IRQn EXTI4_IRQn
#define BRAKE_CS_Pin GPIO_PIN_5
#define BRAKE_CS_GPIO_Port GPIOC
#define STR_INPUTB__Pin GPIO_PIN_0
#define STR_INPUTB__GPIO_Port GPIOB
#define LED_SIG_Pin GPIO_PIN_1
#define LED_SIG_GPIO_Port GPIOB
#define STAT1_Pin GPIO_PIN_12
#define STAT1_GPIO_Port GPIOB
#define LORA_NSS_Pin GPIO_PIN_13
#define LORA_NSS_GPIO_Port GPIOB
#define BRAKE_INB_Pin GPIO_PIN_6
#define BRAKE_INB_GPIO_Port GPIOC
#define BRAKE_INA_Pin GPIO_PIN_8
#define BRAKE_INA_GPIO_Port GPIOC
#define VESC_PPM_Pin GPIO_PIN_9
#define VESC_PPM_GPIO_Port GPIOC
#define IMU_CS_Pin GPIO_PIN_10
#define IMU_CS_GPIO_Port GPIOA
#define STAT2_Pin GPIO_PIN_11
#define STAT2_GPIO_Port GPIOA
#define STAT4_Pin GPIO_PIN_12
#define STAT4_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define FAN4_Pin GPIO_PIN_15
#define FAN4_GPIO_Port GPIOA
#define FAN1_Pin GPIO_PIN_10
#define FAN1_GPIO_Port GPIOC
#define FAN2_Pin GPIO_PIN_11
#define FAN2_GPIO_Port GPIOC
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define LORA_RST_Pin GPIO_PIN_4
#define LORA_RST_GPIO_Port GPIOB
#define LORA_DIO0_Pin GPIO_PIN_5
#define LORA_DIO0_GPIO_Port GPIOB
#define LORA_DIO0_EXTI_IRQn EXTI9_5_IRQn
#define FAN3_Pin GPIO_PIN_7
#define FAN3_GPIO_Port GPIOB
#define BRAKE_SEL0_Pin GPIO_PIN_8
#define BRAKE_SEL0_GPIO_Port GPIOB
#define BRAKE_PWM_Pin GPIO_PIN_9
#define BRAKE_PWM_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
