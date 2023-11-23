import mariadb
import datetime
import sys
from fastapi import FastAPI
from .models.log import Log

app = FastAPI()

# External: http://177.67.253.69:5080/phpmyadmin/
# Internal: http://10.0.235.199/phpmyadmin/

# Connect to MariaDB Platform
try:
    conn = mariadb.connect(
        user="178810",
        password="178810",
        # Internal
        # host="10.0.235.199",
        # port=3306,
        # External
        host="177.67.253.69",
        port=53306,
        database="178810",
    )
except mariadb.Error as e:
    print(f"Error connecting to MariaDB Platform: {e}")
    sys.exit(1)

# Get Cursor
cur = conn.cursor()


@app.post("/logs/")
async def new_log(log: Log):
    actual_datetime = datetime.datetime.now().astimezone()
    try:
        cur.execute(
            "INSERT INTO logs (temperature, turned_alarm_on, datetime) VALUES (?, ?, ?)",
            (log.temperature, log.turned_alarm_on, actual_datetime),
        )
        return log.temperature
    except mariadb.Error as e:
        print(f"Erro ao salvar o log: {e}")
