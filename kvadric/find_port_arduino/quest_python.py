# -*- coding: utf-8 -*-
import sys
from serial import Serial, SerialException

PORTS = ['/dev/ttyUSB0', '/dev/ttyUSB1', '/dev/ttyUSB2', '/dev/ttyUSB3', '/dev/ttyUSB4']


class Arduino():
    
    def __init__(self, x):
        self.port = None
        for port in PORTS:
            try:
                #print(port)
                self.port = Serial(port = port, baudrate = 9600, timeout = 2)
                #print(self.port.readlines())
                b = self.port.read()
                
                
                if b == x:
                    break               
               
                
            except SerialException as e:
                print(port, 'failed')
            
        if self.port == None:
            raise SerialException('Port is not found')
    
    def __del__(self):
        try:
            self.port.close()
        except AttributeError:
            print('nicht close', file = sys.stderr)

    def __str__(self):
        return str(self.port)
    
if __name__ == '__main__':
    
    left = Arduino(1)
    right = Arduino(2)
    print('L=%s\nR=%s' % (left,right))
    #print(left.port.readline())    
    del left, right

    
     