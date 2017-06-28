/*
 * ============================================================================
 * ENGG4810 Team 3
 * Created by Timothy Ryan Hadwen
 * Created: Mar 26, 2016
 * ============================================================================
 * Filename: digiscope_net.c
 * ============================================================================
 */

#include "digiscope_net.h"

extern uint8_t DMABlockCH1[];

extern SemaphoreHandle_t NetSendTrigger;
extern uint8_t* channel1_beforetrigger;
extern uint8_t* channel1_aftertrigger;
extern uint8_t* channel2_beforetrigger;
extern uint8_t* channel2_aftertrigger;

extern int channel1_beforetrigger_counter;
extern int channel2_beforetrigger_counter;

ClientConnection connection;
int udpsock = 0;

void digiscope_net_init() {
    // Start LwIP
    LwIP_Init();

    // Create TCP Task
    xTaskCreate((void *) &TCPTask, (const signed char *) "TCPTX", TCPTASK_STACK_SIZE, NULL, TCPTASK_PRIORITY, NULL);
}

/**
 * @brief  UDP Task. Listen to a UDP Socket.
 * @param  None
 * @retval None
 */
void TCPTask(void * pvParameters) {

    int sock = 0;
    int tcpconn = 0;
    long size = 0;
    int ret = 0;
    int i;
    unsigned char recv_buffer[20];
    uint8_t* send_buffer;
    //    uint8_t send_buffer[100];

    struct sockaddr_in address, remotehost;

    /* create a TCP socket */
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return;
    }

    /* bind to port 80 at any interface */
    address.sin_family = AF_INET;
    address.sin_port = htons(TCP_SOCKET_PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (const struct sockaddr *) (&address), sizeof (address)) < 0) {
        return;
    }

    /* listen for incoming connections (TCP listen backlog = 5) */
    listen(sock, 5);

    size = sizeof (remotehost);

    memset(recv_buffer, 0, sizeof (recv_buffer));

    for (;;) {

disconnect:
        
        printf("Waiting for connection\n");
        /* Wait for connection */
        tcpconn = accept(sock, (struct sockaddr *) (&remotehost), (socklen_t *) &size);
        printf("Accepted connection.\n");

        while (1) {

            if (NetSendTrigger != NULL) { // Check the semaphore exists
                if (xSemaphoreTake(NetSendTrigger, 100) == pdPASS) {

                    printf("Sending data now...\n");

                    /*
                     Send the channel 1 samples
                     */

                    /* Send the CH1 identifier hash */
                    char buffer[3] = "CH1";
                    if (write(tcpconn, buffer, sizeof (uint8_t)*3) < 3) {
                        goto disconnect;
                    }
                    vTaskDelay(10);

                    /* Send the samples before the trigger condition */
                    for (i = channel1_beforetrigger_counter; i < 25000; i += 10) {
                        if (write(tcpconn, channel1_beforetrigger + i, sizeof (uint8_t) * 10) < 10) {
                            goto disconnect;
                        }
                    }

                    for (i = 0; i < channel1_beforetrigger_counter; i += 10) {
                        if (write(tcpconn, channel1_beforetrigger + i, sizeof (uint8_t) * 10) < 10) {
                            goto disconnect;
                        }
                    }

                    for (i = 0; i < 25000; i += 10) {
                        /* Send the samples after the trigger condition */
                        if (write(tcpconn, channel1_aftertrigger + i, sizeof (uint8_t) * 10) < 10) {
                            goto disconnect;
                        }
                    }

                    write(tcpconn, "END", sizeof(uint8_t) * 3);
                    
                    i = 0;
                    /* Send the CH1 identifier hash */
                    char buffer2[3] = "CH2";
                    if (write(tcpconn, buffer2, sizeof (uint8_t)*3) < 3) {
                        goto disconnect;
                    }
                    vTaskDelay(10);

                    /* Send the samples before the trigger condition */
                    for (i = channel2_beforetrigger_counter; i < 25000; i += 10) {
                        if (write(tcpconn, channel2_beforetrigger + i, sizeof (uint8_t) * 10) < 10) {
                            goto disconnect;
                        }
                    }

                    for (i = 0; i < channel2_beforetrigger_counter; i += 10) {
                        if (write(tcpconn, channel2_beforetrigger + i, sizeof (uint8_t) * 10) < 10) {
                            goto disconnect;
                        }
                    }

                    for (i = 0; i < 25000; i += 10) {
                        /* Send the samples after the trigger condition */
                        if (write(tcpconn, channel2_aftertrigger + i, sizeof (uint8_t) * 10) < 10) {
                            goto disconnect;
                        }
                    }

                    write(tcpconn, "END", sizeof(uint8_t) * 3);

                    ADC_Start();
                }
            }

            vTaskDelay(1000);
        }
    }
}