/*
 * ============================================================================
 * Created by Timothy Ryan Hadwen
 * Created: Mar 25, 2016
 * ============================================================================
 */

#ifndef DIGISCOPE_CLI_H
#define DIGISCOPE_CLI_H

#include <string.h>
#include "netconfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "tcpip.h"
#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include "lwip_init.h"

#include "task.h"
#include "queue.h"

#include "FreeRTOS_CLI.h"
#include "debugger_printf.h"

#ifdef __cplusplus
extern "C" {
#endif

#define mainCLI_TASK_STACK_SIZE  ( configMINIMAL_STACK_SIZE * 3 )
#define mainCLI_PRIORITY     ( tskIDLE_PRIORITY + 1)

void droid_cli_init();
static BaseType_t prvEchoCommand(char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString);

void CLI_Task(void);



#ifdef __cplusplus
}
#endif

#endif /* DIGISCOPE_CLI_H */

