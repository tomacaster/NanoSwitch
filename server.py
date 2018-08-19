import socket
import sys

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the port
server_address = ('192.168.88.250', 10000)
print('starting up on {} port {}'.format(*server_address))
sock.bind(server_address)

# Listen for incoming connections
sock.listen(1)

while True:
    # Wait for a connection
    print('waiting for a connection')
    connection, client_address = sock.accept()
    try:
        print('connection from', client_address)

        # Receive the data in small chunks and retransmit it
        while True:
            data = connection.recv(3)
            command = data
            if data == b"r2":
                print(f"command {command}")
                print(f"recived message: {data}")
                connection.sendall(b"tab2 Reboot...")
                break
            else:
                print("Wrong message...")
                break
                
    finally:
        # Clean up the connection
        connection.close()