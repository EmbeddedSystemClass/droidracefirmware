/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Mar 27, 2016
 * ============================================================================
 * Filename: digiscope_adc.h
 * ============================================================================
 */

#ifndef DIGISCOPE_ADC_H
#define DIGISCOPE_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"
#include "stm32f4xx_nucleo_144.h"
#include "debugger_printf.h"
#include <stdint.h>
#include <string.h>
#include "netconfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "digiscope_settings.h"

    void ADC_DMAInit(ADC_HandleTypeDef *hadc);
    void digiscope_adc_init_ch1(void);
    void digiscope_adc_init_ch2(void);
    void digiscope_adc_trigger_init();
    void digiscope_adc_samplingrate(int rate);
    void digiscope_adc_deinit(void);
    void digiscope_adc_samplingmode(int);
    void ADC_Trigger_HalfConv_8B(ADC_HandleTypeDef* AdcHandle);
    void ADC_Trigger_FullConv_8B(ADC_HandleTypeDef* AdcHandle);
    void ADC_Trigger_HalfConv_12B(ADC_HandleTypeDef* AdcHandle);
    void ADC_Trigger_FullConv_12B(ADC_HandleTypeDef* AdcHandle);
    void ADC_Unpause(void);

#define ADC_CH1_GPIO_PIN    GPIO_PIN_0
#define ADC_CH1_GPIO_PORT   GPIOC
#define ADC_CH1_GPIO_CLK()  __HAL_RCC_GPIOC_CLK_ENABLE()

#define ADC_CH1_ADC         ADC1
#define ADC_CH1_CHANNEL     ADC_CHANNEL_10

#define ADC_CH2_GPIO_PIN    GPIO_PIN_3
#define ADC_CH2_GPIO_PORT   GPIOC
#define ADC_CH2_GPIO_CLK()  __HAL_RCC_GPIOC_CLK_ENABLE()

#define ADC_CH2_ADC         ADC2
#define ADC_CH2_CHANNEL     ADC_CHANNEL_13

    /* Output trigger pins */
#define ADC_CH1_GPIO_TRIGGER_PIN    GPIO_PIN_0
#define ADC_CH1_GPIO_TRIGGER_PORT   GPIOG
#define ADC_CH1_GPIO_TRIGGER_CLK()  __HAL_RCC_GPIOG_CLK_ENABLE()

#define ADC_CH2_GPIO_TRIGGER_PIN    GPIO_PIN_1
#define ADC_CH2_GPIO_TRIGGER_PORT   GPIOG
#define ADC_CH2_GPIO_TRIGGER_CLK()  __HAL_RCC_GPIOG_CLK_ENABLE()

#ifdef __cplusplus
}
#endif

#endif /* DIGISCOPE_ADC_H */

