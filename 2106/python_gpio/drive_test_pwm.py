# -*- coding: utf-8 -*-
"""
Created on Tue May 26 11:42:59 2020

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

open("/sys/class/gpio/export", "w").write("6")

gpio_pin_6_mode = open("/sys/class/gpio/gpio6/direction", "w")

gpio_pin_6_mode.write("out")

center = 1

try:
    while 1:
        state = input()
        if state == "f":
            print(state)
            open("/sys/class/gpio/gpio16/value", "w").write("1")
            open("/sys/class/gpio/gpio18/value", "w").write("0")
            sleep(2)
            open("/sys/class/gpio/gpio16/value", "w").write("0")
            open("/sys/class/gpio/gpio18/value", "w").write("0")
        elif state == "b":
            print(2)
            open("/sys/class/gpio/gpio16/value", "w").write("0")
            open("/sys/class/gpio/gpio18/value", "w").write("1")
            sleep(2)
            open("/sys/class/gpio/gpio16/value", "w").write("0")
            open("/sys/class/gpio/gpio18/value", "w").write("0")
        elif state == "r":
            if center:
                i = 10
                while i > 0:
                    open("/sys/class/gpio/gpio6/value", "w").write("1")
                    sleep(0.017)
                    print(")")
                    open("/sys/class/gpio/gpio6/value", "w").write("0")
                    sleep(0.001 )
                    print("*")
                    i = i - 1
                center = 0
            elif center == 2:
                i  = 10
                while i > 0:
                    open("/sys/class/gpio/gpio6/value", "w").write("1")
                    sleep(0.019)
                    print(")")
                    open("/sys/class/gpio/gpio6/value", "w").write("0")
                    sleep(0.002 )
                    print("*")
                    i = i - 1
                i = 10
                while i > 0:
                    open("/sys/class/gpio/gpio6/value", "w").write("1")
                    sleep(0.017)
                    print(")")
                    open("/sys/class/gpio/gpio6/value", "w").write("0")
                    sleep(0.001)
                    print("*")
                    i = i - 1
                center = 0
        elif state == "l":
            if center:
                i = 10
                while i > 0:
                    open("/sys/class/gpio/gpio6/value", "w").write("1")
                    sleep(0.001)
                    print(")")
                    open("/sys/class/gpio/gpio6/value", "w").write("0")
                    sleep(0.017 )
                    print("*")
                    i = i - 1
                center = 2
            elif center == 0:
                i  = 10
                while i > 0:
                    open("/sys/class/gpio/gpio6/value", "w").write("1")
                    sleep(0.002)
                    print(")")
                    open("/sys/class/gpio/gpio6/value", "w").write("0")
                    sleep(0.019)
                    print("*")
                    i = i - 1
                i = 10
                while i > 0:
                    open("/sys/class/gpio/gpio6/value", "w").write("1")
                    sleep(0.001)
                    print(")")
                    open("/sys/class/gpio/gpio6/value", "w").write("0")
                    sleep(0.017 )
                    print("*")
                    i = i - 1
                center = 2
        elif state == "c":
            if center == 2:
                i  = 10
                while i > 0:
                    open("/sys/class/gpio/gpio6/value", "w").write("1")
                    sleep(0.019)
                    print(")")
                    open("/sys/class/gpio/gpio6/value", "w").write("0")
                    sleep(0.002)
                    print("*")
                    i = i - 1
            elif center == 0:
                i  = 10
                while i > 0:
                    open("/sys/class/gpio/gpio6/value", "w").write("1")
                    sleep(0.002)
                    print(")")
                    open("/sys/class/gpio/gpio6/value", "w").write("0")
                    sleep(0.019)
                    print("*")
                    i = i - 1
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
    
    gpio_exit_6 = open("/sys/class/gpio/unexport", "w")
    
    gpio_exit_6.write("6")
 #   gpiosetup.close()
    
    gpio_pin_6_mode.close()
    
    gpio_exit_6.close()
    
    print("bye")
            