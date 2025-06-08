#ifndef SPLASH_H
#define SPLASH_H

typedef struct {
    unsigned char *pixels;
    int width;
    int height;
} ImageBuffer;

void show_splash(ImageBuffer img, int seconds);

#endif