/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: May 25, 2016
 * ============================================================================
 * Filename: digiscope_voltagedetect.h
 * ============================================================================
 */

#ifndef DIGISCOPE_VOLTAGEDETECT_H
#define DIGISCOPE_VOLTAGEDETECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal_conf.h"
#include "debugger_printf.h"

#define DIGISCOPE_VD_CH1_1_PIN GPIO_PIN_9
#define DIGISCOPE_VD_CH1_1_PORT GPIOC

#define DIGISCOPE_VD_CH1_2_PIN  GPIO_PIN_14
#define DIGISCOPE_VD_CH1_2_PORT GPIOG

#define DIGISCOPE_VD_CH2_1_PIN
#define DIGISCOPE_VD_CH2_1_PORT

#define DIGISCOPE_VD_CH2_2_PIN
#define DIGISCOPE_VD_CH2_2_PORT

    void digiscope_voltagedetect_detect();




#ifdef __cplusplus
}
#endif

#endif /* DIGISCOPE_VOLTAGEDETECT_H */

