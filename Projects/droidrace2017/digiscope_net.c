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

int currentConnectionFD = -1;
void handleReceivedPacket(char *data, int length);
void monitorConnection(void);

void digiscope_net_init() {
    // Start LwIP
    LwIP_Init();

    // Create TCP Task
    xTaskCreate((void *) &TCPTask, (const signed char *) "TCPTX", TCPTASK_STACK_SIZE, NULL, TCPTASK_PRIORITY, NULL);
}

int sendTcpPacket(char *sendBuf, int size) {
    int bytesSent;
    if (currentConnectionFD == -1) {
        return -1;
    }
    bytesSent = send(currentConnectionFD, sendBuf, size, 0);
    if (bytesSent < 0) {
        debugger_printf("Error: send failed.\n");
        VCP_txflush();
    }
    return bytesSent;
}

void TCPTask(void * pvParameters) {
    int status;
    int clientfd;
    int server;
    struct sockaddr_in localAddr;
    struct sockaddr_in clientAddr;
    int optval;
    int optlen = sizeof(optval);
    socklen_t addrlen = sizeof(clientAddr);

    server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server == -1) {
        debugger_printf("Error: socket not created.\n");
        goto shutdown;
    }

    memset(&localAddr, 0, sizeof(localAddr));
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(TCP_PORT);

    status = bind(server, (struct sockaddr *) &localAddr, sizeof(localAddr));
    if (status == -1) {
        debugger_printf("Error: bind failed.\n");
        goto shutdown;
    }

    status = listen(server, 1);
    if (status == -1) {
        debugger_printf("Error: listen failed.\n");
        goto shutdown;
    }

    optval = 1;
    if (setsockopt(server, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
        debugger_printf("Error: setsockopt keep alive failed\n");
        goto shutdown;
    }

    currentConnectionFD = -1;

    while ((clientfd = accept(server, (struct sockaddr *) &clientAddr, &addrlen)) != -1) {

        debugger_printf("Connection accepted. fd: %d\n", clientfd);
        VCP_txflush();
        currentConnectionFD = clientfd;
        monitorConnection();
        // Set to be disconnected

        /* addrlen is a value-result param, must reset for next accept call */
        addrlen = sizeof(clientAddr);
    }

    debugger_printf("Error: accept failed.\n");
    VCP_txflush();

  shutdown:
    if (server > 0) {
        VCP_txflush();
        close(server);
        currentConnectionFD = -1;
    }
}

void monitorConnection() {
    int bytesRcvd;
    int bytesSent;
    char buffer[TCP_PACKET_SIZE] = { 0 };

    while (1) {
        bytesRcvd = recv(currentConnectionFD, buffer, TCP_PACKET_SIZE, MSG_DONTWAIT);
        if (bytesRcvd > 0) {
            handleReceivedPacket(buffer, bytesRcvd);
            memset(buffer, '\0', TCP_PACKET_SIZE);
        } else if (bytesRcvd == 0) {
            // Other side disconnected
            break;
        }
        Task_sleep(100);
    }

    debugger_printf("Closed connection. fd: %d\n", currentConnectionFD);
    VCP_txflush();
    close(currentConnectionFD);
    currentConnectionFD = -1;
}

void handleReceivedPacket(char *data, int length) {

}