"""
Routes and views for the flask application.
"""

from datetime import datetime
from flask import json, jsonify, request, render_template
from ScoresServer import app

import os

@app.route('/', methods=['GET'])
@app.route('/scores', methods=['GET','POST'])
def home():
    """Returns or saves scores depending on HTTP method"""
    if request.method == 'GET':
        return jsonify(open("scores.json").read())
    elif request.method == 'POST':
        with open("scores.json", "w") as scores:
            scores.write(json.dumps(request.json))
        return jsonify(success=True)
    return jsonify(success=False)
        