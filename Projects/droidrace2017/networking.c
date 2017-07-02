/*
 * ============================================================================
 * Created by Timothy Ryan Hadwen
 * Created: Mar 26, 2016
 * ============================================================================
 */

#include "networking.h"

QueueHandle_t TCPSendQueue;

static int currentConnectionFD = -1;
void handleReceivedPacket(char *data, int length);
void monitorConnection(void);
int sendTcpPacket(char *sendBuf, int size);

void networking_init() {
	// Start LwIP
	LwIP_Init();

	// Create TCP Task
	xTaskCreate((void * ) &TCPTask, (const signed char * ) "TCP",
			TCPTASK_STACK_SIZE, NULL, TCPTASK_PRIORITY, NULL);
}

int TCPIsConnected() {
	if (currentConnectionFD == -1) {
		return 0;
	}
	return 1;
}

int sendTcpPacket(char *sendBuf, int size) {
	int bytesSent;
	if (currentConnectionFD == -1) {
		return -1;
	}
	bytesSent = send(currentConnectionFD, sendBuf, size, 0);
	if (bytesSent < 0) {
		tfp_printf("Error: send failed.\n");
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

	TCPSendQueue = xQueueCreate(TCP_SEND_QUEUE_SIZE, sizeof(char) * TCP_PACKET_SIZE);

	server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server == -1) {
		tfp_printf("Error: socket not created.\n");
		goto shutdown;
	}

	memset(&localAddr, 0, sizeof(localAddr));
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddr.sin_port = htons(TCP_PORT);

	status = bind(server, (struct sockaddr *) &localAddr, sizeof(localAddr));
	if (status == -1) {
		tfp_printf("Error: bind failed.\n");
		goto shutdown;
	}

	status = listen(server, 1);
	if (status == -1) {
		tfp_printf("Error: listen failed.\n");
		goto shutdown;
	}

	optval = 1;
	if (setsockopt(server, SOL_SOCKET, SO_KEEPALIVE, &optval, optlen) < 0) {
		tfp_printf("Error: setsockopt keep alive failed\n");
		goto shutdown;
	}

	currentConnectionFD = -1;

	while ((clientfd = accept(server, (struct sockaddr *) &clientAddr, &addrlen))
			!= -1) {

		tfp_printf("Connection accepted. fd: %d\n", clientfd);
		currentConnectionFD = clientfd;
		monitorConnection();
		// Set to be disconnected

		/* addrlen is a value-result param, must reset for next accept call */
		addrlen = sizeof(clientAddr);
	}

	tfp_printf("Error: accept failed.\n");

	shutdown: if (server > 0) {
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
		vTaskDelay(100);
	}

	tfp_printf("Closed connection. fd: %d\n", currentConnectionFD);
	close(currentConnectionFD);
	currentConnectionFD = -1;
}

void handleReceivedPacket(char *data, int length) {
	int value = 0;
	if (data[0] == 'T') {

	} else if (data[0] = 'S') {
		sscanf(data, "S%d", value);
		printf("Received steering packet %d\n\r", value);
		setSteering(value);
	}

	int i;
	for (i = 0; i < length; i++) {
		/* Convert to ASCII upper case */
		if (data[i] > 96) {
			data[i] -= 32;
		}
	}
//	sendTcpPacket(data, length);
}
