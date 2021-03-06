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
    void LEDTask(void*);
    void vApplicationIdleHook(void);
    static void SystemClock_Config(void);

    /* Includes */
#include <string.h>
#include "stm32f4xx_nucleo_144.h"
#include "stm32f4xx_it.h"

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

#include "digiscope_signalgen.h"
#include "digiscope_cli.h"
#include "digiscope_net.h"
#include "digiscope_adc.h"
#include "lowlevel_digitalpot.h"
    #include "lowlevel_pga.h"
#include "digiscope_vertical.h"
#include "digiscope_voltagedetect.h"



    /* FreeRTOS Settings */
#define LEDTASK_PRIORITY   ( tskIDLE_PRIORITY + 2 )
#define LEDTASK_STACK_SIZE  ( configMINIMAL_STACK_SIZE * 1 )



#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */

