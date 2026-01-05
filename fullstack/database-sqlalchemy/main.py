from sqlalchemy import create_engine, String
from sqlalchemy.orm import DeclarativeBase, Mapped, mapped_column
from typing import Optional

db = create_engine("sqlite:///database.db")

class Base(DeclarativeBase):
    pass

class Book(Base):
    __tablename__ = 'books'

    id: Mapped[int] = mapped_column(primary_key=True)
    author: Mapped[Optional[str]] = mapped_column(String(50))
    title: Mapped[str] = mapped_column(String(120))

    def __repr__(self) -> str:
        return f"Book(id={self.id!r}, author={self.author!r}, title={self.title!r})"

Base.metadata.create_all(db)
