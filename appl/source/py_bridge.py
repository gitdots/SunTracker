import argparse
import socket
import threading as threading
from urllib import request
import board
import adafruit_dht
import time
import os
import re

import RPi.GPIO as GPIO
from time import sleep

sensor = adafruit_dht.DHT22

class SensorReader:
    def __init__(self, pin):
        self.pin = pin
        self.temperature = None
        self.humidity = None
        self.formatted = getattr(board, "D{}".format(pin))
        self.dhtDevice = adafruit_dht.DHT22(self.formatted, use_pulseio=False)

    def get_latest_data(self):
        try:
            self.temperature = (self.dhtDevice).temperature
            self.humidity = (self.dhtDevice).humidity
        except Exception as e:
            self.dhtDevice.exit()
        if self.temperature is not None and self.humidity is not None: #and self.temperature != 0 and self.humidity != 0:
            # print(f"[TH] GOT {self.temperature}; {self.humidity}")
            return f"{self.temperature};{self.humidity}"
        else:
            self.temperature = None
            self.humidity = None
            # print(f"[TH] GOT ELSE {self.temperature}; {self.humidity}")
            return "999.0;999.0"
        
class Servomotor:
    def __init__(self, pin):
        self.pin = pin
        self.servo_min = 0
        self.servo_max = 180
        self.servo_freq = 50
        self.servo = None
        self.angle = 0
        
    def setup(self):
        GPIO.setup(self.pin, GPIO.OUT)

        self.servo = GPIO.PWM(self.pin, self.servo_freq)
        self.servo.start(0)

    def move(self, angle_new):
        tmp_angle = self.angle + angle_new
        if self.angle != tmp_angle and tmp_angle < self.servo_max and tmp_angle > self.servo_min:
            self.angle = tmp_angle
            duty_cycle = self.angle / 18.0 + 2
            self.servo.ChangeDutyCycle(duty_cycle)
            time.sleep(.2)
            self.servo.ChangeDutyCycle(0)

class SocketServer:
    def __init__(self, socket_file_path, sensor_reader):
        self.socket_file_path = socket_file_path
        self.sensor_reader = sensor_reader
        self.server_socket = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        if os.path.exists(socket_file_path):
            os.remove(socket_file_path)
        self.server_socket.settimeout(None)
        self.server_socket.bind(socket_file_path)
        self.server_socket.listen(1)
        self.running = True
        self.servo_pattern = r"{(-?\d+);(-?\d+)}"

    def handle_client(self, conn):
        while self.running:
            data = conn.recv(1024)
            if not data:
                break

            request_code = data.decode().strip()
            got_servo = re.findall(self.servo_pattern, request_code)
            if got_servo:
                for angle in got_servo:
                    anglex, angley = angle
                servox.move(int(anglex))
                servoy.move(int(angley))
                response = f'{servox.angle};{servoy.angle}'
                conn.send(response.encode())

            if request_code == 'th':
                response = self.sensor_reader.get_latest_data()
                conn.send(response.encode())
            elif request_code == '2':
                self.running = False
                response = 'Closing connection'
                self.server_socket.close()
                break
            else:
                sleep(.2)
        
        GPIO.cleanup()
        conn.close()

    def start(self):
        if self.running:
            conn, addr = self.server_socket.accept()
            self.server_socket.settimeout(0.2)
            client_thread = threading.Thread(target=self.handle_client, args=(conn,))
            client_thread.start()


if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("pinTh", type=int, help="GPIO pin number of DHT22 sensor")
    parser.add_argument("pinx", type=int, help="GPIO pin number of horizontal servomotor")
    parser.add_argument("piny", type=int, help="GPIO pin number of vertical servomotor")
    parser.add_argument("socket_name", help="Name of Unix socket to create")
    args = parser.parse_args()

    GPIO.setmode(GPIO.BCM)

    servox = Servomotor(args.pinx)
    servox.setup()
    servox.move(90)
    servoy = Servomotor(args.piny)
    servoy.setup()
    servoy.move(90)

    sensor_reader = SensorReader(args.pinTh)

    server = SocketServer(args.socket_name, sensor_reader)
    server.start()
