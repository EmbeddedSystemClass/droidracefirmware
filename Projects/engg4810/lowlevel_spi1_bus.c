/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Apr 4, 2016
 * ============================================================================
 * Filename: lowlevel_spi1_bus.c
 * ============================================================================
 */

#include "lowlevel_spi1_bus.h"

SPI_HandleTypeDef SpiHandle;
SemaphoreHandle_t SPI1Mutex;

void lowlevel_spi1_bus_init(void) {
    GPIO_InitTypeDef GPIO_spi;

    /* Enable GPIO Pin clocks */
    LOWLEVEL_SPI1_COMMON_SPI_CLK();
    LOWLEVEL_SPI1_COMMON_SCK_CLK();
    LOWLEVEL_SPI1_COMMON_MOSI_CLK();

    /* SPI SCK pin configuration */
    GPIO_spi.Pin = LOWLEVEL_SPI1_COMMON_SCK_PIN;
    GPIO_spi.Mode = GPIO_MODE_AF_PP;
    GPIO_spi.Speed = GPIO_SPEED_HIGH;
    GPIO_spi.Pull = GPIO_PULLDOWN;
    GPIO_spi.Alternate = LOWLEVEL_SPI1_COMMON_SCK_AF;
    HAL_GPIO_Init(LOWLEVEL_SPI1_COMMON_SCK_PORT, &GPIO_spi);

    /* SPI MOSI pin configuration */
    GPIO_spi.Pin = LOWLEVEL_SPI1_COMMON_MOSI_PIN;
    GPIO_spi.Mode = GPIO_MODE_AF_PP;
    GPIO_spi.Pull = GPIO_PULLDOWN;
    GPIO_spi.Alternate = LOWLEVEL_SPI1_COMMON_MOSI_AF;
    HAL_GPIO_Init(LOWLEVEL_SPI1_COMMON_MOSI_PORT, &GPIO_spi);

    /* SPI configuration */
    SpiHandle.Instance = (SPI_TypeDef *) (LOWLEVEL_SPI1_COMMON_SPI);

    __HAL_SPI_DISABLE(&SpiHandle);

    SpiHandle.Init.Mode = SPI_MODE_MASTER;
    SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; //256;
    SpiHandle.Init.Direction = SPI_DIRECTION_2LINES;
    SpiHandle.Init.CLKPhase = SPI_PHASE_1EDGE;
    SpiHandle.Init.CLKPolarity = SPI_POLARITY_LOW;
    SpiHandle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLED;
    SpiHandle.Init.CRCPolynomial = 0; //7;
    SpiHandle.Init.DataSize = SPI_DATASIZE_8BIT;
    SpiHandle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.NSS = SPI_NSS_SOFT;
    SpiHandle.Init.TIMode = SPI_TIMODE_DISABLED;

    HAL_SPI_Init(&SpiHandle);

    __HAL_SPI_ENABLE(&SpiHandle);

    SPI1Mutex = xSemaphoreCreateMutex();

}