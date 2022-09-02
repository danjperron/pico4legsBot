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
    straight = [[ 0, 0, 0, 0],
                [ 1, 0, 0, 1],
                [ 1,-1,-1, 1],
                [ 0,-1,-1, 0],
                [ 0, 0, 0, 0],
                [ 0, 1, 1, 0],
                [-1, 1, 1,-1],
                [-1, 0, 0,-1]]

    rotate = [[  0, 0, 0, 0],
              [  0, 0, 1, 0],
              [ -1, 0, 1, 0],
              [ -1, 0, 0, 0],
              [  0, 0, 0, 0],
              [  0,-1, 0, 0],
              [  0,-1, 0, 1],
              [  0, 0, 0, 1]]




    ''' movement
          fwd = (0,1)
          fwd_right = (1,1)
          rotate_right = (1,0)
          back_right = (1, -1)
          back = (0,-1)
          back_left = (-1,-1)
          rotate_left = (-1,0)
          fwd_left = (-1,1)
          each movement will start with all legs UP
    '''

    def __init__(self):
        self.leftFactor = 1.0
        self.rightFactor = 1.0
        self.Angle= 30
        self.BR = Servo(Pin(18))
        self.FR = Servo(Pin(19))
        self.BL = Servo(Pin(20),invert=True)
        self.FL = Servo(Pin(21),invert=True)
        self.currentMove = (0,0)  #(x,y) -1,0 or 1
        self.move=(0,0)
        self.StopFlag= True
        self.counter=0
        legTimer = Timer()
        legTimer.init(freq=10,mode=Timer.PERIODIC,callback=self.TimerFlag)



    # next modification will be to use only one table but change L,R variable accordingly
    def TimerFlag(self,_timer):
        # check modulus 4 to check next step change
        if (self.counter % 4)==0:
                self.CurrentMove = self.move
        if self.CurrentMove==(0,0):
                return;
        # skip diagonal for now
        if self.CurrentMove[0]==0:
            #forward or backward
            if self.CurrentMove[1] >0:
                self.counter +=1
            else:
                self.counter +=7
            self.counter = self.counter % 8
            self.set(self.straight[self.counter])
        elif self.CurrentMove[1]==0:
            #rotate
            if self.CurrentMove[0] >0:
                self.counter +=1
            else:
                self.counter +=7
            self.counter = self.counter % 8
            self.set(self.rotate[self.counter])
        else:
            #rotate and  straight
            if self.CurrentMove[0] > 0:
                L = 1.2
                R = 0.6
            else:
                L = 0.6
                R = 1.2
            if self.CurrentMove[1] >0:
                self.counter +=1
            else:
                self.counter +=7
            self.counter = self.counter % 8
            nextstep = (self.straight[self.counter][0] * L,
                        self.straight[self.counter][1] * R,
                        self.straight[self.counter][2] * L,
                        self.straight[self.counter][3] * R)
            self.set(nextstep)



    def set(self,_step):
        if len(_step) == 4:
            self.BL.angle( self.leftFactor * self.Angle * _step[0])
            self.BR.angle( self.rightFactor * self.Angle * _step[1])
            self.FL.angle( self.leftFactor * self.Angle * _step[2])
            self.FR.angle( self.rightFactor * self.Angle * _step[3])

    def down(self):
        self.move = (0,0)
        time.sleep_ms(500)
        self.set([-1,-1,-1,-1])

    def up(self):
        self.move = (0,0)
        time.sleep_ms(500)
        self.set([0,0,0,0])

    def fwd(self):
        self.move=(0,1)

    def back(self):
        self.move=(0,-1)

    def stop(self):
        self.move=(0,0)

    def turnLeft(self):
        self.move=(-1,0)

    def turnRight(self):
        self.move=(1,0)



if __name__ == "__main__":
    bot = BotLegs()
    #adjust servo
    # on first time remove adjustment
    # use bot.up() and ajust arm to be up and perpendicular
    # next use  on each servo adjustCenter(x) where x is set to be perpendicular 1.5ms is normally the center
    # then use on each servo  angle(45) and play withe the factor to have all servo with the same span
    #  ex : bot.BR.adjustCenter(1.4)
    #       bot.BR.angle(45)
    #       bot.BR.factor(1.2)
    #       bot.BR.angle(45)
    # once is done for all just add the settings after you create the BotLegs()

    bot.BR.center_ms=1.4
    bot.BL.factor=1.1
    bot.FL.factor=1.1
    bot.FL.center_ms=1.44

    bot.up()
    time.sleep_ms(1000)
    #move forward 5 seconds
    bot.fwd()
    time.sleep_ms(5000)
    #move back  5 seconds
    bot.back()
    time.sleep_ms(5000)
    #stop
    bot.turnLeft()
    time.sleep_ms(5000)
    bot.turnRight()
    time.sleep_ms(5000)
    bot.stop()
    time.sleep_ms(500)
    bot.down()

