#ifndef VIEW_MUPDF_H
#define VIEW_MUPDF_H

#include "book.h"

Book *pdf_book_open(const char *path);
Page *pdf_page_load(Page *page, double max_width, double max_height);
void pdf_dump_link(Page *page);

#endif
