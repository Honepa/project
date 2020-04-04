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
import os
app = Flask(__name__)

code = open("code.py", "w")
code.write("print('Hello World!')")
datapy = open("code.py", "r")

@app.route('/')
def index():
    return render_template('index.html', cod = datapy)

@app.route('/submit', methods=['POST'])
def submit():
    code.write(request.form['text'])

    #p = []
    #p = os.popen("./run_py.sh")
    #data = open("out", "r" ).read()    
    return render_template('index.html', cod = datapy, out = datapy)

if __name__ == '__main__':
    app.run()
