from flask import Blueprint, jsonify, request
from webserver import database

device_blueprint = Blueprint('device_blueprint', __name__, url_prefix='/devices')

# read all devices
@device_blueprint.route('/', methods = ['GET', 'POST'])
def devices():
    if request.method == 'GET':
        get_req_message = 'Getting the devices from the database'
        try:
            db = database.get_database()
            c = db.execute("SELECT * FROM Devices")
            devices = c.fetchall()
            c.close()
            d = [tuple(device) for device in devices]
            return jsonify(
                message =  get_req_message,
                category = 'Success',
                data = d,
                status = 200
            )
        except Exception as e:
            return jsonify(
                message = 'Could not retrieve the devices from the database',
                category = 'Failure',
                data = str(e),
                status = 500
            )
    
    if request.method == 'POST':
        device = request.get_json()
        post_req_message = "Updating the database with new device"
        if not device:
            return jsonify(
                message = 'Invalid JSON data',
                category = 'Failure',
                data = '',
                status = 400
            )
        
        try:
            db = database.get_database()
            c = db.execute('INSERT INTO Devices (device_id, place) VALUES (?, ?)',
                (device['device_id'], device['place']))
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
            data = str(device),
            status = 200
        )

    
@device_blueprint.route('/devices/<int:id>', methods = ['PUT', 'DELETE'])
    
# Updates the specified device
def update_device(id):
    if request.method == 'PUT':
        update_req_message = 'Updating the device'
        data = request.get_json()
        if not data or 'place' not in data:
            return jsonify(
                message = 'Invalid JSON data',
                category = 'Failure',
                data = '',
                status = 400    
            )
        try:
            db = database.get_database()
            c = db.execute('UPDATE Devices SET place = ? WHERE device_id = ?', 
                (data['place'], id))
            db.commit()
            c.close()
        except Exception as e:
            return jsonify(
                message = update_req_message,
                category = 'Failure',
                data = str(e),
                status = 400    
            )
        return jsonify(
            message = update_req_message,
            category = 'Success',
            data = data,
            status = 200    
        )
    
    if request.method == 'DELETE':
        delete_req_message = 'Deleting the device'
        try:
            db = database.get_database()
            c = db.execute('DELETE FROM Devices WHERE device_id = ?', (id,))
            db.commit()
            c.close()
        except Exception as e:
            return jsonify(
                message = delete_req_message,
                category = 'Failure',
                data = str(e),
                status = 400
            )
        
        return jsonify(
            message = delete_req_message,
            category = 'Success',
            data = '',
            status = 200
        )