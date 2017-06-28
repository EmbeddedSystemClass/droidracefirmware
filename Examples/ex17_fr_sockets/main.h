/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Mar 14, 2016
 * ============================================================================
 * Filename: main.h
 * ============================================================================
 */

#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif
    
    /* Function Prototypes */
    static void Hardware_init();
    void TCPTask(void * pvParameters);
    void UDPTask(void * pvParameters);
    void LEDTask(void*);
    void vApplicationIdleHook(void);

    /* Includes */
    #include <string.h>
    #include "stm32f4xx_nucleo_144.h"

    #include "netconfig.h"
    #include "FreeRTOS.h"
    #include "task.h"

    #include "tcpip.h"
    #include "lwip/opt.h"
    #include "lwip/api.h"
    #include "lwip/sys.h"
    #include "lwip/sockets.h"
    #include "lwip_init.h"

    #include "debugger_printf.h"

    /* Defines */
    #define TCP_SOCKET_PORT 10
    #define UDP_SOCKET_PORT 11

    /* FreeRTOS Settings */
    #define TCPTASK_PRIORITY   ( tskIDLE_PRIORITY + 2 )
    #define UDPTASK_PRIORITY    ( tskIDLE_PRIORITY + 1 )

    #define TCPTASK_STACK_SIZE  ( configMINIMAL_STACK_SIZE * 6 )
    #define UDPTASK_STACK_SIZE  ( configMINIMAL_STACK_SIZE * 6 )



#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */

