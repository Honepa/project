# -*- coding: utf-8 -*-
import sys
from serial import Serial, SerialException

PORTS = ['/dev/ttyUSB0', '/dev/ttyUSB1', '/dev/ttyUSB2', '/dev/ttyUSB3', '/dev/ttyUSB4']


class Arduino():
    
    def __init__(self, **kwargs):
        self.findport = None
        
        for port in PORTS:
            try:
                #print(port)
                self.port = Serial(port = port, baudrate = 9600, timeout = 2)
                #print(self.port.readlines())
                b = self.port.read()
                left = bytes('2', encoding = "ASCII")
                right = bytes('1', encoding = "ASCII")
                
                if b == right:
                    print('Arduino in right', b, self.port)
                elif b == left:
                    print('Arduino in left', b, self.port)
                else:
                    break
                
            except SerialException as e:
                pass
            
        if self.port == None:
            raise SerialException('Port is not found')
    
    def __del__(self):
        try:
            self.port.close()
        except AttributeError:
            print('nicht close', file = sys.stderr)

    def __str__(self):
        return 'Arduino!!!!! ' + str(self.port)
    
if __name__ == '__main__':
    
    arduino = Arduino()
    
    #print(arduino.port.readline())    
    del arduino

    
     