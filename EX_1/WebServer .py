from socket import *
import sys


def main():
    # http://10.100.102.26:2000/HelloWorld.html
    serverPort = 2000
    serverSocket = socket(AF_INET, SOCK_STREAM)
    server_ADDRESS = ("10.100.102.26", serverPort)
    serverSocket.bind(server_ADDRESS)
    serverSocket.listen(1)
    while True:
        print('Ready to serve...')
        connectionSocket, addr = serverSocket.accept()
        try:
            message = connectionSocket.recv(2048).decode()
            filename = message.split()[1]
            f = open(filename[1:])
            outputdata = f.read();
            connectionSocket.send("HTTP/1.1 200 OK\r\n\n".encode())
            for i in range(0, len(outputdata)):
                connectionSocket.send(outputdata[i].encode())
            connectionSocket.send("\r\n".encode())
            connectionSocket.close()
        except IOError:
            connectionSocket.send("HTTP/1.1 404 Not Found\r\n\r\n".encode())
            connectionSocket.sendto("404 Not Found".encode(), addr)
            connectionSocket.close()
    serverSocket.close()
    sys.exit()

if __name__ == '__main__':
    main()
