# -*- coding: utf-8 -*-
"""
Created on Mon Mar 30 11:51:55 2020

@author: orangepi
"""
from flask import Flask
app = Flask(__name__)

app.route('/')


def hello_world():
    return 'hello world'
if __name__ == '(__name__)':
    app.run(host='127.0.0.0', port=3000)
