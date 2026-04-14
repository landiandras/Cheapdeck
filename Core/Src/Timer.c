/*
 * Timer.c
 *
 *  Created on: Apr 2, 2026
 *      Author: Landi András
 */

#include "Timer.h"
extern TIM_HandleTypeDef htim3;
extern bool RefreshScreen;
extern bool UpdateButtons;
volatile uint32_t counter;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim == &htim3){
		++counter;
		UpdateButtons = true;
		if(counter % 35){
			RefreshScreen=true;
		}

	}
}

