# -*- coding: utf-8 -*-
"""
Created on Thu May 21 07:05:09 2020

@author: orangepi
"""
from time  import sleep

open("/sys/class/gpio/export", "w").write("16")
open("/sys/class/gpio/export", "w").write("18")

gpio_pin_16_mode = open("/sys/class/gpio/gpio16/direction", "w")
gpio_pin_18_mode = open("/sys/class/gpio/gpio18/direction", "w")

gpio_pin_16_mode.write("out")
gpio_pin_18_mode.write("out")

gpio_pin_16_val = open("/sys/class/gpio/gpio16/value", "w")
gpio_pin_18_val = open("/sys/class/gpio/gpio16/value", "w")
try:
    while 1:
        state = input()
        if(state):
            gpio_pin_16_val.write("1")
            gpio_pin_18_val.write("0")
            sleep(5)
            gpio_pin_16_val.write("0")
            gpio_pin_18_val.write("0")
        else:
            gpio_pin_16_val.write("0")
            gpio_pin_18_val.write("1")
            sleep(5)
            gpio_pin_16_val.write("0")
            gpio_pin_18_val.write("0")
except KeyboardInterrupt:
    gpio_exit = open("/sys/class/gpio/unexport", "w")
    gpio_exit.write("16")
    
 #   gpiosetup.close()
    gpio_pin_16_mode.close()
    gpio_pin_18_mode.close()
    gpio_pin_16_val.close()
    gpio_pin_18_val.close()
    gpio_exit.close()
    
    print("bye")
            
        
