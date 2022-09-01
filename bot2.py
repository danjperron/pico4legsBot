import time
from machine import Pin,Timer
from servo import Servo
import sys
import network
import usocket
from secrets import secrets
import struct

led = machine.Pin('LED', machine.Pin.OUT)
# Set country to avoid possible errors / https://randomnerdtutorials.com/micropython-mqtt-esp32-esp8266/
rp2.country('CA')

wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.connect(secrets['ssid'],secrets['pw'])


wlan.connect(secrets['ssid'],secrets['pw'])

#set 10 seconds for time out delay
wlandelay =  time.ticks_ms() + 10000


while time.ticks_ms() < wlandelay:
  if wlan.isconnected():
      if wlan.status() <0  or wlan.status() >=3 :
          break
  machine.idle()


if wlan.status() != 3:
    raise RuntimeError('Wi-Fi connection failed')
    machine.reset()
else:
    for i in range(wlan.status()):
        led.on()
        time.sleep(.1)
        led.off()
    print('Connected')
    status = wlan.ifconfig()
    print('ip = ' + status[0])
    wlan_mac = wlan.config('mac')
    print("MAC Address:", wlan_mac)  # Show MAC for peering

sock = usocket.socket(usocket.AF_INET,usocket.SOCK_DGRAM)
sock.bind(("",5001))
#movement declaration

# from down to up

def GotSocket(self, _sock):
    print("Get sock",_sock)

# BL & FR forward  
# BR & FL backward
# BL & FR CENTER
# BR & FL forward
# BL & FR backward
# BR & FL CENTER



class Bot:
    #BL,BR,FL,FR
    step = [[ 0, 0, 0, 0],
            [ 1, 0, 0, 1],
            [ 1,-1,-1, 1],
            [ 0,-1,-1, 0],
            [ 0, 0, 0, 0],
            [ 0, 1, 1, 0],
            [-1, 1, 1,-1],
            [-1, 0, 0,-1]]
            
    
    
    def __init__(self):
        self.leftFactor = 1.0
        self.rightFactor = 1.0
        self.Angle= 30
        self.BR = Servo(Pin(18),factor=0.9)
        self.BR.center_ms=1.4
        self.FR = Servo(Pin(19))
        self.BL = Servo(Pin(20),invert=True,factor=1.1)
        self.FL = Servo(Pin(21),invert=True, factor=1.1)
        self.FL.center_ms=1.44
        #set timer for every 250ms
        self.move=0
        self.counter=0
        legTimer = Timer()
        legTimer.init(freq=10,mode=Timer.PERIODIC,callback=self.TimerFlag)


    def TimerFlag(self,_timer):
        if self.move==0:
            return
        if self.move >0:
            self.counter +=1
        else:
            self.counter +=7
        self.counter = self.counter % 8   
        self.set(self.step[self.counter % 8])

    def set(self,_step):
        if len(_step) == 4:
            self.BL.angle( self.leftFactor * self.Angle * _step[0])
            self.BR.angle( self.rightFactor * self.Angle * _step[1])
            self.FL.angle( self.leftFactor * self.Angle * _step[2])
            self.FR.angle( self.rightFactor * self.Angle * _step[3])


    def down(self):
        self.move = 0
        time.sleep_ms(500)
        self.set([-1,-1,-1,-1])
        
    def up(self):
        self.move = 0
        time.sleep_ms(500)
        self.set([0,0,0,0])
    
    def fwd(self,delay):
        if self.move==0:
            self.Counter=0
            self.up()
            time.sleep_ms(1000)
        self.move=1



if __name__ == "__main__":
    robot = Bot()    
    p = sock.recv(2)
    robot.move =struct.unpack("h",p)[0]
    print("sock recv:",p,robot.move)
    robot.down()
    time.sleep_ms(500)
    robot.up()
    time.sleep_ms(1000)

    while True:
        p = sock.recv(2)
        robot.move =struct.unpack("h",p)[0]
        print("sock recv:",p,robot.move)
        
    
sys.exit()
        