from fastapi import FastAPI, HTTPException, APIRouter
from pydantic import BaseModel

app = FastAPI()
v1 = APIRouter(prefix="/api/v1")


# Pydantic models for request/response validation
class Book(BaseModel):
    id: int | None = None
    title: str
    author: str


books = [
    {"id": 1, "title": "The Great Gatsby", "author": "F. Scott Fitzgerald"},
    {"id": 2, "title": "To Kill a Mockingbird", "author": "Harper Lee"},
    {"id": 3, "title": "1984", "author": "George Orwell"},
]


# All resources: /api/v1/books, GET, POST
@v1.get("/books")
def get_books():
    return books


@v1.post("/books", status_code=201)
def create_book(book: Book):
    new_book = {
        "id": max([b["id"] for b in books], default=0) + 1,
        "title": book.title,
        "author": book.author,
    }
    books.append(new_book)
    return new_book


# Single resource: /api/v1/books/{book_id}, GET, PUT, DELETE
@v1.get("/books/{book_id}")
def get_book(book_id: int):
    book = next((book for book in books if book["id"] == book_id), None)
    if book is None:
        raise HTTPException(status_code=404, detail="Book not found")
    return book


@v1.put("/books/{book_id}")
def update_book(book_id: int, book: Book):
    existing_book = next((b for b in books if b["id"] == book_id), None)
    if existing_book is None:
        raise HTTPException(status_code=404, detail="Book not found")
    existing_book["title"] = book.title
    existing_book["author"] = book.author
    return existing_book


@v1.delete("/books/{book_id}", status_code=204)
def delete_book(book_id: int):
    global books
    book = next((b for b in books if b["id"] == book_id), None)
    if book is None:
        raise HTTPException(status_code=404, detail="Book not found")
    books = [b for b in books if b["id"] != book_id]
    return None

app.include_router(v1)
