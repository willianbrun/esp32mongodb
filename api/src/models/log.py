from pydantic import BaseModel
from datetime import datetime


class Log(BaseModel):
    temperature: float
