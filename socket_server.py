"""
----- socket_server.py-----
Feedback incoming socket client request
Version 0.2
This project is a part of CPE-312 Embedded System
Copyrights   Teeraphat Kullanankanjana
"""

# import module
import socket
import datetime

# host & port setup
host = "192.168.1.56"
port = 1102

# initializing server config
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:  # protocol config
    try:  # error handle
        s.bind((host, port))  # server start
        print("Server start at IP {}:{}".format(host, port))
        s.listen(5)  # listen incoming massage
        while True:  # Infinity loop
            connection, address = s.accept()  # accept incoming client
            print("{} Connected by {} ".format(datetime.datetime.now(), address))
            with connection as c:  # received data handler
                data = c.recv(1024)  # initializing received data variable
                print("{} incoming massage from{} : {} \n".format(datetime.datetime.now(), address, data.decode("utf-8")))
                c.sendall(b"Hello from server")  # feedback massage
                # check the winning conditions
                if data.decode("utf-8") == "1":
                    print("Player 1 win")
                elif data.decode("utf-8") == "2":
                    print("Player 2 win")
                else:
                    print("Invalid data")
                    pass  # if received data not match then discard the data
    except OSError as e:
        print(e)  # print error to console
