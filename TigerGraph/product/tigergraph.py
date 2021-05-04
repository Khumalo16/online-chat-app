import requests

from flask import Flask, render_template, request, session

app = Flask(__name__)

@app.route('/')
def index():
	return "RUNNIGN IN THE DOCKER!!!  why not changing"

if __name__ == '__main__':
	app.run(host='0.0.0.0',port=5000,debug=True)

