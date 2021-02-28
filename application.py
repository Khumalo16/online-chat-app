import os

from flask import Flask, render_template, flash;
from flask_socketio import SocketIO, emit, send

app = Flask(__name__)
app.config["SECRET_KEY"] = 'mysecret'
socketio = SocketIO(app)
users = []
channel = []

@app.route("/")
def index():
    return render_template('flack/index.html')


@app.route("/registration")
def registration():
    return render_template('flack/registration.html')

@socketio.on("username")
def username(data):


    if data["user"]:
        data["user"] = data["user"].capitalize()
    if not users and data["user"]:
        
        users.append(data['Home'])
    if len(users) > 0:
      
        for user in users:
            if user == data["user"]:
                data["user"] = "1"
    
    if data["user"] and data["user"] != "1":
        users.append(data["user"])         
    print(users, "this is not zero")
    #del users[:]
    print("this list is empyt", data["user"])
    emit("username", {"user": data["user"]}, broadcast=True)

@socketio.on("message sent")
def message(data):
    print("the message was received...")
    emit("message back",{"msg": data["msg"]}, broadcast=True)

@socketio.on("channel name")
def createChannel(data):
    channel.append(data["li"])
    emit("channel name", {"li":data["li"]}, broadcast=True)

if __name__ == '__main__':
    socketio.run(app)

