import firebase_admin
from firebase_admin import credentials
from firebase_admin import firestore
import datetime
from fastapi import FastAPI
from .models.log import Log

app = FastAPI()

# Use a service account.
cred = credentials.Certificate("./api/temperature-monitor-key.json")
application = firebase_admin.initialize_app(cred)
db = firestore.client()


@app.post("/logs/")
async def new_log(log: Log):
    actual_datetime = datetime.datetime.now().astimezone()
    try:
        doc = {
            "temperature": log.temperature,
            "turned_alarm_on": log.turned_alarm_on,
            "datetime": actual_datetime,
        }

        db.collection("logs").add(doc)
        return log.temperature
    except:
        print(f"Erro ao salvar o log")
