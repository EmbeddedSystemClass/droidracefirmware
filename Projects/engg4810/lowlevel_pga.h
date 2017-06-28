/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Apr 3, 2016
 * ============================================================================
 * Filename: lowlevel_pga.h
 * ============================================================================
 */

#ifndef LOWLEVEL_PGA_H
#define LOWLEVEL_PGA_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"
#include "debugger_printf.h"
#include <stdint.h>
#include "netconfig.h"
#include "FreeRTOS.h"
#include "semphr.h"

#define LOWLEVEL_PGA_CH1_CS_PIN      GPIO_PIN_5
#define LOWLEVEL_PGA_CH1_CS_PORT     GPIOF
#define LOWLEVEL_PGA_CH1_CS_CLK()    __HAL_RCC_GPIOF_CLK_ENABLE()

#define LOWLEVEL_PGA_CH2_CS_PIN      GPIO_PIN_3
#define LOWLEVEL_PGA_CH2_CS_PORT     GPIOD
#define LOWLEVEL_PGA_CH2_CS_CLK()    __HAL_RCC_GPIOD_CLK_ENABLE()

#define PGA_CH1_1   0
#define PGA_CH1_2   1
#define PGA_CH2_1   2
#define PGA_CH2_2   3

#define PGA_LT_G0 0x0
#define PGA_LT_G1 0x1
#define PGA_LT_G2 0x2
#define PGA_LT_G5 0x3
#define PGA_LT_G10 0x4
#define PGA_LT_G20 0x5

    void lowlevel_pga_setgain(int ch, int firstgain, int secondgain);
    void lowlevel_pga_init();

#ifdef __cplusplus
}
#endif

#endif /* LOWLEVEL_PGA_H */

