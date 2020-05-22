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
gpio_pin_18_val = open("/sys/class/gpio/gpio18/value", "w")
try:
    while 1:
        state = input()
        if(state):
            print(state)
            gpio_pin_16_val.write("1")
            gpio_pin_18_val.write("0")
            sleep(5)
            gpio_pin_16_val.write("0")
            gpio_pin_18_val.write("0")
        else:
            print(state)
            gpio_pin_16_val.write("0")
            gpio_pin_18_val.write("1")
            sleep(5)
            gpio_pin_16_val.write("0")
            gpio_pin_18_val.write("0")
except KeyboardInterrupt:
    gpio_exit_16 = open("/sys/class/gpio/unexport", "w")
    gpio_exit_18 = open("/sys/class/gpio/unexport", "w")
    gpio_exit_16.write("16")
    gpio_exit_18.write("18")
 #   gpiosetup.close()
    gpio_pin_16_mode.close()
    gpio_pin_18_mode.close()
    gpio_pin_16_val.close()
    gpio_pin_18_val.close()
    gpio_exit_16.close()
    gpio_exit_18.close()
    print("bye")
            
        
