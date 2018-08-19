import socket
import sys

try:

    # Send data
    while True:

        # Create a TCP/IP socket
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        # Connect the socket to the port where the server is listening
        server_address = ('192.168.88.250', 10000)
        print('connecting to {} port {}'.format(*server_address))
        sock.connect(server_address)

        mess = input("Podaj komende: ")
        message = str.encode(mess)
        print(f"Sending: {message}")
        sock.sendall(message)

        # Look for the response
        amount_received = 0
        amount_expected = len(message)

        while amount_received < amount_expected:
            data = sock.recv(16)
            amount_received += len(data)
            print('received {!r}'.format(data))

finally:
    print('closing socket')
    sock.close()