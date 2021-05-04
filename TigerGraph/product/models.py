from flask_sqlalchemy import  SQLAlchemy

db = SQLAlchemy()

class Employee(db.Model):
    __tablename__ ="employee"
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String, nullable=False)
    surname = db.Column(db.String, nullable=False)

class email(db.Model):
    __tablename__ ="email"
    id = db.Column(db.Integer, primary_key=True)
    email = db.Column(db.String, nullable=False)
    email_id = db.Column(db.Integer, db.ForeignKey("employee.id"), nullable=False)



