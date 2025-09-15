#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include <cglm/cglm.h>

#include "camera.h"
#include "framebuffer.h"
#include "vec.h"


typedef struct
{
    vec3 p;
    vec3 n;
    vec3 c;
} Vertex;

typedef struct
{
    Vertex v[3];
} Triangle;

CREATE_VECTOR_TYPE(Triangle, triangle)
typedef struct
{
    vector_triangle tris;
} Mesh;

typedef struct
{
    mat4 world;
    mat4 proj;
    mat4 WV;
    mat4 WVP;
    vec3 ambient_light_color;
    Camera *cam;
} World;

typedef struct
{
    vec3 source_pos;
    vec3 dir_norm;
    vec3 color;
} Light;

static Triangle __attribute__((unused)) graphics_cube_tris[] = {
    // SOUTH  (‑Z)
    { .v = {
        { .p = { 0.0f, 0.0f, 0.0f }, .n = { 0.0f, 0.0f, -1.0f } },
        { .p = { 0.0f, 1.0f, 0.0f }, .n = { 0.0f, 0.0f, -1.0f } },
        { .p = { 1.0f, 1.0f, 0.0f }, .n = { 0.0f, 0.0f, -1.0f } }
    }},
    { .v = {
        { .p = { 0.0f, 0.0f, 0.0f }, .n = { 0.0f, 0.0f, -1.0f } },
        { .p = { 1.0f, 1.0f, 0.0f }, .n = { 0.0f, 0.0f, -1.0f } },
        { .p = { 1.0f, 0.0f, 0.0f }, .n = { 0.0f, 0.0f, -1.0f } }
    }},

    // EAST  (+X)
    { .v = {
        { .p = { 1.0f, 0.0f, 0.0f }, .n = { 1.0f, 0.0f, 0.0f } },
        { .p = { 1.0f, 1.0f, 0.0f }, .n = { 1.0f, 0.0f, 0.0f } },
        { .p = { 1.0f, 1.0f, 1.0f }, .n = { 1.0f, 0.0f, 0.0f } }
    }},
    { .v = {
        { .p = { 1.0f, 0.0f, 0.0f }, .n = { 1.0f, 0.0f, 0.0f } },
        { .p = { 1.0f, 1.0f, 1.0f }, .n = { 1.0f, 0.0f, 0.0f } },
        { .p = { 1.0f, 0.0f, 1.0f }, .n = { 1.0f, 0.0f, 0.0f } }
    }},

    // NORTH (+Z)
    { .v = {
        { .p = { 1.0f, 0.0f, 1.0f }, .n = { 0.0f, 0.0f, 1.0f } },
        { .p = { 1.0f, 1.0f, 1.0f }, .n = { 0.0f, 0.0f, 1.0f } },
        { .p = { 0.0f, 1.0f, 1.0f }, .n = { 0.0f, 0.0f, 1.0f } }
    }},
    { .v = {
        { .p = { 1.0f, 0.0f, 1.0f }, .n = { 0.0f, 0.0f, 1.0f } },
        { .p = { 0.0f, 1.0f, 1.0f }, .n = { 0.0f, 0.0f, 1.0f } },
        { .p = { 0.0f, 0.0f, 1.0f }, .n = { 0.0f, 0.0f, 1.0f } }
    }},

    // WEST (-X)
    { .v = {
        { .p = { 0.0f, 0.0f, 1.0f }, .n = { -1.0f, 0.0f, 0.0f } },
        { .p = { 0.0f, 1.0f, 1.0f }, .n = { -1.0f, 0.0f, 0.0f } },
        { .p = { 0.0f, 1.0f, 0.0f }, .n = { -1.0f, 0.0f, 0.0f } }
    }},
    { .v = {
        { .p = { 0.0f, 0.0f, 1.0f }, .n = { -1.0f, 0.0f, 0.0f } },
        { .p = { 0.0f, 1.0f, 0.0f }, .n = { -1.0f, 0.0f, 0.0f } },
        { .p = { 0.0f, 0.0f, 0.0f }, .n = { -1.0f, 0.0f, 0.0f } }
    }},

    // TOP (+Y)
    { .v = {
        { .p = { 0.0f, 1.0f, 0.0f }, .n = { 0.0f, 1.0f, 0.0f } },
        { .p = { 0.0f, 1.0f, 1.0f }, .n = { 0.0f, 1.0f, 0.0f } },
        { .p = { 1.0f, 1.0f, 1.0f }, .n = { 0.0f, 1.0f, 0.0f } }
    }},
    { .v = {
        { .p = { 0.0f, 1.0f, 0.0f }, .n = { 0.0f, 1.0f, 0.0f } },
        { .p = { 1.0f, 1.0f, 1.0f }, .n = { 0.0f, 1.0f, 0.0f } },
        { .p = { 1.0f, 1.0f, 0.0f }, .n = { 0.0f, 1.0f, 0.0f } }
    }},

    // BOTTOM (-Y)
    { .v = {
        { .p = { 1.0f, 0.0f, 1.0f }, .n = { 0.0f, -1.0f, 0.0f } },
        { .p = { 0.0f, 0.0f, 1.0f }, .n = { 0.0f, -1.0f, 0.0f } },
        { .p = { 0.0f, 0.0f, 0.0f }, .n = { 0.0f, -1.0f, 0.0f } }
    }},
    { .v = {
        { .p = { 1.0f, 0.0f, 1.0f }, .n = { 0.0f, -1.0f, 0.0f } },
        { .p = { 0.0f, 0.0f, 0.0f }, .n = { 0.0f, -1.0f, 0.0f } },
        { .p = { 1.0f, 0.0f, 0.0f }, .n = { 0.0f, -1.0f, 0.0f } }
    }}
};

static inline void graphics_point2(const vec2 src, point2_t *dest)
{
    dest->x = src[0];
    dest->y = src[1];
}

static inline void vec3_pow(const vec3 src, float exp, vec3 dst)
{
    dst[0] = powf(src[0], exp);
    dst[1] = powf(src[1], exp);
    dst[2] = powf(src[2], exp);
}

static inline pixel_t graphics_color_rgba_from_linear(const vec3 lin)
{
    vec3 gamma;
    vec3_pow(lin, 1.0f / 2.2f, gamma);

    return color_rgba(
        (uint8_t)(gamma[0] * 255.0f),
        (uint8_t)(gamma[1] * 255.0f),
        (uint8_t)(gamma[2] * 255.0f),
        255);
}

static inline void graphics_multiply_matrix_vector(mat4 matrix, vec3 src, vec3 dest)
{
    vec4 tmp;
    glm_vec4(src, 1, tmp);
    glm_mat4_mulv(matrix, tmp, tmp);
    if (tmp[3] != 0.0f || tmp[3] != 1.0f)
    {
       tmp[0] /= tmp[3];
       tmp[1] /= tmp[3];
       tmp[2] /= tmp[3];
    }
    glm_vec3(tmp, dest);
}

static inline void graphics_set_projection(const Framebuffer *fb, float fNear, float fFar, float fFovDeg, mat4 out)
{
    float fAspect = (float)fb->width / (float)fb->height;
    glm_perspective(fFovDeg * M_PI / 180, fAspect, fNear, fFar, out);
}

static inline void graphics_world_init(Framebuffer *fb, World *world, Camera *cam, vec3 ambient_light_color)
{
    glm_mat4_identity(world->world);
    graphics_set_projection(fb, 0.1f, 1000.0f, 60.0f, world->proj);
    world->cam = cam;
    glm_vec3_copy(ambient_light_color, world->ambient_light_color);
}

static inline void graphics_world_update(World *world)
{
    camera_update_view(world->cam);
    glm_mat4_mul(world->cam->view, world->world, world->WV);
    glm_mat4_mul(world->proj, world->WV, world->WVP);
}

static inline void graphics_light_init(Light *light, uint8_t r, uint8_t g, uint8_t b)
{
    light->color[0] = r / 255.0f;
    light->color[1] = g / 255.0f;
    light->color[2] = b / 255.0f;
}

static inline void graphics_light_update(Light* light, vec3 pos)
{
    glm_vec3_normalize_to(pos, light->dir_norm);
}

static inline void graphics_calculate_color(World *world, Light *light, vec4 normal_obj, vec3 color_out)
{
    vec4 normal_ws;
    glm_mat4_mulv(world->world, normal_obj, normal_ws);
    glm_vec3_normalize(normal_ws);

    float ndotl = glm_vec3_dot(normal_obj, light->dir_norm);
    ndotl = fmaxf(ndotl, 0.0f);

    vec3 diffuse, ambient;
    glm_vec3_scale(light->color, ndotl, diffuse);
    glm_vec3_mul(light->color, world->ambient_light_color, ambient);
    glm_vec3_add(diffuse, ambient, color_out);
    for (int k = 0; k < 3; ++k)
        color_out[k] = fminf(fmaxf(color_out[k], 0.0f), 1.0f);
}

void graphics_draw_mesh(Framebuffer *fb, World *world, Light *light, Mesh *mesh)
{
    const size_t triangle_count = vector_triangle_size(&mesh->tris);
    for (size_t i = 0; i < triangle_count; ++i)
    {
        Triangle *tri = vector_triangle_at(&mesh->tris, i);
        Triangle tri_clip, tri_screen;

        for (int v = 0; v < 3; ++v)
        {
            vec4 world_pos = { tri->v[v].p[0], tri->v[v].p[1], tri->v[v].p[2], 1.0f };
            graphics_multiply_matrix_vector(world->WVP, world_pos, tri_clip.v[v].p);

            vec4 normal_obj = { tri->v[v].n[0], tri->v[v].n[1], tri->v[v].n[2], 1.0f };
            graphics_calculate_color(world, light, normal_obj, tri->v[v].c);

            float x = (tri_clip.v[v].p[0] + 1.0f) * 0.5f * (float)(fb->width - 1);
            float y = (tri_clip.v[v].p[1] - 1.0f) * -0.5f * (float)(fb->height - 1);
            tri_screen.v[v].p[0] = x;
            tri_screen.v[v].p[1] = y;
        }

        float dx1 = tri_screen.v[1].p[0] - tri_screen.v[0].p[0];
        float dy1 = tri_screen.v[1].p[1] - tri_screen.v[0].p[1];
        float dx2 = tri_screen.v[2].p[0] - tri_screen.v[0].p[0];
        float dy2 = tri_screen.v[2].p[1] - tri_screen.v[0].p[1];
        if (dx1 * dy2 - dy1 * dx2 > 0.0f) continue;

        point2_t p1, p2, p3;
        graphics_point2(tri_screen.v[0].p, &p1);
        graphics_point2(tri_screen.v[1].p, &p2);
        graphics_point2(tri_screen.v[2].p, &p3);

        pixel_t c0 = graphics_color_rgba_from_linear(tri->v[0].c);
        pixel_t c1 = graphics_color_rgba_from_linear(tri->v[1].c);
        pixel_t c2 = graphics_color_rgba_from_linear(tri->v[2].c);
        framebuffer_fill_triangle(fb, p1, p2, p3, c0, c1, c2);
    }
}

#endif //__GRAPHICS_H__
