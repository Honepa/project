# -*- coding: utf-8 -*-
"""
    @author: honepa
"""

#export FLASK_APP=drive_fl.py
#flask run --host 0.0.0.0 --port 8080

from flask import Flask, render_template, request
import json

app = Flask(__name__)

@app.route('/', methods=['GET','POST'])
def index(**qwargs):
    return render_template('index.html')

@app.route('/state', methods=['GET','POST'])
def drive_mod(**qwargs):
    state = request.form
    print(state.get('speed'))
    return json.dumps({'success': True})

@app.errorhandler(404)
def not_found(error):
    return render_template('eror404.html'), 404


if __name__ == '__main__':
    app.run(debud=True)