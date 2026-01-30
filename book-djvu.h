#ifndef VIEW_DJVU_H
#define VIEW_DJVU_H

#include "book.h"

Book *djvu_book_open(const char *path);
Page *djvu_page_load(Page *page, double max_w, double max_h);

#endif
