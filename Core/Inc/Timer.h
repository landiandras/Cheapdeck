/*
 * Timer.h
 *
 *  Created on: Apr 2, 2026
 *      Author: Landi András
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"
#include "stm32f4xx_hal.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif /* INC_TIMER_H_ */
