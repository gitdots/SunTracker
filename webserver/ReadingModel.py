from .app import db
from marshmallow_sqlalchemy import SQLAlchemyAutoSchema
from datetime import datetime


class Readings (db.Model):
    id = db.Column(db.Integer, primary_key=True)
    read_time = db.Column(db.DateTime, default=datetime.utcnow())
    lul = db.Column(db.Integer)
    lur = db.Column(db.Integer)
    ldl = db.Column(db.Integer)
    ldr = db.Column(db.Integer)
    panel = db.Column(db.Float(2))
    temp = db.Column(db.Float(2))
    hum = db.Column(db.Float(2))
    angle0X = db.Column(db.Integer)
    angle0Y = db.Column(db.Integer)

    def __repr__(self):
        return '<Reading %r>' % self.id

class ReadingSchema(SQLAlchemyAutoSchema):
    class Meta:
        model = Readings
        load_instance = True