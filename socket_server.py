"""
----- socket_server.py-----
Feedback incoming socket client request
Version 0.3
This project is a part of CPE-312 Embedded System
Copyrights   Teeraphat Kullanankanjana
"""

# import module
import socket
import pygame
from datetime import datetime

# setting host&port 
host = "192.168.1.161"
port = 1102

pygame.init()
now = datetime.now().strftime("%H:%M:%S")
end_sound = pygame.mixer.Sound("/home/pi/Desktop/flukework/PythonSocket/victory.wav")
def play_sound():
    end_sound.play()
# start socket server
with socket.socket(socket.AF_INET,socket.SOCK_STREAM) as s:
    try: # error handler 
        s.bind((host,port)) # start server
        print("{}: Server Start at IP {}:{}\nAwaiting Client Connection....\n".format(now,host,port)) # print server address 
        s.listen(5)  # listen incoming message
        while True: # connection lost handler 
            connection, address = s.accept() # accept incoming client
            print("\n{}: Connected by {} ".format(now,address)) # print client address
            with connection as c: # sand data back to client  
                data = c.recv(1024) # data recvied
                c.sendall(b"Thank you for connecting from server") # send massage back to clinet
                if data.decode("utf-8") == "1": # if recived data is 1 then player 1 win
                    print("{}: Player 1 Lose\tPlayer 2 Win\n".format(now))
                    play_sound()
                elif data.decode("utf-8") == "2": # if recived data is 2 then player 2 win
                    print("{}: Player 2 Lose\tPlayer 1 Win".format(now))
                    play_sound()
                else: # other recived data
                    print("{} Invalid data ".format(now))
                    pass # just pass recived data
    except OSError as e: # print error 
        print(e)


