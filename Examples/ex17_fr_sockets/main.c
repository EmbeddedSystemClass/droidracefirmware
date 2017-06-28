/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Mar 14, 2016
 * ============================================================================
 * Filename: main.c
 * ============================================================================
 */


/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

extern struct netif xnetif;

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
int main(void) {

    Hardware_init();

    /* Init the LwIP stack */
    LwIP_Init();

    /* Create 3 Tasks */
    xTaskCreate((void *) &UDPTask, (const signed char *) "UDP", UDPTASK_STACK_SIZE, NULL, UDPTASK_PRIORITY, NULL);
    xTaskCreate((void *) &TCPTask, (const signed char *) "TCP", TCPTASK_STACK_SIZE, NULL, TCPTASK_PRIORITY, NULL);
    xTaskCreate((void *) &LEDTask, (const signed char *) "LED", TCPTASK_STACK_SIZE, NULL, TCPTASK_PRIORITY, NULL);
    
    /* Start scheduler */
    vTaskStartScheduler();

    return 0;
}

/**
 * Keep alive LED Task.
 * @param pvParam
 */
void LEDTask(void* pvParam) {
    for (;;) {
        BSP_LED_Toggle(LED2);
        printf("IP=%d.%d.%d.%d\n", 0xff & xnetif.ip_addr.addr, 0xff&(xnetif.ip_addr.addr>>8), 0xff&(xnetif.ip_addr.addr>>16), 0xff&(xnetif.ip_addr.addr>>24));
        vTaskDelay(1000);
    }
}

/**
 * @brief  TCP Task. Bind to a TCP Socket. Reflect back captilised version of received string.
 * @param  None
 * @retval None
 */
void TCPTask(void * pvParameters) {
    BSP_LED_On(LED1);

    int sock = 0;
    int tcpconn = 0;
    long size = 0;
    int ret = 0;
    int i;
    unsigned char recv_buffer[20];

    struct sockaddr_in address, remotehost;

    /* create a TCP socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        // 	debug_printf("can not create socket");
        // VCP_txflush();
        BSP_LED_Toggle(LED1);
        return;
    }

    /* bind to port 80 at any interface */
    address.sin_family = AF_INET;
    address.sin_port = htons(TCP_SOCKET_PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (const struct sockaddr *) (&address), sizeof (address)) < 0) {
        // 	debug_printf("can not bind socket");
        // VCP_txflush();
        BSP_LED_Toggle(LED1);
        return;
    }

    /* listen for incoming connections (TCP listen backlog = 5) */
    listen(sock, 5);

    size = sizeof (remotehost);

    memset(recv_buffer, 0, sizeof (recv_buffer));

    for (;;) {
        BSP_LED_Toggle(LED1);
        /* Wait for connection */
        tcpconn = accept(sock, (struct sockaddr *) (&remotehost), (socklen_t *) & size);


        /* Read from Socket */
        while ((ret = read(tcpconn, recv_buffer, sizeof (recv_buffer))) > 0) {

            // debug_printf("TCP Socket: ");
            for (i = 0; i < ret; i++) {

                /* Convert to ASCII upper case */
                if (recv_buffer[i] > 96) {
                    recv_buffer[i] -= 32;
                }

                // debug_putc(recv_buffer[i]);		//put character into USB buffer
            }

            /* Put new line and carrriage return characters into the USB buffer */
            // debug_putc('\n');
            // debug_putc('\r');
            vTaskDelay(5);
            // debug_flush();						//flush USB buffer to transmit characters

            /* Write buffer back to socket */
            write(tcpconn, recv_buffer, ret);
        }

        vTaskDelay(100);
    }
}

/**
 * @brief  UDP Task. Listen to a UDP Socket. Reflect back captilised version of received string.
 * @param  None
 * @retval None
 */
void UDPTask(void * pvParameters) {

    int udpsock = 0;
    int ret = 0;
    int len = 0;
    int i;
    unsigned char recv_buffer[20];
    struct sockaddr_in address;
    struct sockaddr remotehost;

    /* create a UDP socket */
    if ((udpsock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        //debug_printf("can not create UDP socket");
        return;
    }

    /* bind to UDP port at any interface */
    address.sin_family = AF_INET;
    address.sin_port = htons(UDP_SOCKET_PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(udpsock, (const struct sockaddr *) (&address), sizeof (address)) < 0) {
        // debug_printf("can not bind UPD socket");
        return;
    }

    len = (uint32_t) sizeof (remotehost);

    for (;;) {

        /* Read from Socket */
        while ((ret = recvfrom(udpsock, recv_buffer, sizeof (recv_buffer), 0, (struct sockaddr *) (&remotehost), (uint32_t *) & len)) > 0) {

            // debug_printf("UDP Socket: ");
            for (i = 0; i < ret; i++) {

                /* Convert to ASCII upper case */
                if (recv_buffer[i] > 96) {
                    recv_buffer[i] -= 32;
                }
                // debug_putc(recv_buffer[i]);
            }
            // debug_putc('\n');
            // debug_putc('\r');
            vTaskDelay(5);
            // debug_flush();

            /* Write back to socket */
            sendto(udpsock, recv_buffer, ret, 0, (struct sockaddr *) (&remotehost), sizeof (remotehost));
        }

        vTaskDelay(100);
    }
}

/**
 * @brief  Hardware Initialisation.
 * @param  None
 * @retval None
 */
void Hardware_init(void) {
    portDISABLE_INTERRUPTS();
    
    /* Enable USER Led */
    BSP_LED_Init(LED1);
    BSP_LED_Init(LED2);
    BSP_LED_Init(LED3);
    
    /* Enable HAL */
    if (HAL_Init() != HAL_OK) {
        /* If error turn on Red LED*/
        BSP_LED_On(LED3);
    }
    
    debugger_printf_init();
    
    portENABLE_INTERRUPTS();
}

/**
 * @brief  Idle Application Task (Disabled)
 * @param  None
 * @retval None
 */
void vApplicationIdleHook(void) {
    static TickType_t xLastTx = 0;

    // BRD_LEDOff();
    /* The idle hook simply prints the idle tick count */
    if ((xTaskGetTickCount() - xLastTx) > (1000 / portTICK_RATE_MS)) {
        xLastTx = xTaskGetTickCount();
        // debug_printf("IDLE Tick %d\n", xLastTx);
    }
}

/**
 * @brief  vApplicationStackOverflowHook
 * @param  Task Handler and Task Name
 * @retval None
 */
void vApplicationStackOverflowHook(xTaskHandle pxTask, signed char *pcTaskName) {
    /* This function will get called if a task overflows its stack.   If the
    parameters are corrupt then inspect pxCurrentTCB to find which was the
    offending task. */

    // BRD_LEDOff();
    (void) pxTask;
    (void) pcTaskName;

    for (;;);
}
