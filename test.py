import socket
import sys
import struct

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = ('10.0.1.42', 3000)
print >>sys.stderr, 'connecting to %s port %s' % server_address
sock.connect(server_address)

try:

    amount_received = 0

    while (True):
        data = sock.recv(2)
        amount_received += len(data)
        datamap = int.from_bytes(b, byteorder='big', signed=False)
        for i in datamap:
            print(i)

finally:
    print >>sys.stderr, 'closing socket'
    sock.close()
