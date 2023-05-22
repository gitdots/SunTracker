import sqlite3
import click
from flask import current_app, g


def get_database():
    if 'db' not in g:
        g.db = sqlite3.connect(
            current_app.config['DATABASE'],
            detect_types = sqlite3.PARSE_DECLTYPES
        )
        g.db.row_factory = sqlite3.Row
        return g.db
    
def close_database(e = None):
    db = g.pop('db', None)
    if db is not None:
        db.close()

def initialize_database():
    db = get_database()
    
    with current_app.open_resource('schema.sql') as tables:
        db.executescript(tables.read().decode('utf8'))

def initialize_app(app):
    app.teardown_appcontext(close_database)
    app.cli.add_command(initialize_database_command)
    
@click.command('init-db')
def initialize_database_command():
    #   clear the existing data and create new tables
    initialize_database()       
    click.echo('Database instatiated')
