from sqlalchemy import Boolean, Column, ForeignKey, Integer, String
from sqlalchemy.orm import relationship

from .database import Base


class User(Base):
    __tablename__ = "users"

    id = Column(Integer, primary_key=True, index=True)
    name = Column(String)

    collects = relationship("Collect", back_populates="owner")


class Collect(Base):
    __tablename__ = "collects"

    id = Column(Integer, primary_key=True, index=True)
    temperature = Column(Integer)
    is_door_open = Column(Boolean)
    owner_id = Column(Integer, ForeignKey("users.id"))

    owner = relationship("User", back_populates="items")


#https://fastapi.tiangolo.com/tutorial/sql-databases/