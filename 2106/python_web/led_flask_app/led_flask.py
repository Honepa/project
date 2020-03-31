# -*- coding: utf-8 -*-
"""
Created on Tue Mar 31 08:27:41 2020

@author: orangepi
"""
#export FLASK_APP=led_flask.py
#flask run --host 0.0.0.0 --port 8080 
from flask import Flask, render_template

open("/sys/class/gpio/export", "w").write("16")

gpio_pin_16_mode = open("/sys/class/gpio/gpio16/direction", "w")
gpio_pin_16_mode.write("out")

led_flask_app = Flask(__name__)

@led_flask_app.route('/')
def index():
    return render_template('led_ht.html')
    
@led_flask_app.route('/on/')
def on():
    open("/sys/class/gpio/gpio16/value", "w").write("1")
    return render_template('led_ht.html')
    
@led_flask_app.route('/off/')
def off():
    open("/sys/class/gpio/gpio16/value", "w").write("0")
    return render_template('led_ht.html')    

if __name__ == '__main__':
    led_flask_app.run()