from machine import Timer
import gamepad_hw

class GamePad():

    # normal GamePadData is
    #
    # S,x,y,z,Rx,Ry,hat, buttons 
    # Only buttons is 32 bits , the rest is 8  bits
    # On mine z doesn't exist
    
    
    #hat definition
    #
    #     7  0  1
    #     6  8  2
    #     5  4  3
    #
    
    HatTable =  ((0,1),(1,1),(1,0),(1,-1),(0,-1),(-1,-1),(-1,0),(-1,1),(0,0))
    HatIndex= 5
        
    def __init__(self, freq=100):
        self.periodic_pull= Timer()
        self.periodic_pull.init(freq=freq, mode=Timer.PERIODIC, callback= gamepad_hw.pull)
        
    def get(self):
        return gamepad_hw.get()

    def hat(self):
        return self.HatTable[gamepad_hw.get()[5]]
    

        
    
