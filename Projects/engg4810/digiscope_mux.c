/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: May 14, 2016
 * ============================================================================
 * Filename: digiscope_mux.c
 * ============================================================================
 */

#include "digiscope_mux.h"
#include "Core/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_gpio.h"

void digiscope_mux_init() {
    GPIO_InitTypeDef GPIO_spi;
    
    DIGISCOPE_MUX_CH1_COUP_CLK();

    /* Configure GPIO PIN for SPI Chip select */
    GPIO_spi.Pin = DIGISCOPE_MUX_CH1_COUP_PIN; //Pin
    GPIO_spi.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_spi.Pull = GPIO_PULLUP; //Enable Pull up, down or no pull resister
    GPIO_spi.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_MUX_CH1_COUP_PORT, &GPIO_spi); //Initialise Pin
    
    DIGISCOPE_MUX_CH1_FILT_CLK();

    /* Configure GPIO PIN for SPI Chip select */
    GPIO_spi.Pin = DIGISCOPE_MUX_CH1_FILT_PIN; //Pin
    GPIO_spi.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_spi.Pull = GPIO_PULLUP; //Enable Pull up, down or no pull resister
    GPIO_spi.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_MUX_CH1_FILT_PORT, &GPIO_spi); //Initialise Pin
    
    DIGISCOPE_MUX_CH2_COUP_CLK();

    /* Configure GPIO PIN for SPI Chip select */
    GPIO_spi.Pin = DIGISCOPE_MUX_CH2_COUP_PIN; //Pin
    GPIO_spi.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_spi.Pull = GPIO_PULLUP; //Enable Pull up, down or no pull resister
    GPIO_spi.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(DIGISCOPE_MUX_CH2_COUP_PORT, &GPIO_spi); //Initialise Pin
    
    digiscope_mux_coupling(1, DIGISCOPE_MUX_DC_COUPLING);
            
    digiscope_mux_coupling(2, DIGISCOPE_MUX_DC_COUPLING);
    
    digiscope_mux_filtering(DIGISCOPE_MUX_LPF);
}

void digiscope_mux_coupling(int ch, int value) {
    switch(ch) {
        case 1:
            HAL_GPIO_WritePin(DIGISCOPE_MUX_CH1_COUP_PORT, DIGISCOPE_MUX_CH1_COUP_PIN, value);
            break;
        case 2:
            HAL_GPIO_WritePin(DIGISCOPE_MUX_CH2_COUP_PORT, DIGISCOPE_MUX_CH2_COUP_PIN, value);
            break;
    }
}

void digiscope_mux_filtering(int value) {
    HAL_GPIO_WritePin(DIGISCOPE_MUX_CH1_FILT_PORT, DIGISCOPE_MUX_CH1_FILT_PIN, value);
}