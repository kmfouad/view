#include <string.h>
#include "estd.h"
#include <libspectre/spectre.h>
#include "globals.h"
#include "book.h"
#include "book-ps.h"

typedef struct LibSpectre {
    SpectreDocument *doc;
    SpectreRenderContext *ctx;
} LibSpectre;

#define ps ((LibSpectre*)lib)

static void
fix_bitmap(Page *pg) {
    unsigned char *bitmap, *p, *r;
    bitmap = emalloc(pg->w * pg->h * 4);
    int linesize = pg->w*4;
    p = bitmap;
    r = pg->bitmap;
    for(int y = pg->h-1; y >= 0; y--, p += linesize, r += pg->linesize)
        memcpy(p, r, linesize);
    free(pg->bitmap);
    pg->bitmap = bitmap;
    pg->linesize = linesize;
}

Book *
ps_book_open(const char *path) {
    Book *book;

    lib = malloc(sizeof(LibSpectre));
    ps->doc = NULL;
    ps->ctx = NULL;

    ps->doc = spectre_document_new();
    if(!ps->doc)
        goto err;
    ps->ctx = spectre_render_context_new();
    if(!ps->ctx)
        goto err;

    spectre_document_load(ps->doc, path);
    if(spectre_document_status(ps->doc) != SPECTRE_STATUS_SUCCESS)
        goto err;

    book = book_init(spectre_document_get_n_pages(ps->doc));
    return book;

    err:
    free(ps->doc);
    free(ps->ctx);
    free(lib);
    return NULL;
}

Page *
ps_page_load(Page *page, double max_w, double max_h) {
    int w, h;
    double r1, r2, r;
    unsigned char *bitmap = NULL;
    int linesize;
    SpectrePage *pspage;

    pspage = spectre_document_get_page(ps->doc, page->n);
    if(!pspage)
        return NULL;

    spectre_page_get_size(pspage, &w, &h);

    r1 = max_w / (double)w;
    r2 = max_h / (double)h;
    r = r1 < r2 ? r1 : r2;
    w = (int)(r * (double)w);
    h = (int)(r * (double)h);

    spectre_render_context_set_scale(ps->ctx, r, r);
    spectre_page_render(pspage, ps->ctx, &bitmap, &linesize);

    page->bitmap = bitmap;
    page->w = w;
    page->h = h;
    page->linesize = linesize;

    fix_bitmap(page);
    return page;
}
