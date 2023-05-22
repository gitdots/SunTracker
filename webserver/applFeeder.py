import threading
import os
import select
import time
import socket

from enum import Enum

from .ReadingModel import ReadingSchema
from .app import db

class RunningMode(Enum):
    NOT_RUNNING = 0
    RUNNING_DB = 1
    RUNNING_LIVE = 2

SOCKET_PATH = os.path.join('webserver', 'appl2flask.socket')
TIMES_FOR_AVERAGE_BEFORE_DB_PUSH = 900 # 15mins
appl_connected = False

MAX_TRIES_TO_CLOSE = 5

class ApplicationFeeder():
    def __init__(self):
        self.socket_path = SOCKET_PATH
        self.socket = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        if os.path.exists(self.socket_path):
            os.remove(self.socket_path)
        self.socket.bind(self.socket_path)
        self.socket.setblocking(0)
        self.socket.listen(1)
        self.avg_read = ReadingSchema()
        self.read = 0
        self.mode = RunningMode.RUNNING_DB
        self.running = True
        print('Unix socket for reading live feed from application started')
    
    def set_mode(self, _mode):
        if isinstance(_mode, RunningMode):
            self.mode = _mode.value
        else:
            raise TypeError('Use enum RunningMode')

    def start(self):
        self.mode = RunningMode.RUNNING_DB
        while self.running:
            conn, addr = self.socket.accept()
            self.live_data_handler(connection=conn)
            appl_thread = threading.Thread(target=self.connection_handler, args=(conn,))
            appl_thread.daemon = True
            appl_thread.start()

    def stop(self):
        if self.socket:
            self.socket.close()
            os.remove(self.socket_path)
            print('Live data server stopped.')

    def connection_handler(self):
        while self.running:
            ready_to_read, _, _ = select.select([self.socket], [], [], 0)
            if ready_to_read:
                conn, addr = self.socket.accept()
                self.live_data_handler(conn)

    def live_data_handler(self, connection):
        try:
            live_data = connection.recv(1024)
        except OSError as e:
            print("Could not read")
        self.process_new_data(live_data.decode().strip())
        # time.sleep(1)

    def process_new_data(self, live_data):
        if self.mode is RunningMode.RUNNING_DB:
            self.process_in_db_mode(live_data)    

        if self.mode is RunningMode.RUNNING_LIVE:
            self.proces_in_live_mode(live_data)

    def process_in_db_mode(self, data):
        reading_schema = ReadingSchema()
        try:
            reading = reading_schema.loads(data)
            self.compute_avg_with_new(reading)
            self.read += 1
            if self.read >= TIMES_FOR_AVERAGE_BEFORE_DB_PUSH:
                # with current_app.app_context():
                db.session.add(self.avg_read)
                db.session.commit()
                self.read = 0
        except ValidationError as err:
            print(err.messages)

    def process_in_live_mode(self, data):
        self.process_in_db_mode(data)
        # socketio.emit('new_data', {'data': data})
       
    def compute_avg_with_new(self, new_reading):
        self.avg_read.lul =     self.avg_read.lul + new_reading.lul / 2
        self.avg_read.lur =     self.avg_read.lur + new_reading.lur / 2
        self.avg_read.ldl =     self.avg_read.ldl + new_reading.ldl / 2
        self.avg_read.ldr =     self.avg_read.ldr + new_reading.ldr / 2
        self.avg_read.panel =   self.avg_read.panel + new_reading.panel / 2
        self.avg_read.temp =    self.avg_read.temp + new_reading.temp / 2
        self.avg_read.hum =     self.avg_read.hum + new_reading.hum / 2
        self.avg_read.angle0X = self.avg_read.angle0X + new_reading.angle0X / 2
        self.avg_read.angle0Y = self.avg_read.angle0Y + new_reading.angle0Y / 2