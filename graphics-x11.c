#include <stdio.h>
#include <stdint.h>
#include "estd.h"
#define X11_IMP
#include "x11.h"
#include "globals.h"
#include "graphics.h"
#include "graphics-x11.h"
#include "util.h"

#define xorg ((x11*)disp)

static void
x11_draw_info(void) {
/*
    static char buf[100];
    snprintf(buf, sizeof(buf), "%d/%d", page->n, book->page_count);
    XDrawString(xorg->display, xorg->window, xorg->gc, 0, 0, buf, len);
    x11_flush(xorg);
*/
}

static void
x11_clear_info(void) {
}

static void
x11_draw_error(void) {
    BITMAP_ERR(xorg);
    x11_flush(xorg);
}

static void
x11_draw_image(Page *pg) {
    BITMAP_COPY(xorg, pg);
    x11_flush(xorg);
}

static void
x11_quit(void) {
    x11_free(xorg);
}

void
x11_main(void) {
    XEvent e;
    KeySym c;
    int w;
    double max_w, max_h;
    int mult;

    disp = (void*)x11_init();
    Page *mid = page + book->page_count / 2;
    page_load(mid, xorg->w/2,xorg->h);
    w = mid->w;
    if(mid->w < 100)
        w = 100;
    x11_init_window(xorg, w, mid->h);
    /*
    xorg->font = XLoadQueryFont(xorg->display, "fixed");
    if(!xorg->font)
        die(PROGNAME, "No fonts found");
    XSetFont(xorg->display, xorg->gc, xorg->font->fid);
    */
    HANDLE_KEY_INIT(xorg->w, xorg->h);
    goto draw;
    for(;;) {
        XNextEvent(xorg->display, &e);
        switch(e.type) {
        case Expose:
            x11_flush(xorg);
            break;
        case KeyPress:
            c = XkbKeycodeToKeysym(xorg->display, e.xkey.keycode, 0, 0);
            HANDLE_KEY(c, x11_)
            break;
        }
    }
}
