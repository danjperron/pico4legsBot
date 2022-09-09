from machine import Pin,PWM

# PICO pwm is base on freq an 0..65535 value
# so ms/step at freq(50) 20/65535 = 0.3us/STEP
#  servo center = 1.5ms => 4915 step
#  servo -45 degree = 1.0 ms =>  3277
#  servo +45 degree = 2.0 ms =  6553
# if you go behond +/- 45 degree not all servo are egual


class Servo:
    
    def __init__(self , pin, invert=False, factor=1.0):
        self.pwm = PWM(pin)
        self.pwm.freq(50)
        self.center_ms= 1.5
        self.invert = invert
        self.factor = factor
        
    def setms(self, timems):
        #from milli second to  nano second
        self.pwm.duty_ns(int(timems * 1000000))
        
    def angle(self, Angle):
        if self.invert:
            self.setms(self.center_ms  + self.factor*(-Angle* 0.5 /45.0))
        else:
            self.setms(self.center_ms  + self.factor*(Angle* 0.5 /45.0))

    def adjustCenter(self,valuems):
        self.center_ms=valuems
        self.setms(valuems)
        


# example

if __name__ == "__main__":

    import time, random
    #initiate 2 servos
    servo1 = Servo(Pin(13))
    servo2 = Servo(Pin(12))
    servo1.angle(0)
    servo2.angle(0)
    
    try:

        while True:
           servo1.angle(-60)
           servo2.angle(-60)
           time.sleep_ms(200)
           servo1.angle(60)
           servo2.angle(60)
           time.sleep_ms(200)
    except KeyboardInterrupt:
        pass
    


