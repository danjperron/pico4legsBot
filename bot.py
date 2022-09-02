import time
from machine import Pin
import sys
import network
import usocket
from secrets import secrets
import struct
import botlegs

led = machine.Pin('LED', machine.Pin.OUT)
# Set country to avoid possible errors / https://randomnerdtutorials.com/micropython-mqtt-esp32-esp8266/
rp2.country('CA')

wlan = network.WLAN(network.STA_IF)
wlan.active(True)
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
    #get socket
    RcvPacket= sock.recv(4)  # 4 bytes fo position (x,y) -1,0 or 1
    if len(RcvPacket) !=4 :
        continue
    bot.move =struct.unpack("hh",RcvPacket)
    print("sock recv:",RcvPacket,bot.move)

sys.exit()
