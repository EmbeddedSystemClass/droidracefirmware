//
// Created by Adam Stacey on 29/6/17.
//

#ifndef DROID_CONTROL_HAL_H
#define DROID_CONTROL_HAL_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"
#include "stm32f4xx_it.h"

void control_init();

extern int control_ch1;


#endif //DROID_CONTROL_HAL_H
