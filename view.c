#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <libgen.h>
#include <math.h>
#include <mupdf/fitz.h>

#define VIEW_STORAGE
#include "estd.h"
#include "globals.h"
#include "db.h"
#include "magic.h"
#include "book-pdf.h"
#ifdef VIEW_FB
#include "graphics-fb.h"
#endif
#ifdef VIEW_X11
#include "graphics-x11.h"
#endif
#ifdef VIEW_WAY
#include "graphics-way.h"
#endif

static DB *db;
static const char *path;

static void
rand_init(void) {
    unsigned seed;

    //__asm__ ("rdrand %0\n" : "=r" (seed));
    seed = time(NULL);
    srand48(seed);
}

void
view_quit(void) {
    if(db_flag)
        db_add(db, path, page->n);
    exit(1);
}

int
main(int argc, char **argv) {
    int n = 0, arg = 1;

    if(argc < 2)
        die("view", "Not enough arguments");

    if(strcmp(argv[1], "-s") == 0) {
        db_flag = 0;
        arg++;
    }

    path = argv[arg];
    book = book_open(path);
    if(!book)
        diearg("view", path, "File not found");

    db = db_init("view", "books.txt");
    n = db_read(db, path);
    page = &book->pages[n];
    undo = page;
    rand_init();

    if(0) { 
#ifdef VIEW_FB
    } else if(strcmp(getenv("TERM"), "linux") == 0) {
        fb_main();
#endif
#ifdef VIEW_X11
    } else if(getenv("DISPLAY")) {
        x11_main();
#endif
#ifdef VIEW_WAY
    } else if(getenv("WAYLAND_DISPLAY")) {
        way_main();
#endif
    } else {
        die("view", "No display drivers found");
    }
}
