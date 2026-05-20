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
extern uint8_t mute_flag;
volatile uint32_t EncoderLockout = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){

	//Encoder turned:
	if(GPIO_Pin == ENC_CHA_Pin || GPIO_Pin == ENC_CHB_Pin) {

	        static uint8_t prev_state = 0;
	        uint8_t curr_state = 0;											//lowest bit is channel B state, second bit is channel A

	        if (ENC_CHA_GPIO_Port->IDR & ENC_CHA_Pin) curr_state |= 0x02;
	        if (ENC_CHB_GPIO_Port->IDR & ENC_CHB_Pin) curr_state |= 0x01;

	        uint8_t state_index = (prev_state << 2) | curr_state;			//combine previous state and current state into a 4 bit state

	        //lookup table for the 16 possible combinations
	        // 0 = invalid/bounce, 1 = CW (Up), -1 = CCW (Down)
	        const int8_t encoder_states[16] = {
	             0, -1,  1,  0,
	             1,  0,  0, -1,
	            -1,  0,  0,  1,
	             0,  1, -1,  0
	        };

	        encodercounter += encoder_states[state_index];
	        prev_state = curr_state;
	    }

	//Encoder Button:
	if(GPIO_Pin == ENC_BTN_Pin){
		if(HAL_GetTick()-EncoderButtonLockout > LockoutDuration){
			EncoderButton = !(ENC_BTN_GPIO_Port->IDR & ENC_BTN_Pin);
			if(EncoderButton) mute_flag = true;
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
