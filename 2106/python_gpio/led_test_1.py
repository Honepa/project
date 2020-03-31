#!/usr/bin/envy python
# -*- coding utf-8 -*-
import OPi.GPIO as GPIO
from time  import sleep

GPIO.setboard(GPIO.ZEROPLUS2H3)
GPIO.setmode(GPIO.BOARD)

#led = GPIO.PA+3
GPIO.setup(3, GPIO.OUT)

while 1:
    GPIO.output(3, 1)
    sleep(0.1)
    GPIO.output(3, 0)
    sleep(0.1)