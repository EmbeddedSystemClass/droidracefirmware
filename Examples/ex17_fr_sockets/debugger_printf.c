/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Mar 14, 2016
 * ============================================================================
 * Filename: debugger_printf.c
 * ============================================================================
 */

#include "debugger_printf.h"

UART_HandleTypeDef UART_Debugger;

/**
 * Initalize the debug printf uart
 */
void debugger_printf_init() {
#ifdef ENABLE_DEBUGGER_UART
    GPIO_InitTypeDef GPIO_serialtx;

    //Enable DEBUG UART clock
    DEBUGGER_PRINTF_UART_CLK();

    UART_Debugger.Instance = (USART_TypeDef *) (DEBUGGER_PRINTF_UART);
    UART_Debugger.Init.BaudRate = 9600;
    UART_Debugger.Init.WordLength = UART_WORDLENGTH_8B;
    UART_Debugger.Init.StopBits = UART_STOPBITS_1;
    UART_Debugger.Init.Parity = UART_PARITY_NONE;
    UART_Debugger.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UART_Debugger.Init.Mode = UART_MODE_TX;


    DEBUGGER_PRINTF_TX_CLK();
    /* Configure the GPIO TX pin for DEBUG UART */
    GPIO_serialtx.Pin = DEBUGGER_PRINTF_TX_PIN;
    GPIO_serialtx.Mode = GPIO_MODE_AF_PP;
    GPIO_serialtx.Pull = GPIO_PULLUP;
    GPIO_serialtx.Speed = GPIO_SPEED_FAST;
    GPIO_serialtx.Alternate = DEBUGGER_PRINTF_TX_AF;
    HAL_GPIO_Init(DEBUGGER_PRINTF_TX_PORT, &GPIO_serialtx);
    
    HAL_UART_Init(&UART_Debugger);
    
    init_printf(NULL, debugger_printf_putc_tiny);
#endif
}

void debugger_printf_putc(char x) {
    #ifdef ENABLE_DEBUGGER_UART
        int status = HAL_UART_Transmit(&UART_Debugger, &x, 1,  0);
        printf_delay(10000); // Wait 1000 clock cycles
    #endif
}

void debugger_printf_putc_tiny(void* UNUSED, char x) {
    debugger_printf_putc(x);
}

static void printf_delay(int x){
    while(x--){
        asm("nop");
    }
}