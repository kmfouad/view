# view

view is a minimal document viewer, it features vi-like keybindings, and uses a simple text file to keep track of reading progress in ~/.local/share/view/books.txt

## building

to build for the framebuffer simply run
```
make
```
to build for all display drivers run
```
make X11=1 WAYLAND=1
```

dependencies:
- xxhash
- mupdf
- djvulibre
- libspectre
- libX11 (optional)
- libwayland (optional)

all the keys can be modified in include/config.h
