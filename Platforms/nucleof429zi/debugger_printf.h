/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Mar 14, 2016
 * ============================================================================
 * Filename: debugger_printf.h
 * ============================================================================
 */

#ifndef DEBUGGER_PRINTF_H
#define DEBUGGER_PRINTF_H

#ifdef __cplusplus
extern "C" {
#endif
    
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal.h"
#include "tinyprintf.h"

#define DEBUGGER_PRINTF_UART        USART3
#define DEBUGGER_PRINTF_UART_CLK()    __USART3_CLK_ENABLE()

#define DEBUGGER_PRINTF_TX_PIN      GPIO_PIN_8
#define DEBUGGER_PRINTF_TX_PORT     GPIOD
#define DEBUGGER_PRINTF_TX_CLK()      __GPIOD_CLK_ENABLE()
#define DEBUGGER_PRINTF_TX_AF       GPIO_AF7_USART3

#define DEBUGGER_PRINTF_RX_PIN      GPIO_PIN_9
#define DEBUGGER_PRINTF_RX_PORT     GPIOD
#define DEBUGGER_PRINTF_RX_CLK      __GPIOD_CLK_ENABLE
#define DEBUGGER_PRINTF_RX_AF       GPIO_AF7_USART3

void debugger_printf_init(void);
void debugger_printf_putc(char x);
void debugger_printf_putc_tiny(void *, char x);
static void printf_delay(int x);
#ifdef __cplusplus
}
#endif

#endif /* DEBUGGER_PRINTF_H */

