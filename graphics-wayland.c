#include <stdio.h>
#include <stdint.h>
#include "estd.h"
#define WAY_IMP
#include "way.h"
#include "globals.h"
#include "graphics.h"
#include "graphics-way.h"
#include "util.h"

#define way ((Way*)disp)
#define D() printf("%s: %d\n", __func__, __LINE__);

static double max_w, max_h;
static int mult;

static void
way_draw_info(void) {
}

static void
way_clear_info(void) {
}

static void
way_draw_error(void) {
    BITMAP_ERR(way);
    way_flush(way);
}

static void
way_draw_image(Page *pg) {
    BITMAP_COPY(way, pg);
    way_damage(way, 0, 0, way->w, way->h);
    way_flush(way);
}

static void
way_quit(void) {
    way_free(way);
}

static void
in(int c) {
    HANDLE_KEY(c, way_)
}

void
way_main(void) {
    Page *mid;
    int w;

    disp = (void*)way_init(in);
    mid = page + book->page_count / 2;
    page_load(mid, way->w/2,way->h);
    w = mid->w;
    if(mid->w < 100)
        w = 100;
    way_init_window(way, w, way->h);
    way_flush(way);
    HANDLE_KEY_INIT(way->w, way->h);
    page_load(page, max_w, max_h);
    way_draw_image(page);
    way_loop(way);
}
