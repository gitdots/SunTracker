from flask import Blueprint, jsonify, request
from webserver import database

reading_bp = Blueprint('device_blueprint', __name__, url_prefix='/readings')

@reading_bp.route('/', methods=['GET'])
def get_put_today_readings():
    if request.method == 'GET':
        get_req_message = 'Getting all the readings'
        try:
            db = database.get_database()
            c = db.execute('SELECT * FROM Readings')
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
                message = 'Could not get all the',
                category = 'Failure',
                data = str(e),
                status = 500
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
        