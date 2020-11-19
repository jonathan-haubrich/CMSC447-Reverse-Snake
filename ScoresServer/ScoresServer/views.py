"""
Routes and views for the flask application.
"""

from datetime import datetime
from flask import jsonify, request, render_template
from ScoresServer import app

import os

@app.route('/', methods=['GET'])
@app.route('/scores', methods=['GET','POST'])
def home():
    """Returns or saves scores depending on HTTP method"""
    if request.method == 'GET':
        return jsonify(open("scores.json").read())
    elif request.method == 'POST':
        print(request.data)
        print(request.json)
        with open("scores.json", "w") as scores:
            scores.write(request.json)
        return jsonify(success=True)
    return jsonify(success=False)
        