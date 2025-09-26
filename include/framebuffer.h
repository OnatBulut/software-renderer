#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <float.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MIN(a,b)        (((a)<(b))?(a):(b))
#define MAX(a,b)        (((a)>(b))?(a):(b))
#define MIN3(a, b, c)   MIN(MIN((a),(b)),(c))
#define MAX3(a, b, c)   MAX(MAX((a),(b)),(c))
#define ABS(a)          (((a)<(0))?-(a):(a))
#define ARRLEN(arr)     (sizeof(arr)/sizeof((arr)[0]))

#define FONT_WIDTH      8
#define FONT_HEIGHT     8
#define CHAR_H_ADVANCE  (FONT_WIDTH + 1)
#define LINE_V_ADVANCE  (FONT_HEIGHT + 2)
#define TAB_COLS        4
#define TAB_PIXELS      (TAB_COLS * CHAR_H_ADVANCE)

typedef uint32_t pixel_t;

typedef struct
{
    pixel_t *pixels;
    float *depth;
    int width;
    int height;
    size_t size;
    uint8_t *font_bmp;
} Framebuffer;

static inline pixel_t color_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return (a << 24) | (r << 16) | (g << 8) | b;
}

static inline void framebuffer_clear_depth(Framebuffer *fb, float clear_depth)
{
    for (int i = 0; i < fb->width * fb->height; i++)
        fb->depth[i] = clear_depth;
}

static inline Framebuffer* framebuffer_create(int width, int height)
{
    Framebuffer *fb = malloc(sizeof(Framebuffer));
    if (!fb) return NULL;

    fb->width = width;
    fb->height = height;
    fb->size = width * height * sizeof(pixel_t);
    fb->pixels = malloc(fb->size);
    if (!fb->pixels)
    {
        free(fb);
        return NULL;
    }

    fb->depth = malloc(fb->width * fb->height * sizeof(float));
    if (!fb->depth)
    {
        free(fb->pixels);
        free(fb);
        return NULL;
    }

    framebuffer_clear_depth(fb, FLT_MAX);

    return fb;
}

static inline int framebuffer_resize(Framebuffer *fb, int width, int height)
{
    if (!fb) return -1;

    size_t new_size = (size_t)width * height * sizeof(pixel_t);
    pixel_t *new_pixels = realloc(fb->pixels, new_size);
    float *new_depth = realloc(fb->depth, width * height * sizeof(float));
    if (!new_pixels && !new_depth && new_size != 0) return -1;

    fb->width = width;
    fb->height = height;
    fb->size = new_size;
    fb->pixels = new_pixels;
    fb->depth = new_depth;

    return 0;
}

static inline void framebuffer_destroy(Framebuffer *fb)
{
    if (fb)
    {
        free(fb->pixels);
        free(fb->depth);
        free(fb);
    }
}

static inline void framebuffer_copy_pixels(const Framebuffer *fb, pixel_t *dst)
{
    if (!fb || !dst) return;
    memcpy(dst, fb->pixels, fb->size);
}

static inline void framebuffer_clear(Framebuffer *fb, pixel_t color)
{
    for (int i = 0; i < fb->width * fb->height; i++)
        fb->pixels[i] = color;
}

typedef struct { int x; int y; } point2_t;

static inline void framebuffer_draw_pixel(Framebuffer *fb, point2_t p, pixel_t color)
{
    if (p.x >= 0 && p.x < fb->width && p.y >= 0 && p.y < fb->height)
        fb->pixels[p.y * fb->width + p.x] = color;
}

static inline bool _liang_barsky_clip(float *x0, float *y0, float *x1, float *y1,
                                      float xmin, float ymin, float xmax, float ymax)
{
    float dx = *x1 - *x0;
    float dy = *y1 - *y0;
    float p[4] = { -dx, dx, -dy, dy };
    float q[4] = { *x0 - xmin, xmax - *x0, *y0 - ymin, ymax - *y0 };

    float t0 = 0.0f, t1 = 1.0f;

    for (int i = 0; i < 4; i++)
    {
        if (p[i] == 0.0f)
        {
            if (q[i] < 0.0f)
                return false;
        }
        else
        {
            float t = q[i] / p[i];
            if (p[i] < 0.0f)
            {
                if (t > t1)
                    return false;
                if (t > t0)
                    t0 = t;
            }
            else
            {
                if (t < t0)
                    return false;
                if (t < t1)
                    t1 = t;
            }
        }
    }

    *x1 = *x0 + t1 * dx;
    *y1 = *y0 + t1 * dy;
    *x0 = *x0 + t0 * dx;
    *y0 = *y0 + t0 * dy;

    return true;
}

static inline void framebuffer_draw_line(Framebuffer *fb, point2_t p0, point2_t p1, pixel_t color)
{
    float x0 = (float)p0.x;
    float y0 = (float)p0.y;
    float x1 = (float)p1.x;
    float y1 = (float)p1.y;

    if (!_liang_barsky_clip(&x0, &y0, &x1, &y1, 0, 0, fb->width - 1, fb->height - 1))
        return;

    p0.x = (int)(x0 + 0.5f);
    p0.y = (int)(y0 + 0.5f);
    p1.x = (int)(x1 + 0.5f);
    p1.y = (int)(y1 + 0.5f);

    // Point
    if (p0.x == p1.x && p0.y == p1.y)
    {
        framebuffer_draw_pixel(fb, p0, color);
        return;
    }

    // Vertical Line
    if (p0.x == p1.x)
    {
        if (p1.y > p0.y)
        {
            for (int y = p0.y; y <= p1.y; ++y)
            {
                point2_t pt = { p0.x, y };
                framebuffer_draw_pixel(fb, pt, color);
            }
        }
        else
        {
            for (int y = p0.y; y >= p1.y; --y)
            {
                point2_t pt = { p0.x, y };
                framebuffer_draw_pixel(fb, pt, color);
            }
        }
        return;
    }

    // Horizontal Line
    if (p0.y == p1.y)
    {
        if (p1.x > p0.x)
        {
            for (int x = p0.x; x <= p1.x; ++x)
            {
                point2_t pt = { x, p0.y };
                framebuffer_draw_pixel(fb, pt, color);
            }
        }
        else
        {
            for (int x = p0.x; x >= p1.x; --x)
            {
                point2_t pt = { x, p0.y };
                framebuffer_draw_pixel(fb, pt, color);
            }
        }
        return;
    }

    // Perfect Diagonal Line
    if (ABS(p1.x - p0.x) == ABS(p1.y - p0.y))
    {
        int dx = (p1.x > p0.x) ? 1 : -1;
        int dy = (p1.y > p0.y) ? 1 : -1;

        int x = p0.x;
        int y = p0.y;
        while (1)
        {
            point2_t pt = { x, y };
            framebuffer_draw_pixel(fb, pt, color);

            if (x == p1.x && y == p1.y) break;
            x += dx;
            y += dy;
        }
        return;
    }

    // Bresenham's Line
    {
        int dx = ABS(p1.x - p0.x);
        int dy = ABS(p1.y - p0.y);

        int sx = (p0.x < p1.x) ? 1 : -1;
        int sy = (p0.y < p1.y) ? 1 : -1;

        int err = (dx > dy ? dx : -dy) / 2;

        int x = p0.x;
        int y = p0.y;

        while (1)
        {
            point2_t pt = { x, y };
            framebuffer_draw_pixel(fb, pt, color);

            if (x == p1.x && y == p1.y) break;

            int e2 = err;
            if (e2 > -dx)
            {
                err -= dy;
                x += sx;
            }
            if (e2 <  dy)
            {
                err += dx;
                y += sy;
            }
        }
    }
}

static inline void framebuffer_draw_triangle(Framebuffer *fb, point2_t p0, point2_t p1, point2_t p2, pixel_t color)
{
    framebuffer_draw_line(fb, p0, p1, color);
    framebuffer_draw_line(fb, p1, p2, color);
    framebuffer_draw_line(fb, p2, p0, color);
}

typedef struct { int x; int y; float z; } point3_t;

static inline void _fill_triangle_generic(Framebuffer *fb, point3_t p0, point3_t p1, point3_t p2, pixel_t c0, pixel_t c1, pixel_t c2, bool use_depth, bool use_gouraud)
{
    const int min_x = MAX(0, MIN3(p0.x, p1.x, p2.x));
    const int max_x = MIN(fb->width - 1, MAX3(p0.x, p1.x, p2.x));
    const int min_y = MAX(0, MIN3(p0.y, p1.y, p2.y));
    const int max_y = MIN(fb->height - 1, MAX3(p0.y, p1.y, p2.y));

    if (min_x > max_x || min_y > max_y) return;

    const int dx01 = p0.x - p1.x;
    const int dy01 = p0.y - p1.y;
    const int dx12 = p1.x - p2.x;
    const int dy12 = p1.y - p2.y;
    const int dx20 = p2.x - p0.x;
    const int dy20 = p2.y - p0.y;

    const int area = dx12 * (p0.y - p2.y) - dy12 * (p0.x - p2.x);
    if (area <= 0) return;

    const float inv_area = 1.0f / (float)area;

    const int r0 = (c0 >> 16) & 0xFF;
    const int g0 = (c0 >>  8) & 0xFF;
    const int b0 = (c0 >>  0) & 0xFF;

    int r1, g1, b1, r2, g2, b2;
    if (use_gouraud)
    {
        r1 = (c1 >> 16) & 0xFF;
        g1 = (c1 >>  8) & 0xFF;
        b1 = (c1 >>  0) & 0xFF;
        r2 = (c2 >> 16) & 0xFF;
        g2 = (c2 >>  8) & 0xFF;
        b2 = (c2 >>  0) & 0xFF;
    }

    int w0_row = dx12 * (min_y - p2.y) - dy12 * (min_x - p2.x);
    int w1_row = dx20 * (min_y - p0.y) - dy20 * (min_x - p0.x);
    int w2_row = dx01 * (min_y - p1.y) - dy01 * (min_x - p1.x);

    for (int y = min_y; y <= max_y; ++y)
    {
        int w0 = w0_row;
        int w1 = w1_row;
        int w2 = w2_row;

        pixel_t *pixel_row = &fb->pixels[y * fb->width + min_x];
        float *depth_row = use_depth ? &fb->depth[y * fb->width + min_x] : NULL;

        for (int x = min_x; x <= max_x; ++x)
        {
            if (w0 >= 0 && w1 >= 0 && w2 >= 0)
            {
                float lambda0 = (float)w0 * inv_area;
                float lambda1 = (float)w1 * inv_area;
                float lambda2 = (float)w2 * inv_area;

                bool draw = true;
                if (use_depth)
                {
                    float z = lambda0 * p0.z + lambda1 * p1.z + lambda2 * p2.z;
                    draw = (z < *depth_row - 1e-5f);
                    if (draw)
                        *depth_row = z;
                }

                if (draw)
                {
                    int r, g, b;
                    if (use_gouraud)
                    {
                        r = (int)(lambda0 * r0 + lambda1 * r1 + lambda2 * r2);
                        g = (int)(lambda0 * g0 + lambda1 * g1 + lambda2 * g2);
                        b = (int)(lambda0 * b0 + lambda1 * b1 + lambda2 * b2);
                    }
                    else
                    {
                        r = r0;
                        g = g0;
                        b = b0;
                    }
                    *pixel_row = color_rgba(r, g, b, 255);
                }
            }

            w0 -= dy12;
            w1 -= dy20;
            w2 -= dy01;
            pixel_row++;
            if (use_depth) depth_row++;
        }

        w0_row += dx12;
        w1_row += dx20;
        w2_row += dx01;
    }
}

static inline void framebuffer_fill_triangle(Framebuffer *fb, point2_t p0, point2_t p1, point2_t p2, pixel_t c)
{
    point3_t p0_3d = {p0.x, p0.y, 0};
    point3_t p1_3d = {p1.x, p1.y, 0};
    point3_t p2_3d = {p2.x, p2.y, 0};
    _fill_triangle_generic(fb, p0_3d, p1_3d, p2_3d, c, c, c, false, false);
}

static inline void framebuffer_fill_triangle_gouraud(Framebuffer *fb, point2_t p0, point2_t p1, point2_t p2, pixel_t c0, pixel_t c1, pixel_t c2)
{
    point3_t p0_3d = {p0.x, p0.y, 0};
    point3_t p1_3d = {p1.x, p1.y, 0};
    point3_t p2_3d = {p2.x, p2.y, 0};
    _fill_triangle_generic(fb, p0_3d, p1_3d, p2_3d, c0, c1, c2, false, true);
}

static inline void framebuffer_fill_triangle_with_depth(Framebuffer *fb, point3_t p0, point3_t p1, point3_t p2, pixel_t c)
{
    _fill_triangle_generic(fb, p0, p1, p2, c, c, c, true, false);
}

static inline void framebuffer_fill_triangle_gouraud_with_depth(Framebuffer *fb, point3_t p0, point3_t p1, point3_t p2, pixel_t c0, pixel_t c1, pixel_t c2)
{
    _fill_triangle_generic(fb, p0, p1, p2, c0, c1, c2, true, true);
}

static inline void framebuffer_set_font(Framebuffer *fb, uint8_t *font_bmp)
{
    if (!font_bmp) return;
    fb->font_bmp = font_bmp;
}

static inline void framebuffer_write_char(Framebuffer *fb, point2_t p, pixel_t color, uint8_t c)
{
    if (!fb->font_bmp || p.x <= -FONT_WIDTH || p.x >= fb->width || p.y <= -FONT_HEIGHT || p.y >= fb->height)
        return;

    int x0 = p.x < 0 ? 0 : p.x;
    int y0 = p.y < 0 ? 0 : p.y;
    int x_end = p.x + FONT_WIDTH  > fb->width  ? fb->width  : p.x + FONT_WIDTH;
    int y_end = p.y + FONT_HEIGHT > fb->height ? fb->height : p.y + FONT_HEIGHT;

    const uint8_t *glyph = &fb->font_bmp[c * FONT_HEIGHT];

    for (int row = y0; row < y_end; ++row)
    {
        uint8_t bits = glyph[row - p.y];

        for (int col = x0; col < x_end; ++col)
        {
            int bit_idx = FONT_WIDTH - 1 - (col - p.x);
            if (bits & (1u << bit_idx))
                fb->pixels[row * fb->width + col] = color;
        }
    }
}

static inline void framebuffer_write_string(Framebuffer *fb, point2_t p, pixel_t color, const char *str)
{
    int cursor_x = p.x;
    int cursor_y = p.y;

    for (const char *c = str; *c != '\0'; ++c)
    {
        switch (*c)
        {
            case '\n':
                cursor_x = p.x;
                cursor_y += LINE_V_ADVANCE;
                break;

            case '\t':
                {
                    int cells_from_origin = (cursor_x - p.x) / CHAR_H_ADVANCE;
                    int next_tab_stop = ((cells_from_origin / TAB_COLS) + 1) * TAB_COLS;
                    cursor_x = p.x + next_tab_stop * CHAR_H_ADVANCE;
                }
                break;

            default:
                framebuffer_write_char(fb, (point2_t) { cursor_x, cursor_y }, color, (uint8_t)*c);
                cursor_x += CHAR_H_ADVANCE;
                break;
        }
    }
}

static inline void framebuffer_printf(Framebuffer *fb, point2_t p, pixel_t color, const char *fmt, ...)
{
    char tmp_buf[512];

    va_list ap;
    va_start(ap, fmt);
    (void)vsnprintf(tmp_buf, sizeof(tmp_buf), fmt, ap);
    va_end(ap);

    framebuffer_write_string(fb, p, color, tmp_buf);
}


#endif //__FRAMEBUFFER_H__
