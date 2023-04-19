import os

from flask import Flask
from . import database

def create_app(test_config=None):

     # create and configure the app
    app = Flask(__name__, instance_relative_config=True)
    app.config.from_mapping(
        SECRET_KEY='dev',
        DATABASE=os.path.join(app.instance_path, 'server.sqlite'),
    )

    if test_config is None:
        # load the instance config, if it exists, when not testing
        app.config.from_pyfile('config.py', silent=True)
    else:
        # load the test config if passed in
        app.config.from_mapping(test_config)

    # ensure the instance folder exists
    try:
        os.makedirs(app.instance_path)
    except OSError:
        pass

    database.initialize_app(app)

    from .blueprints import device, date_summary, reading
    app.register_blueprint(device.device_bp)
    app.register_blueprint(date_summary.summary_bp)
    app.register_blueprint(reading.reading_bp)

    @app.route('/')
    def index():
        return 'Hello world'

    return app