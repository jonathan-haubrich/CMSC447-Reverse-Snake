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
        new_scores = request.json
        with open("scores.json", "r") as scores:
            try:
                leaderboard = json.load(scores)
                for initial, score in leaderboard.items():
                    if new_scores.get(initial):
                        if new_scores[initial] < score:
                            new_scores[initial] = score
                    else:
                        new_scores[initial] = score
            except Exception as e:
                print(e)
        with open("scores.json", "w") as scores:
            scores.write(json.dumps(new_scores))
        return jsonify(success=True)
    return jsonify(success=False)
        