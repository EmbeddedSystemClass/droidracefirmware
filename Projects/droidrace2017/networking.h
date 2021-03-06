/*
 * ============================================================================
 * Created by Timothy Ryan Hadwen
 * Created: Mar 26, 2016
 * ============================================================================
 */

#ifndef DIGISCOPE_NET_H
#define DIGISCOPE_NET_H

#ifdef __cplusplus
extern "C" {
#endif
#include "debugger_printf.h"

#include "netconfig.h"
#include "FreeRTOS.h"
#include "task.h"

#include "tcpip.h"
#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include "lwip_init.h"
#include <string.h>
#include "control_hal.h"

void networking_init();
void TCPTask(void * pvParameters);
int TCPIsConnected();

/* Defines */
#define TCP_PORT 3000
#define TCP_PACKET_SIZE 256
#define TCP_SEND_QUEUE_SIZE 5

extern uint32_t nucleoIPAddress;

extern QueueHandle_t TCPSendQueue;

/* FreeRTOS Settings */
#define TCPTASK_PRIORITY   ( tskIDLE_PRIORITY + 2 )
#define TCPTASK_STACK_SIZE  ( configMINIMAL_STACK_SIZE * 6 )

#ifdef __cplusplus
}
#endif

#endif /* DIGISCOPE_NET_H */

