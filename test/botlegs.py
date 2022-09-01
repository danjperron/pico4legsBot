import time
from machine import Pin,Timer
from servo import Servo

# BL & FR forward
# BR & FL backward
# BL & FR CENTER
# BR & FL forward
# BL & FR backward
# BR & FL CENTER

class BotLegs:
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
        self.BR = Servo(Pin(18))
        self.FR = Servo(Pin(19))
        self.BL = Servo(Pin(20),invert=True)
        self.FL = Servo(Pin(21),invert=True)
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

    def fwd(self):
        self.move=1

    def back(self):
        self.move=-1

    def stop(self):
        self.move=0

if __name__ == "__main__":
    bot = BotLegs()
    bot.up()
    time.sleep_ms(1000)
    #move forward 5 seconds
    bot.fwd()
    time.sleep_ms(5000)
    #move back  5 seconds
    bot.back()
    time.sleep_ms(5000)
    #stop
    bot.stop()
    time.sleep_ms(500)
    bot.down()
