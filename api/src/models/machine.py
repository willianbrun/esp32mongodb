from datetime import datetime
from pydantic import BaseModel
from typing import Optional

class Machine(BaseModel):
    id: Optional[int]
    name: str
    status: str
    latitude: float
    longitude: float
    last_updated: Optional[datetime]
    created: Optional[datetime]