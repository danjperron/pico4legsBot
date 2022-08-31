import time
from machine import Pin
from servo import Servo


class Bot:
    
    def __init__(self):
        self.BR = Servo(Pin(18),factor=0.7)
        self.BR.center_ms=1.4
        self.FR = Servo(Pin(19))
        self.BL = Servo(Pin(20),invert=True,factor=1.1)
        self.FL = Servo(Pin(21),invert=True, factor=1.1)
        self.FL.center_ms=1.44
        
    def down(self):
        self.BL.angle(45)
        self.FL.angle(45)
        self.BR.angle(45)
        self.FR.angle(45)
        
    def up(self):
        self.BL.angle(0)
        self.FL.angle(0)
        self.BR.angle(0)
        self.FR.angle(0)
    
    def fwd(self,delay):
        self.up()
        self.FR.angle(45)
        self.BL.angle(45)
        time.sleep_ms(delay)
        self.FL.angle(-45)
        self.BR.angle(-45)
        time.sleep_ms(delay)
        self.BL.angle(0)
        self.FR.angle(0)
        time.sleep_ms(delay)
        self.FL.angle(0)
        self.BR.angle(0)
        time.sleep_ms(delay)
        
if __name__ == "__main__":
    robot = Bot()
    robot.down()
    time.sleep_ms(500)
    robot.up()
    time.sleep_ms(500)
    for i in range(10):
        robot.fwd(300)
        
    time.sleep_ms(1000)
    robot.down()

        