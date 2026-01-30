#include <stdlib.h>
#include <string.h>
#include "estd.h"
#include "globals.h"
#include "book.h"
#include "magic.h"

#include "book-pdf.h"
#include "book-djvu.h"
#include "book-ps.h"

Book *
book_init(int page_count) {
    Book *book;

    book = emalloc(sizeof(Book));
    book->page_count = page_count;
    book->pages = emalloc(page_count * sizeof(Page));
    for(int i = 0; i < page_count; i++) {
        book->pages[i].n = i;
        book->pages[i].bitmap = NULL;
    }
    return book;
}

Book *
book_open(const char *path) {
    Book *book;
    const char *format;

    format = get_mimetype(path);

    if(strcmp(format, "application/pdf") == 0 || strncmp(format, "application/epub", 16) == 0) {
        book = pdf_book_open(path);
        page_load = pdf_page_load;
        dump_link = pdf_dump_link;
    } else if(strcmp(format, "image/vnd.djvu") == 0) {
        book = djvu_book_open(path);
        page_load = djvu_page_load;
        dump_link = NULL;
    } else if(strcmp(format, "application/postscript") == 0) {
        book = ps_book_open(path);
        dump_link = NULL;
    } else {
        diearg("view", path, "File format not supported");
    }

    return book;
}
