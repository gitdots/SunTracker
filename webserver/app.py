import os
import socket

from flask import Flask, jsonify, render_template
from flask_socketio import SocketIO
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
import threading 
import queue
# from models import Readings

FIRST = True
SOCKET_PATH = os.path.join('appl2flask.socket')
lock = threading.Lock()
condition = threading.Condition(lock)
data = None
first_request = True
db = SQLAlchemy()
app = Flask(__name__, instance_relative_config=True)
DB_PATH = os.path.join(app.instance_path, 'test.db')
new_data_queue = queue.Queue()

app.config['SQLALCHEMY_DATABASE_URI'] = f'sqlite:///{DB_PATH}'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

client = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
client.settimeout(0.2)  # set timeout to prevent blocking
client.connect(SOCKET_PATH)

# def read_new_data_thread():
#     while True:
#         data = client.recv(1024).decode()
#         if data:
#             new_data_queue.put(data)

# def handle_new_data():
#     with app.app_context():
#         while True:
#             try:
#                 data = new_data_queue.get(block=False)
#                 new_data = data


@app.route('/')
def index():
    return render_template('index.html')

import random
@app.route('/')
def home():
    return render_template('index.html')

@app.route('/get_data')
def get_data():
    try:
        data = client.recv(1024).decode()
        print(f'received: {data}')
    except socket.timeout as e:
        data = "No new data"
    return jsonify({'data': data})

@app.route('/statistics')
def statistics():
    return render_template('stats.html')

if __name__ == '__main__':
    app.run()
