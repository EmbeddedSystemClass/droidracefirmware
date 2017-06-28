/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Apr 2, 2016
 * ============================================================================
 * Filename: lowlevel_digitalpot.h
 * ============================================================================
 */

#ifndef LOWLEVEL_DIGITALPOT_H
#define LOWLEVEL_DIGITALPOT_H

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

    void lowlevel_digitalpot_init(void);
    void lowlevel_digitalpot_setohm(int);

#define LOWLEVEL_DIGITALPOT_CS_PIN      GPIO_PIN_7
#define LOWLEVEL_DIGITALPOT_CS_PORT     GPIOC
#define LOWLEVEL_DIGITALPOT_CS_CLK()    __HAL_RCC_GPIOC_CLK_ENABLE()
#define LOWLEVEL_DIGITALPOT_CS_LOW()    HAL_GPIO_WritePin(LOWLEVEL_DIGITALPOT_CS_PORT, LOWLEVEL_DIGITALPOT_CS_PIN, 0)
#define LOWLEVEL_DIGITALPOT_CS_HIGH()    HAL_GPIO_WritePin(LOWLEVEL_DIGITALPOT_CS_PORT, LOWLEVEL_DIGITALPOT_CS_PIN, 1)



#ifdef __cplusplus
}
#endif

#endif /* LOWLEVEL_DIGITALPOT_H */

