from gpiozero import LED

from gpiozero.pins.pigpio import PiGPIOFactory

from time import sleep

factory = PiGPIOFactory(host='192.168.2.202')

red = LED(17,pin_factory=factory)

while True:

    red.on()

    sleep(1)

    red.off()

    sleep(1)