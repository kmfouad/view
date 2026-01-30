#ifndef VIEW_GRAPHICS_H
#define VIEW_GRAPHICS_H

#include <stdlib.h>
#include <string.h>
#include "config.h"

#define BITMAP_SET(driver, width, height, color) \
    int w = width; \
    uint32_t *p = (uint32_t*)driver->bitmap + (driver->w - w) / 2; \
    for(int y = height-1; y >= 0; y--) \
       for(int x = w-1; x >= 0; x--) \
           *p++ = color

#define BITMAP_ERR(driver) BITMAP_SET(driver, driver->w/3, driver->h, 0xff00)
#define BITMAP_CLEAR(driver) BITMAP_SET(driver, driver->w, driver->h, 0)

#define BITMAP_COPY(driver, page) \
    uint32_t *p, *r; \
    p = (uint32_t*)driver->bitmap + (driver->w - page->w) / 2; \
    r = (uint32_t*)page->bitmap; \
    for(int y = page->h-1; y >= 0; y--, p += driver->w, r += page->w) \
        memcpy(p, r, page->linesize)

#define HANDLE_KEY_INIT(w, h) max_w = (double)w; max_h = (double)h; mult = 0;

#define HANDLE_KEY_FREE() EXIT:

#define HANDLE_KEY(c, funcprefix) \
    switch(c) { \
        case KEY_RANDOM: \
            undo = page; \
            page =  &book->pages[(int)(drand48() * (double)book->page_count)]; \
            goto draw; \
        case KEY_INFO: \
            if(info_visible) { \
                goto draw; \
            } else { \
                info_visible = 1; \
                funcprefix ## draw_info(); \
            } \
            break; \
        case KEY_START: \
            undo = page; \
            page = book->pages; \
            goto draw; \
        case KEY_END: \
            undo = page; \
            if(!mult) \
                mult = book->page_count; \
            page = &book->pages[mult-1]; \
            mult = 0; \
            goto draw; \
        case KEY_UP: \
            if(mult) \
                undo = page; \
            else \
                mult = 1; \
            if(page->n - mult >= 0) \
                page -= mult; \
            mult = 0; \
            goto draw; \
        case KEY_DOWN: \
            if(mult) \
                undo = page; \
            else \
                mult = 1; \
            if(page->n < book->page_count - mult) \
                page += mult; \
            mult = 0; \
            goto draw; \
        case KEY_UNDO: \
            tmp = page; \
            page = undo; \
            undo = tmp; \
            goto draw; \
        case KEY_LINKS: \
            if(dump_link) \
                dump_link(page); \
            goto draw; \
        case KEY_QUIT: \
            funcprefix ## quit(); \
            view_quit(); \
            break; \
        case '0' ... '9': \
            mult *= 10; \
            mult += c - '0'; \
            break; \
        draw: \
        case KEY_DRAW: \
            if(info_visible) { \
                funcprefix ## clear_info(); \
                info_visible = 0; \
            } \
            if(!page->bitmap) { \
                page_load(page, max_w, max_h); \
                if(!page->bitmap) { \
                    funcprefix ## draw_error(); \
                    break; \
                } \
            } \
            funcprefix ## draw_image(page); \
            break; \
        default: \
            break; \
    }

#endif
