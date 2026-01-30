#ifndef VIEW_GLOBALS_H
#define VIEW_GLOBALS_H

#include "fb.h"
#include "x11.h"
#include "book.h"

#ifdef VIEW_STORAGE
#define VIEW_EXTERN
#define VIEW_SET_VAL(x) x
#else
#define VIEW_EXTERN extern
#define VIEW_SET_VAL(x)
#endif

enum { BGRA = 0 };

VIEW_EXTERN void *lib;
VIEW_EXTERN void *disp;
VIEW_EXTERN Book *book;
VIEW_EXTERN Page *page, *undo, *tmp;
VIEW_EXTERN int info_visible;
VIEW_EXTERN int db_flag VIEW_SET_VAL(= 1);
VIEW_EXTERN int color_format VIEW_SET_VAL(= BGRA);
VIEW_EXTERN Page *(*page_load)(Page *, double, double);
VIEW_EXTERN void (*dump_link)(Page *page);

void view_quit(void);

#endif
