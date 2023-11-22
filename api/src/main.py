from fastapi import FastAPI
from .models.models import DataModel
from .data.dataModels import dataModel_list #come from db


app = FastAPI()

@app.get("/")
async def root():
    return {"message": "Integração ESP 32 to Maria DB"}

@app.get("/dataModels/")
async def read_dataModel():
    return dataModel_list

@app.get("/dataModels/{dataModel_id}")
async def read_dataModel(dataModel_id: int):
    return (dataModel for dataModel in dataModel_list if dataModel.id == dataModel_id)

@app.post("/dataModels/")
async def create_dataModel(dataModel: DataModel):
    dataModel_list.append(dataModel)
    return dataModel

@app.put("/dataModels/{dataModel_id}")
async def update_dataModel(dataModel_id: int, dataModel: DataModel):
    for m in dataModel_list:
        if m.id == dataModel_id:
            m.name = dataModel.name
            m.status = dataModel.status
            m.latitude = dataModel.latitude
            m.longitude = dataModel.longitude
            m.last_updated = dataModel.last_updated
            m.created = dataModel.created
    return (dataModel for dataModel in dataModel_list if dataModel.id == dataModel_id)

@app.patch("/dataModels/{dataModel_id}")
async def delete_dataModel(dataModel_id: int):
    for m in dataModel_list:
        if m.id == dataModel_id:
            dataModel_list.remove(m)
    return []

