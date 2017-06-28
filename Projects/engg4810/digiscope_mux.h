/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: May 14, 2016
 * ============================================================================
 * Filename: digiscope_mux.h
 * ============================================================================
 */

#ifndef DIGISCOPE_MUX_H
#define DIGISCOPE_MUX_H

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

#define DIGISCOPE_MUX_CH1_COUP_PIN      GPIO_PIN_1
#define DIGISCOPE_MUX_CH1_COUP_PORT     GPIOD
#define DIGISCOPE_MUX_CH1_COUP_CLK()    __HAL_RCC_GPIOD_CLK_ENABLE()

#define DIGISCOPE_MUX_CH1_FILT_PIN      GPIO_PIN_0
#define DIGISCOPE_MUX_CH1_FILT_PORT     GPIOG
#define DIGISCOPE_MUX_CH1_FILT_CLK()    __HAL_RCC_GPIOG_CLK_ENABLE()

#define DIGISCOPE_MUX_CH2_COUP_PIN      GPIO_PIN_0
#define DIGISCOPE_MUX_CH2_COUP_PORT     GPIOD
#define DIGISCOPE_MUX_CH2_COUP_CLK()    __HAL_RCC_GPIOD_CLK_ENABLE()

#define DIGISCOPE_MUX_AC_COUPLING   0
#define DIGISCOPE_MUX_DC_COUPLING   1
    
#define DIGISCOPE_MUX_BPF           1
#define DIGISCOPE_MUX_LPF           0
    
    void digiscope_mux_coupling(int ch, int value);
    void digiscope_mux_filtering(int value);

#ifdef __cplusplus
}
#endif

#endif /* DIGISCOPE_MUX_H */

