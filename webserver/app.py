import os
import socket
import select

from datetime import datetime, timedelta, date
import time

from threading import Thread, Event
import queue

from flask import Flask, jsonify, render_template, request
from flask_sqlalchemy import SQLAlchemy
from flask_marshmallow import Marshmallow
import json

app = Flask(__name__)

current_dir = os.path.dirname(os.path.abspath(__file__))
DB_PATH_REL = os.path.join(current_dir, '..', 'database', 'db')
DB_PATH_ABS = database_path = os.path.abspath(DB_PATH_REL)
print(DB_PATH_ABS)
app.config['SQLALCHEMY_DATABASE_URI'] = f'sqlite:///{DB_PATH_ABS}'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)
marsh = Marshmallow(app)

SOCKET_PATH = os.path.join('appl2flask.socket')
new_data_queue = queue.Queue()
data_available = Event()
client = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
client.connect(SOCKET_PATH)
latest_data = None

class Readings (db.Model):
    id = db.Column(db.Integer, primary_key=True, autoincrement=True)
    read_time = db.Column(db.DateTime)
    lul = db.Column(db.Integer)
    lur = db.Column(db.Integer)
    ldl = db.Column(db.Integer)
    ldr = db.Column(db.Integer)
    temp = db.Column(db.Float(2))
    hum = db.Column(db.Float(2))
    angle0X = db.Column(db.Integer)
    angle0Y = db.Column(db.Integer)
    mW = db.Column(db.Float(2))
    mA = db.Column(db.Float(2))
    v = db.Column(db.Float(2))

    def __repr__(self):
        return f"time:{self.read_time},lul:{self.lul},lur:{self.lur},ldl:{self.ldl},ldr:{self.ldr},temp:{self.temp},hum:{self.hum},angle0X:{self.angle0X},angle0Y:{self.angle0Y}, mW:{self.mW}, mA:{self.mA},v:{self.v}"

    def to_dict(self):
        return {c.name: getattr(self, c.name) for c in self.__table__.columns}

class ReadingSchema(marsh.SQLAlchemyAutoSchema):
    class Meta:
        model = Readings
        load_instance = True

def get2_decimals(number):
    if isinstance(number, float):
        dec2_number = round(number, 2)
        return dec2_number
    else:
        return number
    
def read_new_data_thread():
    while True:
        has_new_data, _, _ = select.select([client], [], [])
        new_data = client.recv(1024).decode()
        new_data_queue.put(new_data)
        data_available.set()
        # print(f'[read_new_data_thread] Received {new_data}')
        time.sleep(1)

def handle_new_data_thread():
    global latest_data
    latest_commit_time = datetime.now()
    temp_reading = None
    keys = ['anglex', 'angley', 'busVoltage_V', 'current_mA', 'power_mW', 'shuntVoltage_mV', 'hum', 'temp', 'ldl', 'ldr', 'lul', 'lur']
    empty_data_fields_total = {key : None for key in keys}
    for key in empty_data_fields_total:
        empty_data_fields_total[key] = None
    data_fields_total = empty_data_fields_total
    first_sample = True

    while True:
        try:
            data_available.wait()
            data = new_data_queue.get(block=False)
            latest_data = data
            data_dict = json.loads(latest_data)
            data_available.clear()

            if first_sample:
                for key, value in data_dict.items():
                    data_fields_total[key] = get2_decimals(value)
                first_sample = False
                # print(f'data_fields_total: {data_fields_total}')
            else:
                for key, value in data_dict.items():
                    data_fields_total[key] = (data_fields_total[key] + get2_decimals(value)) / 2

            # print('[handle_new_data_thread] Checking_time...')
            if datetime.now() - latest_commit_time >= timedelta(minutes=15):
                with app.app_context():
                    # print('[handle_new_data_thread] Time_elapsed')
                    temp_reading = Readings(
                        read_time = datetime.utcnow(),
                        lul = data_fields_total.get('lul'),
                        lur = data_fields_total.get('lur'),
                        ldl = data_fields_total.get('ldl'),
                        ldr = data_fields_total.get('ldr'),
                        temp = data_fields_total.get('temp'),
                        hum = data_fields_total.get('hum'),
                        angle0X = data_fields_total.get('anglex'),
                        angle0Y = data_fields_total.get('angley'),
                        mW = data_fields_total.get('power_mW'),
                        mA = data_fields_total.get('current_mA'),
                        v = data_fields_total.get('busVoltage_V') 
                    )

                    print(f'Pushing into db: {temp_reading}')
                    db.session.add(temp_reading)
                    db.session.commit()
                    first_sample = True
                    data_fields_total = empty_data_fields_total
                    latest_commit_time = datetime.now() # Update the last commit time
                    # readings = Readings.query.all()
                    
                    # print('[handle_new_data_thread] Readings Table: ')                    
                    # for reading in readings:
                    #     print(f'[handle_new_data_thread] Readings Table: {reading}')

            time.sleep(0.1)
        except KeyError as e:
            print(f'Key {e} not found in new data')            
        except:
            time.sleep(0.1)

# FOR TESTING
from faker import Faker
import random
@app.route('/mock_table')
def mock_table():
    fake = Faker()
    start_ = datetime(2023, 5, 29)
    end_ = datetime(2023, 5, 31)
    for _ in range(50):
        mock_reading = Readings(
            read_time = fake.date_time_between(start_date=start_, end_date=end_),
            lul = random.randint(0, 1024),
            lur = random.randint(0, 1024),
            ldl = random.randint(0, 1024),
            ldr = random.randint(0, 1024),
            temp = random.uniform(-10.0, 40.0),
            hum = random.uniform(0.0, 100.0),
            angle0X = random.randint(0, 180),
            angle0Y = random.randint(0, 180),
            mW = random.uniform(0.0, 1000.0),
            mA = random.uniform(0.0, 1000.0),
            v = random.uniform(0.0, 5.0)
        )
        db.session.add(mock_reading)
        db.session.commit()

    response = [reading.to_dict() for reading in Readings.query.order_by(Readings.read_time.desc()).all()]
    return jsonify(response)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/get_data')
def get_data():
    return jsonify({'data': latest_data})

@app.route('/statistics')
def statistics():
    return render_template('stats.html')

@app.route('/get_date_data')
def get_date_data():
    request_type = request.args.get('type', default=None, type=str)

    if request_type == 'day':
        specified_date = request.args.get('day', default=None, type=str)
        print(f'required data from {specified_date}')
        
        required_data = Readings.query.filter(db.func.date(Readings.read_time) == specified_date).order_by(Readings.read_time.asc()).all()
        print(required_data)
        results = []
        for item in required_data:
            res = {
                'id': item.id,
                'timestamp': item.read_time.strftime('%H:%M'),
                'power': item.mW,
                'temp': item.temp,
                'hum': item.hum
            }
            results.append(res)
        print(f'{specified_date}:\n {results}')

        return results


if __name__ == '__main__':
    with app.app_context():
        db.create_all()
    
    # try:
    new_data_thread = Thread(target=read_new_data_thread)
    handler_thread = Thread(target=handle_new_data_thread)
    new_data_thread.start()
    handler_thread.start()
    # except KeyboardInterrupt as e:
    #     print('Requested to shut down')
    #     new_data_thread.stop().join()
    #     handle_new_data_thread.stop().join()
    

    app.run( host='192.168.23.21', port=5000, threaded=True)
