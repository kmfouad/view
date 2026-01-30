#include <mupdf/fitz.h>
#include "estd.h"

#include "magic.h"
#include "globals.h"
#include "book-pdf.h"
#include "util.h"

typedef struct Mupdf {
    fz_context *ctx;
    fz_document *doc;
    fz_matrix ctm;
} Mupdf;

#define mupdf ((Mupdf*)lib)

void
reorder_argb_to(Page *pg, int bytes, int color_format) {
    enum { R, G, B, A };
    unsigned char *new_bitmap, *r, *p;
    size_t size = pg->h * pg->w;

    new_bitmap = emalloc(size * 4);
    p = new_bitmap;
    r = pg->bitmap;
    switch(color_format) {
    case BGRA:
        for(size_t i = 0; i < size; i++, r+= bytes)
            *p++ = r[B], *p++ = r[G], *p++ = r[R], *p++ = 0;
        break;
    default:
        die(PROGNAME, "unsupported color format");
    }
    free(pg->bitmap);
    pg->bitmap = new_bitmap;
}

Book *
pdf_book_open(const char *path) {
    Book *book;
    int n;

    lib = emalloc(sizeof(Mupdf));
    mupdf->ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    fz_register_document_handlers(mupdf->ctx);

    // annoying mupdf error messages
    //fclose(stderr);
    mupdf->doc = fz_open_document(mupdf->ctx, path);

    n = fz_count_pages(mupdf->ctx, mupdf->doc);
    book = book_init(n);
    return book;
}

Page *
pdf_page_load(Page *page, double max_w, double max_h) {
    double w, h;
    double r1, r2, r;
    fz_rect bounds;
    fz_pixmap *pixmap;
    fz_page *mu_page;

    mu_page = fz_load_page(mupdf->ctx, mupdf->doc, page->n);
    if(!mu_page)
        return NULL;
    bounds = fz_bound_page(mupdf->ctx, mu_page);
    w = bounds.x1 - bounds.x0;
    h = bounds.y1 - bounds.y0;
    r1 = max_w / w;
    r2 = max_h / h;
    r = r1 < r2 ? r1 : r2;

    mupdf->ctm = fz_scale((float)r, (float)r);
    pixmap = fz_new_pixmap_from_page(mupdf->ctx, mu_page, mupdf->ctm, fz_device_rgb(mupdf->ctx), 0);
    if(!pixmap)
        return NULL;
    page->bitmap = pixmap->samples;
    page->w = pixmap->w;
    page->h = pixmap->h;
    page->linesize = pixmap->w * 4;
    reorder_argb_to(page, pixmap->n, color_format);
    return page;
}

void
pdf_dump_link(Page *page) {
    fz_link *link = fz_load_links(mupdf->ctx, fz_load_page(mupdf->ctx, mupdf->doc, page->n));
    FILE *f = fopen("/tmp/LINKS", "w");
    if(!f)
        return;
    for(; link != NULL; link = link->next)
        if(fz_is_external_link(mupdf->ctx, link->uri))
            fprintf(f, "%d %s\n", (int)link->rect.y0, link->uri);
    fclose(f);
}
