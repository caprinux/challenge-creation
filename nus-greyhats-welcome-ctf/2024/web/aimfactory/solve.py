import requests
import json

r = requests.post("http://localhost:32902/submit_score", json={"score":1000})
print(json.loads(r.text)['flag'])
