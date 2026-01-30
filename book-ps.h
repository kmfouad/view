#ifndef VIEW_PS_H
#define VIEW_PS_H

#include "book.h"

Book *ps_book_open(const char *path);
Page *ps_page_load(Page *page, double max_w, double max_h);

#endif
