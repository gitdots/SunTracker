import board
import adafruit_dht
import argparse
import socket
import os
import time
import threading

REQUEST_DATA = '1'
REQUEST_CLOSE = '2' 

parser = argparse.ArgumentParser(description='Read value from dht22 sensor')
parser.add_argument('pin', help='The pin of the sensor')
parser.add_argument('socket_name', help='Socket to write the data into')
args = parser.parse_args()

pin = int(args.pin)
address = args.socket_name
if os.path.exists(address):
    os.remove(address)

sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
sock.bind(address)
sock.listen(1)

formatted = getattr(board, "D{}".format(pin))
dhtDevice = adafruit_dht.DHT22(formatted, use_pulseio=False)

while True:
    connection, addr = sock.accept()
    print('Waiting for connection')
    request = connection.recv(1024).decode()
    if request == REQUEST_DATA:
        print('Request to send data')
        # Get data and write it into the socket
        last_temp = None
        last_hum = None
        while last_temp is None and last_hum is None:
            try:
                last_temp = dhtDevice.temperature
                last_hum = dhtDevice.humidity
                data = f'{last_temp};{last_hum}'
                connection.send(data.encode())
                print(f'Data sent:{data}')
            except RuntimeError as error:
                print(error.args[0])
                time.sleep(2)
            except Exception as error:
                dhtDevice.exit()
                print(error)

    if request == REQUEST_CLOSE:
        print('Request to cut off connections')
        connection.close()
        sock.close()
        os.unlink(address)
        break; # end script
