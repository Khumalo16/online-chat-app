import os

from flask import Flask, render_template
from flask_socketio import SocketIO, emit, send

app = Flask(__name__)
app.config["SECRET_KEY"] = 'mysecret'
socketio = SocketIO(app)

@app.route("/")
def index():
    return render_template('flack/index.html')

@socketio.on("submit vote")
def vote(data):
    selection = data["selection"]
    print(selection)
    emit("announce vote", {"selection": selection}, broadcast=True)
if __name__ == '__main__':
    socketio.run(app)
    
@app.route("/registration")
def registration():
    return render_template('flack/registration.html')