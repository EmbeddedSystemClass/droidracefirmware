/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Mar 26, 2016
 * ============================================================================
 * Filename: digiscope_net.h
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

void digiscope_net_init();
void TCPTask(void * pvParameters);

typedef struct {
    struct sockaddr host;
    int lastkeepalive;
    int active;
} ClientConnection;

    /* Defines */
#define TCP_SOCKET_PORT 3000

    /* FreeRTOS Settings */
#define TCPTASK_PRIORITY   ( tskIDLE_PRIORITY + 2 )
#define TCPTASK_STACK_SIZE  ( configMINIMAL_STACK_SIZE * 6 )

#ifdef __cplusplus
}
#endif

#endif /* DIGISCOPE_NET_H */

