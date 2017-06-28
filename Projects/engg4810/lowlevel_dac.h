/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Apr 24, 2016
 * ============================================================================
 * Filename: lowlevel_mcp48fcb12.h
 * ============================================================================
 */

#ifndef LOWLEVEL_DAC_H
#define LOWLEVEL_DAC_H

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

#define LOWLEVEL_DAC_WRITE0_CMD 0b00000000
#define LOWLEVEL_DAC_WRITE1_CMD 0b00001000

    void lowlevel_dac_setvalue(int id, int value);
    static void lowlevel_dac_config(int id);

#define LOWLEVEL_DAC_CH1_ID_1 1
#define LOWLEVEL_DAC_CH1_ID_2 2

#define LOWLEVEL_DAC_CH2_ID_1 3
#define LOWLEVEL_DAC_CH2_ID_2 4

#define LOWLEVEL_DAC_FUNC_ID 5
#define LOWLEVEL_DAC_BACKLIGHT_ID 6

#define LOWLEVEL_DAC_CH1_CS_PIN      GPIO_PIN_1
#define LOWLEVEL_DAC_CH1_CS_PORT     GPIOG
#define LOWLEVEL_DAC_CH1_CS_CLK()    __HAL_RCC_GPIOG_CLK_ENABLE()

#define LOWLEVEL_DAC_CH2_CS_PIN      GPIO_PIN_2
#define LOWLEVEL_DAC_CH2_CS_PORT     GPIOE
#define LOWLEVEL_DAC_CH2_CS_CLK()    __HAL_RCC_GPIOE_CLK_ENABLE()
    
#define LOWLEVEL_DAC_CH3_CS_PIN      GPIO_PIN_13
#define LOWLEVEL_DAC_CH3_CS_PORT     GPIOF
#define LOWLEVEL_DAC_CH3_CS_CLK()    __HAL_RCC_GPIOF_CLK_ENABLE()

#ifdef __cplusplus
}
#endif

#endif /* LOWLEVEL_DAC_H */

