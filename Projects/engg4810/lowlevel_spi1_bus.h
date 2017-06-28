/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Apr 4, 2016
 * ============================================================================
 * Filename: lowlevel_spi1_bus.h
 * ============================================================================
 */

#ifndef LOWLEVEL_SPI1_BUS_H
#define LOWLEVEL_SPI1_BUS_H

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

    void lowlevel_spi1_bus_init(void);

#define LOWLEVEL_SPI1_COMMON_SPI         SPI1
#define LOWLEVEL_SPI1_COMMON_SPI_CLK()   __HAL_RCC_SPI1_CLK_ENABLE()

#define LOWLEVEL_SPI1_COMMON_SCK_PIN     GPIO_PIN_3
#define LOWLEVEL_SPI1_COMMON_SCK_PORT    GPIOB
#define LOWLEVEL_SPI1_COMMON_SCK_CLK()   __HAL_RCC_GPIOB_CLK_ENABLE()
#define LOWLEVEL_SPI1_COMMON_SCK_AF      GPIO_AF5_SPI1

#define LOWLEVEL_SPI1_COMMON_MOSI_PIN    GPIO_PIN_5
#define LOWLEVEL_SPI1_COMMON_MOSI_PORT   GPIOB
#define LOWLEVEL_SPI1_COMMON_MOSI_CLK()  __HAL_RCC_GPIOB_CLK_ENABLE()
#define LOWLEVEL_SPI1_COMMON_MOSI_AF     GPIO_AF5_SPI1

#ifdef __cplusplus
}
#endif

#endif /* LOWLEVEL_SPI1_BUS_H */

