/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Mar 14, 2016
 * ============================================================================
 * Filename: platform.h
 * ============================================================================
 */

#ifndef PLATFORM_H
#define PLATFORM_H

#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal.h"

#include <stdio.h>
#include <string.h>

// HAL Tick Timer Initialisation using TIM5
static TIM_HandleTypeDef TIM_HALTTickInit;

#ifdef __cplusplus
extern "C" {
#endif

    HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority);
    void HAL_Delay(__IO uint32_t Delay);
    void tim5_irqhandler(void);
    void HAL_IncTick(void);
    uint32_t HAL_GetTick(void);
    void HAL_SuspendTick(void);
    void HAL_ResumeTick(void);
    
    uint32_t uwhaltick;



#ifdef __cplusplus
}
#endif

#endif /* PLATFORM_H */

