/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include <stdbool.h>
#include <stdint.h>
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DIS_CS_Pin GPIO_PIN_0
#define DIS_CS_GPIO_Port GPIOC
#define DIS_RES_Pin GPIO_PIN_1
#define DIS_RES_GPIO_Port GPIOC
#define DIS_A0_Pin GPIO_PIN_2
#define DIS_A0_GPIO_Port GPIOC
#define BL_PWM_Pin GPIO_PIN_3
#define BL_PWM_GPIO_Port GPIOA
#define DIS_SCL_Pin GPIO_PIN_5
#define DIS_SCL_GPIO_Port GPIOA
#define DIS_SI_Pin GPIO_PIN_7
#define DIS_SI_GPIO_Port GPIOA
#define Col0_Pin GPIO_PIN_0
#define Col0_GPIO_Port GPIOB
#define Col1_Pin GPIO_PIN_1
#define Col1_GPIO_Port GPIOB
#define Col2_Pin GPIO_PIN_2
#define Col2_GPIO_Port GPIOB
#define ENC_BTN_Pin GPIO_PIN_13
#define ENC_BTN_GPIO_Port GPIOB
#define ENC_BTN_EXTI_IRQn EXTI15_10_IRQn
#define ENC_CHA_Pin GPIO_PIN_14
#define ENC_CHA_GPIO_Port GPIOB
#define ENC_CHA_EXTI_IRQn EXTI15_10_IRQn
#define ENC_CHB_Pin GPIO_PIN_15
#define ENC_CHB_GPIO_Port GPIOB
#define ENC_CHB_EXTI_IRQn EXTI15_10_IRQn
#define Col3_Pin GPIO_PIN_3
#define Col3_GPIO_Port GPIOB
#define Row0_Pin GPIO_PIN_4
#define Row0_GPIO_Port GPIOB
#define Row1_Pin GPIO_PIN_5
#define Row1_GPIO_Port GPIOB
#define Row2_Pin GPIO_PIN_6
#define Row2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
typedef struct __attribute__((packed)) {
	uint8_t REPORTID;
    uint8_t MODIFIER; // Ctrl, Shift, Alt, etc.
    uint8_t RESERVED; // Always 0
    uint8_t KEYCODE1; // Key 1
    uint8_t KEYCODE2; // Key 2
    uint8_t KEYCODE3; // Key 3
    uint8_t KEYCODE4; // Key 4
    uint8_t KEYCODE5; // Key 5
    uint8_t KEYCODE6; // Key 6
} HIDkeypress;

typedef struct __attribute__((packed)) {
	uint8_t REPORTID;
	uint8_t DATA[63];
} HIDdata;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
