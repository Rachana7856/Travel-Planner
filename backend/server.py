from flask import Flask, request, jsonify
from flask_cors import CORS
import subprocess
import json
import os

app = Flask(__name__)
CORS(app)

@app.route('/find_route', methods=['POST'])
def find_route():
    try:
        data = request.get_json()
        if not data:
            return jsonify({'error': 'No JSON received'}), 400

        source = data.get('source', '').strip()
        destination = data.get('destination', '').strip()

        if not source or not destination:
            return jsonify({'error': 'Source and destination are required'}), 400

        exe_path = os.path.abspath('./pathfinder.exe')
        if not os.path.exists(exe_path):
            return jsonify({'error': 'pathfinder.exe not found'}), 500

        result = subprocess.run(
            [exe_path, source, destination],
            capture_output=True,
            text=True
        )

        if result.returncode != 0:
            return jsonify({'error': 'Pathfinder failed', 'details': result.stderr}), 500

        output = result.stdout.strip()
        return jsonify(json.loads(output))

    except Exception as e:
        return jsonify({'error': str(e)}), 500

if __name__ == '__main__':
    app.run(debug=True)
