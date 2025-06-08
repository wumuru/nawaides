#include "splash.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <stdlib.h>

void show_splash(ImageBuffer img, int seconds) {
    Display *dpy = XOpenDisplay(NULL);
    if (!dpy) return;

    int screen = DefaultScreen(dpy);
    int depth = DefaultDepth(dpy, screen);
    Visual *visual = DefaultVisual(dpy, screen);

    int screen_w = DisplayWidth(dpy, screen);
    int screen_h = DisplayHeight(dpy, screen);

    int x = (screen_w - img.width) / 2;
    int y = (screen_h - img.height) / 2;

    XSetWindowAttributes attrs;
    attrs.override_redirect = True;

    Window win = XCreateWindow(dpy, RootWindow(dpy, screen), x, y,
        img.width, img.height, 0, depth, InputOutput, visual,
        CWOverrideRedirect, &attrs);

    XMapWindow(dpy, win);
    XFlush(dpy);
    usleep(100000); // 100ms para garantir visibilidade

    GC gc = XCreateGC(dpy, win, 0, NULL);
    XImage *ximg = XCreateImage(dpy, visual, depth, ZPixmap, 0,
        (char *)img.pixels, img.width, img.height, 32, 0);

    XPutImage(dpy, win, gc, ximg, 0, 0, 0, 0, img.width, img.height);
    XFlush(dpy);

    sleep(seconds);

    XDestroyWindow(dpy, win);
    XFreeGC(dpy, gc);

    ximg->data = NULL; // evitar double-free
    XDestroyImage(ximg);
    XCloseDisplay(dpy);
}