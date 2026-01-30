#ifndef VIEW_BOOK_H
#define VIEW_BOOK_H

typedef struct Page {
    unsigned char *bitmap;
    int w, h;
    int n;
    int linesize;
} Page;

typedef struct Book {
    struct Page *pages;
    int page_count;
} Book;

Book *book_open(const char *path);
Book *book_init(int page_count);

#endif
