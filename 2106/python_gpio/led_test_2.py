from time  import sleep

#gpiosetup = open("/sys/class/gpio/export", "w")
open("/sys/class/gpio/export", "w").write("16")

gpio_pin_16_mode = open("/sys/class/gpio/gpio16/direction", "w")
gpio_pin_16_mode.write("out")


try:
   while 1:   
       open("/sys/class/gpio/gpio16/value", "w").write("1")
       sleep(1)
       open("/sys/class/gpio/gpio16/value", "w").write("0")
       sleep(1)
except KeyboardInterrupt:
    gpio_exit = open("/sys/class/gpio/unexport", "w")
    gpio_exit.write("16")
    
 #   gpiosetup.close()
    gpio_pin_16_mode.close()
    gpio_exit.close()
    
    print("bye")