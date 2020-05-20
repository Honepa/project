# -*- coding: utf-8 -*-
"""
    @author: honepa
"""
#export FLASK_APP=text_fl.py
#flask run --host 0.0.0.0 --port 8080 
from flask import Flask, render_template

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')
    
@app.route('/submit', methods=['POST'])
def submit():
    return render_template('index.html')
    
@app.route('/select_arduino', methods=['POST'])
def select_arduino():
    return render_template('index.html')    
    
if __name__ == '__main__':
    app.run()