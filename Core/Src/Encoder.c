/*
 * Encoder.c
 *
 *  Created on: Mar 26, 2026
 *      Author: Landi András
 */

#include "Encoder.h"

volatile int32_t encodercounter;
volatile uint32_t EncoderLockout = 0;
volatile uint32_t EncoderButtonLockout = 0;
volatile bool EncoderButton = false;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	//Encoder turned:

	if(GPIO_Pin == ENC_CHA_Pin || GPIO_Pin == ENC_CHB_Pin){
	//if(HAL_GetTick()- EncoderLockout < 5) return;
	if(GPIO_Pin == ENC_CHA_Pin && HAL_GPIO_ReadPin(ENC_CHA_GPIO_Port, ENC_CHA_Pin) == GPIO_PIN_RESET){
		if(HAL_GPIO_ReadPin(ENC_CHB_GPIO_Port, ENC_CHB_Pin) == GPIO_PIN_SET){
			encodercounter++;
		}
		else {
			encodercounter--;
		}
	}
	else if (GPIO_Pin == ENC_CHB_Pin && HAL_GPIO_ReadPin(ENC_CHB_GPIO_Port, ENC_CHB_Pin) == GPIO_PIN_RESET){
		if(HAL_GPIO_ReadPin(ENC_CHA_GPIO_Port, ENC_CHA_Pin) == GPIO_PIN_SET){
			encodercounter--;
		}
		else {
			encodercounter++;
		}
	}
	//EncoderLockout = HAL_GetTick();
	}

	//Encoder Button:

	if(GPIO_Pin == ENC_BTN_Pin){
		if(HAL_GetTick()-EncoderButtonLockout > 5){
			EncoderButton = !HAL_GPIO_ReadPin(ENC_BTN_GPIO_Port, ENC_BTN_Pin);
			EncoderButtonLockout = HAL_GetTick();
		}
	}
}

bool GetEncoderButtonState(){
	return EncoderButton;
}

int32_t GetEncoderCounter(){
	__disable_irq();
	int32_t temp = encodercounter;
	encodercounter = 0;
	__enable_irq();
	return temp;
}
