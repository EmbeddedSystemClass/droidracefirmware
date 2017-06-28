/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Apr 3, 2016
 * ============================================================================
 * Filename: lowlevel_pga.c
 * ============================================================================
 */

#include "lowlevel_pga.h"

extern SemaphoreHandle_t SPI1Mutex;
extern SPI_HandleTypeDef SpiHandle;

/* ==== Static Functions ==== */
static void lowlevel_pga_cs(int ch, int value) {
    switch (ch) {
        case 1:
            HAL_GPIO_WritePin(LOWLEVEL_PGA_CH1_CS_PORT, LOWLEVEL_PGA_CH1_CS_PIN, value);
            break;
        case 2:
            HAL_GPIO_WritePin(LOWLEVEL_PGA_CH2_CS_PORT, LOWLEVEL_PGA_CH2_CS_PIN, value);
            break;
    }
}

/* ==== Public Functions ==== */
void lowlevel_pga_init() {
    GPIO_InitTypeDef GPIO_spi;

    LOWLEVEL_PGA_CH1_CS_CLK();
    LOWLEVEL_PGA_CH2_CS_CLK();

    GPIO_spi.Pin = LOWLEVEL_PGA_CH1_CS_PIN; //Pin
    GPIO_spi.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_spi.Pull = GPIO_PULLUP; //Enable Pull up, down or no pull resister
    GPIO_spi.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(LOWLEVEL_PGA_CH1_CS_PORT, &GPIO_spi); //Initialise Pin

    GPIO_spi.Pin = LOWLEVEL_PGA_CH2_CS_PIN; //Pin
    GPIO_spi.Mode = GPIO_MODE_OUTPUT_PP; //Output Mode
    GPIO_spi.Pull = GPIO_PULLUP; //Enable Pull up, down or no pull resister
    GPIO_spi.Speed = GPIO_SPEED_FAST; //Pin latency
    HAL_GPIO_Init(LOWLEVEL_PGA_CH2_CS_PORT, &GPIO_spi); //Initialise Pin

    /* Configure GPIO PIN for  Chip select */

    /* Set chip select high */
    lowlevel_pga_cs(1, 1);
    lowlevel_pga_cs(2, 1);
}

void lowlevel_pga_setgain(int ch, int firstgain, int secondgain) {

#ifdef DEBUG
    printf("PGA: %d %d\n", firstgain, secondgain);
#endif /* DEBUG */

    if (xSemaphoreTake(SPI1Mutex, (TickType_t) 10) == pdTRUE) {
        lowlevel_pga_cs(ch, 0);

        // Write to the SPI Bus
        uint8_t send = firstgain << 4 | secondgain;

        HAL_SPI_Transmit(&SpiHandle, &send, 1, 100);

        vTaskDelay(1);

        lowlevel_pga_cs(ch, 1);

        xSemaphoreGive(SPI1Mutex);
    } else {
        printf("Could not get SPI Bus semph\n");
    }
}