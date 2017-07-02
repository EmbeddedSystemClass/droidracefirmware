//
// Created by Adam Stacey on 29/6/17.
//

#ifndef DROID_CONTROL_HAL_H
#define DROID_CONTROL_HAL_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"
#include "stm32f4xx_it.h"

#define PWM_CLOCKFREQ		50000
#define PWM_PULSEPERIOD		(PWM_CLOCKFREQ/100)
#define PWM_PERIOD			2*PWM_CLOCKFREQ/10

#define PWM_C1_SET(value) 	__HAL_TIM_SET_COMPARE(&TIM1_Init, TIM_CHANNEL_1, value)
#define PWM_C2_SET(value) 	__HAL_TIM_SET_COMPARE(&TIM1_Init, TIM_CHANNEL_2, value)

void control_init();
void control_channel1_init();
void control_channel2_init();

extern int input_channels[3];


#endif //DROID_CONTROL_HAL_H
