from flask import Flask
from flask_restx import Api, Resource, fields

app = Flask(__name__)
api = Api(
    app,
    version="1.0",
    title="Book API",
    description="A simple Book API",
    doc="/docs",
    prefix="/api/v1",
)

ns = api.namespace("books", description="Book operations")

book_model = api.model(
    "Book",
    {
        "id": fields.Integer(readonly=True, description="Book ID"),
        "title": fields.String(required=True, description="Book title"),
        "author": fields.String(required=True, description="Book author"),
    },
)

books = [
    {"id": 1, "title": "The Great Gatsby", "author": "F. Scott Fitzgerald"},
    {"id": 2, "title": "To Kill a Mockingbird", "author": "Harper Lee"},
    {"id": 3, "title": "1984", "author": "George Orwell"},
]


@ns.route("/")
class BookList(Resource):
    @ns.doc("list_books")
    @ns.marshal_list_with(book_model)
    def get(self):
        return books

    @ns.doc("create_book")
    @ns.expect(book_model)
    @ns.marshal_with(book_model, code=201)
    def post(self):
        new_book = {
            "id": max([book["id"] for book in books], default=0) + 1,
            "title": api.payload.get("title"),
            "author": api.payload.get("author"),
        }
        books.append(new_book)
        return new_book, 201


@ns.route("/<int:book_id>")
@ns.param("book_id", "Book identifier")
@ns.response(404, "Book not found")
class Book(Resource):
    @ns.doc("get_book")
    @ns.marshal_with(book_model)
    def get(self, book_id):
        book = next((book for book in books if book["id"] == book_id), None)
        if book is None:
            api.abort(404, "Book not found")
        return book

    @ns.doc("update_book")
    @ns.expect(book_model)
    @ns.marshal_with(book_model)
    def put(self, book_id):
        book = next((book for book in books if book["id"] == book_id), None)
        if book is None:
            api.abort(404, "Book not found")

        book["title"] = api.payload.get("title", book["title"])
        book["author"] = api.payload.get("author", book["author"])
        return book

    @ns.doc("delete_book")
    @ns.response(204, "Book deleted")
    def delete(self, book_id):
        global books
        book = next((book for book in books if book["id"] == book_id), None)
        if book is None:
            api.abort(404, "Book not found")

        books = [book for book in books if book["id"] != book_id]
        return "", 204
