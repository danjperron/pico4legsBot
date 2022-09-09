import time
from machine import Pin
import struct
import botlegs
import gamepad

led = machine.Pin('LED', machine.Pin.OUT)
# Set country to avoid possible errors / https://randomnerdtutorials.com/micropython-mqtt-esp32-esp8266/

# create the bot
# adjust some legs settings
bot = botlegs.BotLegs()

#adjust servo
bot.BR.center_ms=1.4
bot.BL.factor=1.1
bot.FL.factor=1.1
bot.FL.center_ms=1.44

bot.up()
time.sleep_ms(1000)


while True:
    bot.move =gamepad.hat()
    print(bot.move)
    time.sleep_ms(50)

sys.exit()
