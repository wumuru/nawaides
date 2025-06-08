#include <librsvg/rsvg.h>
#include <cairo/cairo.h>
#include <cairo/cairo-image-surface.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "splash.h"

ImageBuffer raster_svg(const char *filepath) {
    rsvg_init();
    RsvgHandle *handle = rsvg_handle_new_from_file(filepath, NULL);
    if (!handle) {
        fprintf(stderr, "Erro: não foi possível carregar SVG.\n");
        return (ImageBuffer){NULL, 0, 0};
    }

    RsvgDimensionData dims;
    rsvg_handle_get_dimensions(handle, &dims);
    int w = dims.width;
    int h = dims.height;

    cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, w, h);
    cairo_t *cr = cairo_create(surface);
    rsvg_handle_render_cairo(handle, cr);

    cairo_destroy(cr);
    g_object_unref(handle);

    int stride = cairo_image_surface_get_stride(surface);
    unsigned char *src = cairo_image_surface_get_data(surface);
    unsigned char *copy = malloc(stride * h);
    memcpy(copy, src, stride * h);

    cairo_surface_destroy(surface);

    return (ImageBuffer){copy, w, h};
}

int main() {
    ImageBuffer img = raster_svg("../assets/splash.svg");
    if (!img.pixels) return 1;

    show_splash(img, 3);
    free(img.pixels);
    return 0;
}