# -*- coding: utf-8 -*-
import sys
from serial import Serial, SerialException

PORTS = ['/dev/ttyUSB0', '/dev/ttyUSB1', '/dev/ttyUSB2', '/dev/ttyUSB3', '/dev/ttyUSB4']
#arduino = Serial('/dev/ttyUSB0', 9600, timeout =10) 
class FIND_ARDUINO():
    
    def __init__(self):
        self.findport = None
        for port in PORTS:
            try:
                print(port)
                self.findport = Serial(port = port, baudrate = 9600, timeout = 2)
                print(self.findport.readlines())
                break
            except SerialException as e:
                pass
        if self.findport == None:
            raise SerialException('Port is not found')
    
    def __del__(self):
        try:
            self.findport.close()
        except AttributeError:
            print('nicht close', file = sys.stderr)

if __name__ == '__main__':
    port_arduino = FIND_ARDUINO()
    del port_arduino
    
     