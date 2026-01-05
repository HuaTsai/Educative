from flask import Flask, request
from flask_restful import Resource, Api

app = Flask(__name__)
app.url_map.strict_slashes = False

api = Api(app, prefix="/api/v1")

books = [
    {"id": 1, "title": "The Great Gatsby", "author": "F. Scott Fitzgerald"},
    {"id": 2, "title": "To Kill a Mockingbird", "author": "Harper Lee"},
    {"id": 3, "title": "1984", "author": "George Orwell"},
]


# All resources: /books, GET, POST
class BookList(Resource):
    def get(self):
        return books

    def post(self):
        data = request.get_json()
        new_book = {
            "id": len(books) + 1,
            "title": data.get("title"),
            "author": data.get("author"),
        }
        books.append(new_book)
        return new_book, 201


# Single resource: /books/<book_id>, GET, PUT, DELETE
class Book(Resource):
    def get(self, book_id):
        book = next((book for book in books if book["id"] == book_id), None)
        if book is None:
            return {"message": "Book not found"}, 404
        return book

    def put(self, book_id):
        book = next((book for book in books if book["id"] == book_id), None)
        if book is None:
            return {"message": "Book not found"}, 404
        data = request.get_json()
        book["title"] = data.get("title", book["title"])
        book["author"] = data.get("author", book["author"])
        return book

    def delete(self, book_id):
        global books
        books = [book for book in books if book["id"] != book_id]
        return "", 204


api.add_resource(BookList, "/books")
api.add_resource(Book, "/books/<int:book_id>")
