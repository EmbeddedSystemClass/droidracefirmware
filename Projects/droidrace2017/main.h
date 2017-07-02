/*
 * ============================================================================
 * Created by Timothy Ryan Hadwen
 * Created: Mar 14, 2016
 * ============================================================================
 */

#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

    /* Function Prototypes */
    static void Hardware_init();
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

#include "droid_cli.h"
#include "networking.h"
#include "control_hal.h"



    /* FreeRTOS Settings */
#define LEDTASK_PRIORITY   ( tskIDLE_PRIORITY + 2 )
#define LEDTASK_STACK_SIZE  ( configMINIMAL_STACK_SIZE * 1 )

int input_channels[3] = {75, 75, 75};


#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */

