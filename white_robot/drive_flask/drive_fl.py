# -*- coding: utf-8 -*-
"""
    @author: honepa
"""

from flask import Flask, render_template, request

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.errorhandler(404)
def not_found(error):
    return render_template('eror404.html'), 404

@app.route('/submit', methods=['POST'])
def submit():
    in_time = request.form['text']
    if request.form['button'] == 'Вперёд':
        print(in_time, "forward")
    elif request.form['button'] == 'Назад':
        print(in_time, "back")
    elif request.form['button'] == 'Направо':
        print("Направо")
    elif request.form['button'] == 'Налево':
        print("Налево")
    elif request.form['button'] == 'Стоп':
        print("Стоп")
        """
        экстренная остановка.
        """
    return render_template('index.html')


if __name__ == '__main__':
    app.run()