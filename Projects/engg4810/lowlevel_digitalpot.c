/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Apr 2, 2016
 * ============================================================================
 * Filename: lowlevel_digitalpot.c
 * ============================================================================
 * MAX5482 50k Single Digital Pot
 */
#include "lowlevel_digitalpot.h"

extern SemaphoreHandle_t SPI1Mutex;
extern SPI_HandleTypeDef SpiHandle;

void lowlevel_digitalpot_init() {
    GPIO_InitTypeDef GPIO_spi;

    LOWLEVEL_DIGITALPOT_CS_CLK();

    /* Configure GPIO PIN for SPI Chip select, TFT CS, TFT DC */
    GPIO_spi.Pin = LOWLEVEL_DIGITALPOT_CS_PIN; //Pin
    GPIO_spi.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_spi.Pull = GPIO_PULLUP; //Enable Pull up, down or no pull resister
    GPIO_spi.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(LOWLEVEL_DIGITALPOT_CS_PORT, &GPIO_spi); //Initialise Pin

    /* Configure GPIO PIN for  Chip select */

    /* Set chip select high */
    HAL_GPIO_WritePin(LOWLEVEL_DIGITALPOT_CS_PORT, LOWLEVEL_DIGITALPOT_CS_PIN, 1);
}

void lowlevel_digitalpot_setohm(int ohms) {
    if (ohms < 0 || ohms > 50000) {
        return;
    }

    printf("Setting resistance %d\n", ohms);

    uint16_t value = (int) (ohms * (4095.0 / 50000.0));

    printf("Value %d\n", value);

    if (xSemaphoreTake(SPI1Mutex, (TickType_t) 10) == pdTRUE) {
        printf("Sending: 0x00, 0x%02X, 0x%02X\n", ((value << 4) & 0xFF00) >> 8, (value << 4) & 0xFF);

        LOWLEVEL_DIGITALPOT_CS_LOW();

        uint8_t send[3] = {0x00, ((value << 4) & 0xFF00) >> 8, (value << 4) & 0xFF};

        HAL_SPI_Transmit(&SpiHandle, send, 3, 100);

        vTaskDelay(1);

        LOWLEVEL_DIGITALPOT_CS_HIGH();

        xSemaphoreGive(SPI1Mutex);
    } else {
        printf("Could not get SPIBus semph\n");
    }
}
