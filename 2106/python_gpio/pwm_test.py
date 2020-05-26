# -*- coding: utf-8 -*-
"""
Created on Tue May 26 07:36:01 2020

@author: orangepi
"""

from time import sleep

open("/sys/class/gpio/export", "w").write("6")

gpio_pin_6_mode = open("/sys/class/gpio/gpio6/direction", "w")

gpio_pin_6_mode.write("out")
i = 1000
gpio_pin_6_val = open("/sys/class/gpio/gpio6/value", "w")
try:
    while 1:
        open("/sys/class/gpio/gpio6/value", "w").write("1")
        sleep(0.000544)
        print(")")
        open("/sys/class/gpio/gpio6/value", "w").write("0")
        sleep(0.019456 )
        print("*")
        i = i - 1;
except KeyboardInterrupt:
    
    gpio_exit_6 = open("/sys/class/gpio/unexport", "w")
    
    gpio_exit_6.write("6")
 #   gpiosetup.close()
    
    gpio_pin_6_mode.close()
    
    gpio_pin_6_val.close()
    
    gpio_exit_6.close()
    print("bye")