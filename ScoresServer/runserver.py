"""
This script runs the ScoresServer application using a development server.
"""

from os import environ
from ScoresServer import app



if __name__ == '__main__':
    HOST = environ.get('SERVER_HOST', '0.0.0.0')
    try:
        PORT = int(environ.get('SERVER_PORT', '55443'))
    except ValueError:
        PORT = 55443
    context = ('certs/fullchain.pem', 'certs/privkey.pem')
    app.run(HOST, PORT, ssl_context=context)