/*
 * Encoder.c
 *
 *  Created on: Mar 26, 2026
 *      Author: Landi András
 */

#include "Encoder.h"

volatile int32_t encodercounter;
volatile uint32_t EncoderButtonLockout = 0;
volatile bool EncoderButton = false;
const uint32_t LockoutDuration = 5;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	//Encoder turned:
	if(GPIO_Pin == ENC_CHA_Pin && !(ENC_CHA_GPIO_Port->IDR & ENC_CHA_Pin)){
		if(ENC_CHB_GPIO_Port->IDR & ENC_CHB_Pin){
			encodercounter++;
		}
		else {
			encodercounter--;
		}
	}
	else if (GPIO_Pin == ENC_CHB_Pin && !(ENC_CHB_GPIO_Port->IDR & ENC_CHB_Pin)){
		if(ENC_CHA_GPIO_Port->IDR & ENC_CHA_Pin){
			encodercounter--;
		}
		else {
			encodercounter++;
		}
	}
	//Encoder Button:
	if(GPIO_Pin == ENC_BTN_Pin){
		if(HAL_GetTick()-EncoderButtonLockout > LockoutDuration){
			EncoderButton = !(ENC_BTN_GPIO_Port->IDR & ENC_BTN_Pin);
			EncoderButtonLockout = HAL_GetTick();
		}
	}
}

bool GetEncoderButtonState(){
	return EncoderButton;
}

int32_t GetEncoderCounter(){
	// Save the current interrupt state
	uint32_t primask_bit = __get_PRIMASK();

	// Disable all interrupts
	__disable_irq();

	// Reset the counter
	int32_t temp = encodercounter;
	encodercounter = 0;

	// Return to original interrupt state
	__set_PRIMASK(primask_bit);


	return temp;
}
