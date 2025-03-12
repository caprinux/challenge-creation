from flask import Flask, render_template, request, jsonify
import time
import secrets

app = Flask(__name__)
app.secret_key = secrets.token_hex(32)

# Load the flag from a file
def get_flag():
    with open("flag.txt", "r") as f:
        return f.read().strip()

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/submit_score', methods=['POST'])
def submit_score():
    score = int(request.json.get('score', 0))
    if score >= 1000:
        flag = get_flag()
        return jsonify({'message': 'Congratulations! You earned the flag.', 'flag': flag})
    else:
        return jsonify({'message': 'Keep trying! You need 1000 points to get the flag.'})
