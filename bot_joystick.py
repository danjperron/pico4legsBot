import pygame
import socket
import struct


#bot 4 legs info
bot_IP= "10.11.12.112"
bot_PORT = 5001



sock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)





pygame.init()
clock = pygame.time.Clock()
pygame.joystick.init()

print("number of joystick:",pygame.joystick.get_count())
joy = pygame.joystick.Joystick(0)
joy.init()
hats = joy.get_numhats()
print("number of hats",hats)



while True:

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            break
    # get hat value
    x , y = joy.get_hat(0)
    print("x:",x," y:",y)

    clock.tick(5)

#     packet = struct.pack("hh",x,y)
    packet = struct.pack("h",x)
#    print(packet)
#    bpacket= bytes(packet,encoding="utf-8")
#    print(bpacket)
    sock.sendto(packet,(bot_IP,bot_PORT))



joy.quit()
