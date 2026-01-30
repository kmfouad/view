#include <stdio.h>
#include <stdint.h>
#define FB_IMP
#include "fb.h"
#include "globals.h"
#include "graphics.h"
#include "graphics-fb.h"

#define fb ((FB*)disp)

static void
fb_clear(void) {
    BITMAP_CLEAR(fb);
}

static void
fb_draw_error(void) {
    BITMAP_ERR(fb);
    fb_flush(fb);
}

static void
fb_draw_info(void) {
    printf("%d/%d", page->n + 1, book->page_count);
    fflush(stdout);
}

static void
fb_clear_info(void) {
    printf("\033[1K\r");
    fflush(stdout);
}

static void
fb_draw_image(Page *pg) {
    BITMAP_COPY(fb, pg);
    fb_flush(fb);
}

static void
fb_quit(void) {
    fb_clear();
    fflush(stdout);
    fb_free(fb);
}

void
fb_main(void) {
    int c;
    int mult;
    double max_w, max_h;

    disp = (void*)fb_init();
    fclose(stderr);
    printf("\033[H");
    fflush(stdout);
    HANDLE_KEY_INIT(fb->w, fb->h)
    goto draw;
    for(;;) {
        c = getchar();
        HANDLE_KEY(c, fb_)
    }
}
