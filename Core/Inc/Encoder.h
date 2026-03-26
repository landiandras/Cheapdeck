/*
 * Encoder.h
 *
 *  Created on: Mar 26, 2026
 *      Author: Landi András
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include <stdbool.h>
#include <stdint.h>
#include "main.h"
#include "stm32f4xx_hal.h"


extern void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);
bool GetEncoderButtonState();

#endif /* INC_ENCODER_H_ */
