# -*- coding: utf-8 -*-
"""
    @author: honepa
"""
"""
code = open("code.py", "w")
code.write("print(21)\n print(22)\nprint(23)\n")

#data = open("out", "r").readlines()
print(open("out", "r").read())
code.close()
"""
#export FLASK_APP=text_fl.py
#flask run --host 0.0.0.0 --port 8080 
from flask import Flask, render_template, request
#import os
import subprocess
app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/submit', methods=['POST'])
def submit():
    code = open('code.py', "w")
    innin = request.form['text']
    code.write(innin)
    code.close()
    subprocess.Popen(['bash', 'run_py.sh'])
    outin = open('out', "r").read()
    #outin.close()
    return render_template('index.html', out = outin)

if __name__ == '__main__':
    app.run()
