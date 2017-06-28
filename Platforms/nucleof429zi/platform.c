/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Mar 14, 2016
 * ============================================================================
 * Filename: platform.c
 * ============================================================================
 */

#include "platform.h"


HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority) {

    unsigned short PrescalerValue;

    // Timer 5 clock enable
    __TIM5_CLK_ENABLE();

    // Compute the prescaler value
    PrescalerValue = (uint16_t) ((SystemCoreClock / 2) / 1000000) - 1; //Set clock prescaler to 1kHz - SystemCoreClock is the system clock frequency. CANNOT GO BELOW 1.2kHZ

    // Time base configuration
    TIM_HALTTickInit.Instance = TIM5; //Enable Timer 5
    TIM_HALTTickInit.Init.Period = 0xFFFFFFFF; //Set period to be 1s
    TIM_HALTTickInit.Init.Prescaler = PrescalerValue; //Set presale value
    TIM_HALTTickInit.Init.ClockDivision = 0; //Set clock division
    TIM_HALTTickInit.Init.RepetitionCounter = 0; // Set Reload Value
    TIM_HALTTickInit.Init.CounterMode = TIM_COUNTERMODE_UP; //Set timer to count up.

    // Initialise Timer
    HAL_TIM_Base_Init(&TIM_HALTTickInit);

    // Set priority of Timer 2 update Interrupt [0 (HIGH priority) to 15(LOW priority)]
    //HAL_NVIC_SetPriority(TIM5_IRQn, TickPriority, 0);		//Set Main priority ot 10 and sub-priority ot 0.

    // Enable timer update interrupt and interrupt vector for Timer
    //NVIC_SetVector(TIM5_IRQn, (uint32_t)&tim5_irqhandler);
    //NVIC_EnableIRQ(TIM5_IRQn);

    // Start Timer
    HAL_TIM_Base_Start(&TIM_HALTTickInit);

    //Configure the SysTick IRQ priority
    //HAL_NVIC_SetPriority(SysTick_IRQn, TickPriority ,0);

    // Return function status
    return HAL_OK;

}//*/

//Disable the Tick Interrupt

void HAL_SuspendTick(void) {
    // Disable SysTick Interrupt
    //  SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

//Enable the Tick Interrupt

void HAL_ResumeTick(void) {
    // Enable SysTick Interrupt
    //SysTick->CTRL  |= SysTick_CTRL_TICKINT_Msk;
}


//Delay for period (in ms)

void HAL_Delay(__IO uint32_t Delay) {
    uint32_t tickstart = 0;
    tickstart = HAL_GetTick();
    while ((HAL_GetTick() - tickstart) < (Delay * 1000)) {
    }
}

//Delay for period (in us)

void HAL_Delayus(__IO uint32_t Delay) {
    uint32_t tickstart = 0;
    tickstart = HAL_GetTick();
    while ((HAL_GetTick() - tickstart) < Delay) {
    }
}

/**
 * @brief  Delay Function.
 * @param  nCount:specifies the Delay time length.
 * @retval None
 */
void BRD_Delay(__IO unsigned long nCount) {
    while (nCount--) {
    }
}

//TIM5 Interrupt for HAL Delay

void tim5_irqhandler(void) {
    //Clear Update Flag
    __HAL_TIM_CLEAR_IT(&TIM_HALTTickInit, TIM_IT_UPDATE);
    HAL_IncTick();
}

// Increment the HAL Tick
void HAL_IncTick(void) {
  uwhaltick++;
}

uint32_t HAL_GetTick(void) {
  return (uint32_t) __HAL_TIM_GET_COUNTER(&TIM_HALTTickInit);
}