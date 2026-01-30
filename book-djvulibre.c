#include "estd.h"
#include <libdjvu/ddjvuapi.h>
#include "globals.h"
#include "book.h"
#include "book-djvu.h"


typedef struct DjvuLibre {
    ddjvu_document_t *doc;
    ddjvu_context_t *ctx;
    ddjvu_format_t *format;
} DjvuLibre;

#define djvu ((DjvuLibre*)lib)

Book *
djvu_book_open(const char *path) {
    unsigned int masks[4] = { 0xFF0000, 0xFF00, 0xFF, 0xFF000000};
    Book *book;
    ddjvu_message_t *msg;

    lib = emalloc(sizeof(DjvuLibre));
    djvu->ctx = ddjvu_context_create("view");
    djvu->doc = ddjvu_document_create_by_filename(djvu->ctx, path, 1);
    djvu->format = ddjvu_format_create(DDJVU_FORMAT_RGBMASK32, 4, masks);
    ddjvu_format_set_row_order(djvu->format, TRUE);
    ddjvu_message_wait(djvu->ctx);
    while((msg = ddjvu_message_peek(djvu->ctx)) && msg->m_any.tag != DDJVU_DOCINFO) {
        if(msg->m_any.tag == DDJVU_ERROR)
            goto err;
        ddjvu_message_pop(djvu->ctx);
    }
    if(ddjvu_document_decoding_error(djvu->doc))
        goto err;

    book = book_init(ddjvu_document_get_pagenum(djvu->doc));
    return book;

    err:
    free(djvu);
    return NULL;
}

Page *
djvu_page_load(Page *page, double max_w, double max_h) {
    int w, h;
    double r1, r2, r;
    char *bitmap;
    ddjvu_status_t status;
    ddjvu_pageinfo_t info;
    ddjvu_page_t *dpage;

    while((status = ddjvu_document_get_pageinfo(djvu->doc, page->n, &info)) < DDJVU_JOB_OK);

    if(status >= DDJVU_JOB_FAILED)
        return NULL;

    dpage = ddjvu_page_create_by_pageno(djvu->doc, page->n);
    if(!dpage)
        return NULL;

    while(!ddjvu_page_decoding_done(dpage));

    r1 = max_w / (double)info.width;
    r2 = max_h / (double)info.height;
    r = r1 < r2 ? r1 : r2;
    w = (int)(r * info.width);
    h = (int)(r * info.height);

    ddjvu_rect_t rrect = {0, 0, w, h};
    ddjvu_rect_t prect = {0, 0, w, h};

    bitmap = emalloc(w * h * 4);
    ddjvu_page_render(dpage, DDJVU_RENDER_COLOR, &prect, &rrect, djvu->format, w * 4, bitmap);

    ddjvu_page_release(dpage);
    page->bitmap = (unsigned char *)bitmap;
    page->w = w;
    page->h = h;
    page->linesize = w * 4;

    // reorder_argb_to(page, color_format);
    return page;
}
