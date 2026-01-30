FB = 1
X11 = 0
WAYLAND = 0

CC=cc
SRC = view.c magic.c book.c db.c
SRC += $(wildcard book-*.c)
LIBS = -lxxhash -lmupdf -lmagic -lm -ldjvulibre -lspectre

ifeq ($(FB),1)
    SRC += graphics-fb.c
    CFLAGS += -DVIEW_FB
endif
ifeq ($(X11),1)
    SRC += graphics-x11.c
    LIBS += -lX11
    CFLAGS += -DVIEW_X11
endif
ifeq ($(WAYLAND),1)
    SRC += graphics-wayland.c
    SRC += xdg-shell.c
    LIBS += -lwayland-client -lxkbcommon
    CFLAGS += -DVIEW_WAY
endif

all: view

view: $(SRC)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

xdg-shell.c:
	wayland-scanner private-code  < /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml > xdg-shell.c
	wayland-scanner client-header < /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml > xdg-shell.h

.PHONY: clean install
clean:
	rm -f view xdg-shell.c xdg-shell.h
install: view
	cp view $(DESTDIR)/bin/view
