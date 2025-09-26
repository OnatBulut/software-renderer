#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "common.h"
#include "camera.h"
#include "framebuffer.h"


typedef struct
{
    Mesh mesh;
    vec3 pos;           // position: x, y, z
    vec3 rot;           // rotation: roll, pitch, yaw
    vec3 scale;
    mat4 model;         // transformed object
    mat3 normal_mat;    // normal transformation matrix
    bool dirty;
} Object;

typedef struct
{
    mat4 proj;
    mat4 view;
    vec3 ambient_light_color;
    Camera *cam;
} World;

typedef struct
{
    vec3 pos;
    vec3 color;
} Light;

static Triangle __attribute__((unused)) graphics_cube_tris[] = {
    // SOUTH  (‑Z)
    { .smooth = false, .v = {
        { .p = { 0.0f, 0.0f, 0.0f, 1.0f }, .n = { 0.0f, 0.0f, -1.0f } },
        { .p = { 0.0f, 1.0f, 0.0f, 1.0f }, .n = { 0.0f, 0.0f, -1.0f } },
        { .p = { 1.0f, 1.0f, 0.0f, 1.0f }, .n = { 0.0f, 0.0f, -1.0f } }
    }},
    { .smooth = false, .v = {
        { .p = { 0.0f, 0.0f, 0.0f, 1.0f }, .n = { 0.0f, 0.0f, -1.0f } },
        { .p = { 1.0f, 1.0f, 0.0f, 1.0f }, .n = { 0.0f, 0.0f, -1.0f } },
        { .p = { 1.0f, 0.0f, 0.0f, 1.0f }, .n = { 0.0f, 0.0f, -1.0f } }
    }},

    // EAST  (+X)
    { .smooth = false, .v = {
        { .p = { 1.0f, 0.0f, 0.0f, 1.0f }, .n = { 1.0f, 0.0f, 0.0f } },
        { .p = { 1.0f, 1.0f, 0.0f, 1.0f }, .n = { 1.0f, 0.0f, 0.0f } },
        { .p = { 1.0f, 1.0f, 1.0f, 1.0f }, .n = { 1.0f, 0.0f, 0.0f } }
    }},
    { .smooth = false, .v = {
        { .p = { 1.0f, 0.0f, 0.0f, 1.0f }, .n = { 1.0f, 0.0f, 0.0f } },
        { .p = { 1.0f, 1.0f, 1.0f, 1.0f }, .n = { 1.0f, 0.0f, 0.0f } },
        { .p = { 1.0f, 0.0f, 1.0f, 1.0f }, .n = { 1.0f, 0.0f, 0.0f } }
    }},

    // NORTH (+Z)
    { .smooth = false, .v = {
        { .p = { 1.0f, 0.0f, 1.0f, 1.0f }, .n = { 0.0f, 0.0f, 1.0f } },
        { .p = { 1.0f, 1.0f, 1.0f, 1.0f }, .n = { 0.0f, 0.0f, 1.0f } },
        { .p = { 0.0f, 1.0f, 1.0f, 1.0f }, .n = { 0.0f, 0.0f, 1.0f } }
    }},
    { .smooth = false, .v = {
        { .p = { 1.0f, 0.0f, 1.0f, 1.0f }, .n = { 0.0f, 0.0f, 1.0f } },
        { .p = { 0.0f, 1.0f, 1.0f, 1.0f }, .n = { 0.0f, 0.0f, 1.0f } },
        { .p = { 0.0f, 0.0f, 1.0f, 1.0f }, .n = { 0.0f, 0.0f, 1.0f } }
    }},

    // WEST (-X)
    { .smooth = false, .v = {
        { .p = { 0.0f, 0.0f, 1.0f, 1.0f }, .n = { -1.0f, 0.0f, 0.0f } },
        { .p = { 0.0f, 1.0f, 1.0f, 1.0f }, .n = { -1.0f, 0.0f, 0.0f } },
        { .p = { 0.0f, 1.0f, 0.0f, 1.0f }, .n = { -1.0f, 0.0f, 0.0f } }
    }},
    { .smooth = false, .v = {
        { .p = { 0.0f, 0.0f, 1.0f, 1.0f }, .n = { -1.0f, 0.0f, 0.0f } },
        { .p = { 0.0f, 1.0f, 0.0f, 1.0f }, .n = { -1.0f, 0.0f, 0.0f } },
        { .p = { 0.0f, 0.0f, 0.0f, 1.0f }, .n = { -1.0f, 0.0f, 0.0f } }
    }},

    // TOP (+Y)
    { .smooth = false, .v = {
        { .p = { 0.0f, 1.0f, 0.0f, 1.0f }, .n = { 0.0f, 1.0f, 0.0f } },
        { .p = { 0.0f, 1.0f, 1.0f, 1.0f }, .n = { 0.0f, 1.0f, 0.0f } },
        { .p = { 1.0f, 1.0f, 1.0f, 1.0f }, .n = { 0.0f, 1.0f, 0.0f } }
    }},
    { .smooth = false, .v = {
        { .p = { 0.0f, 1.0f, 0.0f, 1.0f }, .n = { 0.0f, 1.0f, 0.0f } },
        { .p = { 1.0f, 1.0f, 1.0f, 1.0f }, .n = { 0.0f, 1.0f, 0.0f } },
        { .p = { 1.0f, 1.0f, 0.0f, 1.0f }, .n = { 0.0f, 1.0f, 0.0f } }
    }},

    // BOTTOM (-Y)
    { .smooth = false, .v = {
        { .p = { 1.0f, 0.0f, 1.0f, 1.0f }, .n = { 0.0f, -1.0f, 0.0f } },
        { .p = { 0.0f, 0.0f, 1.0f, 1.0f }, .n = { 0.0f, -1.0f, 0.0f } },
        { .p = { 0.0f, 0.0f, 0.0f, 1.0f }, .n = { 0.0f, -1.0f, 0.0f } }
    }},
    { .smooth = false, .v = {
        { .p = { 1.0f, 0.0f, 1.0f, 1.0f }, .n = { 0.0f, -1.0f, 0.0f } },
        { .p = { 0.0f, 0.0f, 0.0f, 1.0f }, .n = { 0.0f, -1.0f, 0.0f } },
        { .p = { 1.0f, 0.0f, 0.0f, 1.0f }, .n = { 0.0f, -1.0f, 0.0f } }
    }}
};

static inline void graphics_point2(const vec2 src, point2_t *dest)
{
    dest->x = src[0];
    dest->y = src[1];
}

static inline void graphics_point3(const vec3 src, point3_t *dest)
{
    dest->x = src[0];
    dest->y = src[1];
    dest->z = src[2];
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

static inline void graphics_multiply_matrix_vector(mat4 matrix, vec4 src, vec4 dest)
{
    vec4 tmp;
    glm_mat4_mulv(matrix, src, tmp);
    if (tmp[3] > GLM_FLT_EPSILON)
    {
        tmp[0] /= tmp[3];
        tmp[1] /= tmp[3];
        tmp[2] /= tmp[3];
    }
    glm_vec4_copy(tmp, dest);
}

static inline void graphics_set_projection(const Framebuffer *fb, float fNear, float fFar, float fFovDeg, mat4 out)
{
    float fAspect = (float)fb->width / (float)fb->height;
    glm_perspective(fFovDeg * M_PI / 180, fAspect, fNear, fFar, out);
}

static inline void graphics_world_init(Framebuffer *fb, World *world, Camera *cam, vec3 ambient_light_color)
{
    graphics_set_projection(fb, 0.1f, 1000.0f, 60.0f, world->proj);
    world->cam = cam;
    glm_vec3_copy(ambient_light_color, world->ambient_light_color);
}

static inline void graphics_world_update(World *world)
{
    camera_update_view(world->cam);
    glm_mat4_copy(world->cam->view, world->view);
}

static inline void graphics_light_init(Light *light, uint8_t r, uint8_t g, uint8_t b)
{
    light->color[0] = r / 255.0f;
    light->color[1] = g / 255.0f;
    light->color[2] = b / 255.0f;
}

static inline void graphics_light_update(Light* light, vec3 pos)
{
    glm_vec3_copy(pos, light->pos);
}

static inline void graphics_object_init(Object *obj, Mesh mesh)
{
    obj->mesh = mesh;
    glm_vec3_zero(obj->pos);
    glm_vec3_zero(obj->rot);
    glm_vec3_one(obj->scale);
    glm_mat4_identity(obj->model);
    glm_mat3_identity(obj->normal_mat);
    obj->dirty = false;
}

static inline void graphics_object_set_position(Object *obj, vec3 pos)
{
    glm_vec3_copy(pos, obj->pos);
    obj->dirty = true;
}

static inline void graphics_object_set_rotation(Object *obj, vec3 rot)
{
    glm_vec3_copy(rot, obj->rot);
    obj->dirty = true;
}

static inline void graphics_object_set_scale(Object *obj, vec3 scale)
{
    glm_vec3_copy(scale, obj->scale);
    obj->dirty = true;
}

static inline void graphics_object_translate(Object *obj, vec3 delta)
{
    glm_vec3_add(obj->pos, delta, obj->pos);
    obj->dirty = true;
}

static inline void graphics_object_rotate(Object *obj, vec3 delta)
{
    glm_vec3_add(obj->rot, delta, obj->rot);
    obj->dirty = true;
}

static inline void graphics_object_update(Object *obj)
{
    if (!obj->dirty) return;

    mat4 translation, rot_x, rot_y, rot_z, rot, scale_mat;

    glm_mat4_identity(translation);
    glm_translate(translation, obj->pos);

    glm_mat4_identity(rot_x);
    glm_rotate_x(rot_x, obj->rot[0], rot_x);

    glm_mat4_identity(rot_y);
    glm_rotate_y(rot_y, obj->rot[1], rot_y);

    glm_mat4_identity(rot_z);
    glm_rotate_z(rot_z, obj->rot[2], rot_z);

    glm_mat4_mul(rot_z, rot_y, rot);
    glm_mat4_mul(rot, rot_x, rot);

    glm_mat4_identity(scale_mat);
    glm_scale(scale_mat, obj->scale);

    mat4 temp;
    glm_mat4_mul(rot, scale_mat, temp);
    glm_mat4_mul(translation, temp, obj->model);

    mat3 model3x3;
    glm_mat4_pick3(obj->model, model3x3);
    glm_mat3_inv(model3x3, obj->normal_mat);
    glm_mat3_transpose(obj->normal_mat);

    obj->dirty = false;
}

static inline void graphics_calculate_color(World *world, Light *light, mat3 normal_matrix, vec4 normal_obj, vec3 vertex_world_pos, vec3 color_out)
{
    vec3 normal_ws;
    glm_mat3_mulv(normal_matrix, normal_obj, normal_ws);
    glm_vec3_normalize(normal_ws);

    vec3 light_dir;
    glm_vec3_sub(light->pos, vertex_world_pos, light_dir);
    glm_vec3_normalize(light_dir);

    float ndotl = glm_vec3_dot(normal_ws, light_dir);
    ndotl = fmaxf(ndotl, 0.0f);

    vec3 diffuse, ambient;
    glm_vec3_scale(light->color, ndotl, diffuse);
    glm_vec3_mul(light->color, world->ambient_light_color, ambient);
    glm_vec3_add(diffuse, ambient, color_out);
    for (int k = 0; k < 3; ++k)
        color_out[k] = fminf(fmaxf(color_out[k], 0.0f), 1.0f);
}

void graphics_draw_object(Framebuffer *fb, World *world, Light *light, Object *obj)
{
    graphics_object_update(obj);

    mat4 mvp;
    glm_mat4_mulN((mat4 *[]) {&world->proj, &world->view, &obj->model}, 3, mvp);

    const size_t triangle_count = vector_triangle_size(&obj->mesh.tris);
    for (size_t i = 0; i < triangle_count; ++i)
    {
        Triangle *tri = vector_triangle_at(&obj->mesh.tris, i);
        Triangle tri_clip, tri_screen;

        for (int v = 0; v < 3; ++v)
        {
            vec4 vertex_obj = { tri->v[v].p[0], tri->v[v].p[1], tri->v[v].p[2], 1.0f };
            vec4 vertex_world;
            graphics_multiply_matrix_vector(obj->model, vertex_obj, vertex_world);

            graphics_multiply_matrix_vector(mvp, vertex_obj, tri_clip.v[v].p);

            vec4 normal_obj = { tri->v[v].n[0], tri->v[v].n[1], tri->v[v].n[2], 0.0f };
            graphics_calculate_color(world, light, obj->normal_mat, normal_obj, vertex_world, tri->v[v].c);

            tri_screen.v[v].p[0] = (tri_clip.v[v].p[0] + 1.0f) * 0.5f * (float)(fb->width - 1);
            tri_screen.v[v].p[1] = (tri_clip.v[v].p[1] - 1.0f) * -0.5f * (float)(fb->height - 1);
            tri_screen.v[v].p[2] = tri_clip.v[v].p[2];
        }

        // continue if triangle is behind the camera (temporary solution)
        if (tri_screen.v[0].p[2] > 1 || tri_screen.v[1].p[2] > 1 || tri_screen.v[2].p[2] > 1) continue;

        // continue if the triangle is facing backwards
        float dx1 = tri_screen.v[1].p[0] - tri_screen.v[0].p[0];
        float dy1 = tri_screen.v[1].p[1] - tri_screen.v[0].p[1];
        float dx2 = tri_screen.v[2].p[0] - tri_screen.v[0].p[0];
        float dy2 = tri_screen.v[2].p[1] - tri_screen.v[0].p[1];
        if (dx1 * dy2 - dy1 * dx2 > 0.0f) continue;

        point3_t p0, p1, p2;
        graphics_point3(tri_screen.v[0].p, &p0);
        graphics_point3(tri_screen.v[1].p, &p1);
        graphics_point3(tri_screen.v[2].p, &p2);

        if (tri->smooth)
        {
            pixel_t c0 = graphics_color_rgba_from_linear(tri->v[0].c);
            pixel_t c1 = graphics_color_rgba_from_linear(tri->v[1].c);
            pixel_t c2 = graphics_color_rgba_from_linear(tri->v[2].c);

            framebuffer_fill_triangle_gouraud_with_depth(fb, p0, p1, p2, c0, c1, c2);
        }
        else
        {
            pixel_t c = graphics_color_rgba_from_linear(tri->v[0].c);

            framebuffer_fill_triangle_with_depth(fb, p0, p1, p2, c);
        }
    }
}


#endif //__GRAPHICS_H__
