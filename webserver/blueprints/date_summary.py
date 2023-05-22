from flask import Blueprint, jsonify, request
from webserver import database

summary_bp = Blueprint('date_summary_bp', __name__, url_prefix='/summaries')

@summary_bp.route('/', methods = ['GET'])
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
        