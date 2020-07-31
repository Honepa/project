# -*- coding: utf-8 -*-
"""
    @author: honepa
"""

#export FLASK_APP=drive_fl.py
#flask run --host 0.0.0.0 --port 8080

from flask import Flask, render_template, request
import json

import sys
from serial import Serial, SerialException

PORTS = ['/dev/ttyUSB0', '/dev/ttyUSB1', '/dev/ttyUSB2', '/dev/ttyUSB3', '/dev/ttyUSB4', '/dev/ttyACM0', '/dev/ttyACM1', '/dev/ttyACM2']

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
    def __connect__(self):
        try:
            self.port.write(b'1')
            

    def __str__(self):
        return str(self.port)

arduino_due = Arduino(1)

app = Flask(__name__)

@app.route('/', methods=['GET','POST'])
def index(**qwargs):
    return render_template('index.html')

@app.route('/state', methods=['GET','POST'])
def drive_mod(**qwargs):
    state = request.form
    print(arduino_due)
    speed = state.get('speed')
    print(state.get('speed'))
    if state.get('forward') == "1":
        print("go forward", speed)
        arduino_due.port.write(b'2')
    elif state.get('back') == "1":
        print("go back", speed)
    elif state.get('left') == "1":
        print("go left", speed)
    elif state.get('right') == "1":
        print("go right", speed)
    elif state.get('on_line') == "1":
        print("go of line", speed)
    elif state.get('stop') == "1":
        print("stop!!!", speed)
    else:
        print("wait")
    return json.dumps({'success': True})

@app.errorhandler(404)
def not_found(error):
    return render_template('eror404.html'), 404


if __name__ == '__main__':
    app.run(debud=True)