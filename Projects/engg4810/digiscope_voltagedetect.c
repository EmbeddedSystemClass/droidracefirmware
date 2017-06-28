/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: May 25, 2016
 * ============================================================================
 * Filename: digiscope_voltagedetect.c
 * ============================================================================
 */

#include "digiscope_voltagedetect.h"

void voltagedetectirqn() {
    HAL_GPIO_EXTI_IRQHandler(EXTI9_5_IRQn);
    printf("VOLTAGE ERROR\n");
}

void digiscope_voltagedetect_init() {
    GPIO_InitTypeDef GPIO_InitStructure;

    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* Set priority of external GPIO Interrupt [0 (HIGH priority) to 15(LOW priority)] */
    /* 	DO NOT SET INTERRUPT PRIORITY HIGHER THAN 3 */
    HAL_NVIC_SetPriority(EXTI9_5_IRQn, 10, 0); //Set Main priority ot 10 and sub-priority ot 0

    //Enable external GPIO interrupt and interrupt vector for pin DO
    NVIC_SetVector(EXTI9_5_IRQn, (uint32_t) &voltagedetectirqn);
    NVIC_EnableIRQ(EXTI9_5_IRQn);

    /* Configure the D0 pin as an output */
    GPIO_InitStructure.Pin = DIGISCOPE_VD_CH1_1_PIN; //Pin
    GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING; //Output Mode
    GPIO_InitStructure.Pull = GPIO_PULLUP; //Enable Pull up, down or no pull resister
    GPIO_InitStructure.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_VD_CH1_1_PORT, &GPIO_InitStructure); //Initialise Pin

}