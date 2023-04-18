from flask import Blueprint, jsonify, request
from webserver import database

summary_bp = Blueprint('date_summary_bp', __name__, url_prefix='/summaries')

@summary_bp.route('/', methods = ['GET', 'POST'])
def summaries():
    if request.method == 'GET':
        get_req_message = 'Getting all the data'
        try:
            db = database.get_database()
            c = db.execute('SELECT * FROM DateSummary')
            summaries = c.fetchall()
            c.close()
            summ = [tuple(summary) for summary in summaries]
            return jsonify(
                message = get_req_message,
                category = 'Success',
                data = summ,
                status = 200
            )
        except Exception as e:
            return jsonify(
                message = 'Could not retrieve summaries',
                category = 'Failure',
                data = str(e),
                status = 500
            )

    if request.method == 'POST':
        post_req_message = 'Adding new summary'
        summary = request.get_json()
        if not summary:
            return jsonify(
                message = 'Invalid JSON data',
                category = 'Failure',
                data = '',
                status = 400
            )
        try:
            db = database.get_database()
            querry_str = 'INSERT INTO DateSummary (summary_date, device_id, min_temp, max_temp, min_hum, max_hum) VALUES (?, ?, ?, ?, ?, ?)'
            c = db.execute(querry_str, (summary['summary_date'], summary['device_id'],
                            summary['min_temp'], summary['max_temp'], summary['min_hum'], summary['max_hum']))
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
            data = str(summary),
            status = 200
        )