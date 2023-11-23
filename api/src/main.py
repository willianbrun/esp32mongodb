import mariadb
import datetime
import sys
from fastapi import FastAPI
from .models.log import Log

app = FastAPI()

# Connect to MariaDB Platform
try:
    conn = mariadb.connect(
        user="178810",
        password="178810",
        # host="10.0.235.199",
        # port=3306,
        host="177.67.253.69",
        port=53306,
        database="178810",
    )
except mariadb.Error as e:
    print(f"Error connecting to MariaDB Platform: {e}")
    sys.exit(1)

# Get Cursor
cur = conn.cursor()


@app.get("/logs/")
async def read_logs():
    logs = cur.execute("SELECT * FROM logs")
    # cur.fetchAll
    return logs


@app.post("/logs/")
async def new_log(log: Log):
    try:
        actual_datetime = datetime.datetime.now().astimezone()
        cur.execute(
            "INSERT INTO logs (temperature, datetime) VALUES (?, ?)",
            (log.temperature, actual_datetime),
        )
        return log.temperature
    except mariadb.Error as e:
        print(f"Erro ao salvar o log: {e}")
