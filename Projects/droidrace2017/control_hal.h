//
// Created by Adam Stacey on 29/6/17.
//

#ifndef DROID_CONTROL_HAL_H
#define DROID_CONTROL_HAL_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"
#include "stm32f4xx_it.h"

void control_init();
void control_channel1_init();
void control_channel2_init();

extern int input_channels[3];


#endif //DROID_CONTROL_HAL_H
