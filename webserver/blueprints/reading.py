from flask import Blueprint, jsonify, request
from webserver import database

reading_bp = Blueprint('device_blueprint', __name__, url_prefix='/readings')

@reading_bp.route('/', methods=['GET', 'POST'])
def get_put_today_readings():
    if request.method == 'GET':
        get_req_message = 'Getting the readings for today'
        try:
            db = database.get_database()
            c = db.execute('SELECT * FROM Readings WHERE DATE(read_time)=DATE("now", "localtime")')
            readings = c.fetchall()
            c.close()
            r = [tuple(reading) for reading in readings]
            return jsonify(
                message = get_req_message,
                category = 'Success',
                data = r,
                status = 200
            )
        except Exception as e:
            return jsonify(
                message = 'Could not get the readings for today',
                category = 'Failure',
                data = str(e),
                status = 500
            )
    if request.method == 'POST':
        post_req_message = 'Posting latest reading'
        read = request.get_json()
        if not read:
            return jsonify(
                message = 'Imvalid JSON data',
                category = 'Failure',
                data = '',
                status = 400
            )

        try:
            db = database.get_database()
            query = """INSERT INTO Readings (read_time, device_id, lul, lur, ldl, ldr,
            temp, hum, angle0x, angle0y) VALUES (CURRENT_TIMESTAMP, ?, ?, ?, ?, ?, ?, ?, ?, ?)"""
            c = db.execute(query, (read['device_id'], read['lul'],
                                   read['lur'], read['ldl'], read['ldr'], read['temp'],
                                   read['hum'], read['angle0x'], read['angle0y']))
            db.commit()
            c.close()
        except Exception as e:
            return jsonify(
                message = post_req_message,
                category = 'Failure',
                data = str(e),
                status = 500
            )
        return jsonify(
            message = post_req_message,
            category = 'Success',
            data = str(read),
            status = 200
        )

@reading_bp.route('/<string:year>-<string:month>-<string:day>/', methods = ['GET'])
def get_all_on_specific_date(year, month, day):
    if request.method == 'GET':
        get_req_message = 'Getting readings on a specific date'
        try:
            db = database.get_database()
            date = f'{year}-{month}-{day}'
            c = db.execute(f'SELECT * FROM Readings WHERE DATE(read_time) = "{date}"')
            data_rows = c.fetchall()
            c.close()
            d = [tuple(data) for data in data_rows]
            return jsonify(
                message = get_req_message,
                category = 'Success',
                data = d,
                status = 200
            )
        except Exception as e:
            return jsonify(
                message = 'Could not retrieve the data for the specified date',
                category = 'Failure',
                data = str(e),
                status = 500
            )