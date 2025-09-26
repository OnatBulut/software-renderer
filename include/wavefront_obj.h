#ifndef __WAVEFRONT_OBJ_H__
#define __WAVEFRONT_OBJ_H__


#include <stdbool.h>

#include "common.h"


CREATE_VECTOR_TYPE(Vertex, vertex);

Mesh wavefront_obj_load_from_file(const char *filename, const unsigned int max_v_per_face)
{
    Mesh mesh = { .tris = vector_triangle_create(NULL, 0) };
    vector_triangle_reserve(&mesh.tris, 0x200);

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        vector_triangle_destroy(&mesh.tris);
        fprintf(stderr, "File not found!\n");
        return (Mesh) { 0 };
    }

    vector_vertex positions = vector_vertex_create(NULL, 0);
    vector_vertex_reserve(&positions, 0x200);

    vector_vertex texcoords = vector_vertex_create(NULL, 0);
    vector_vertex_reserve(&texcoords, 0x200);

    vector_vertex normals = vector_vertex_create(NULL, 0);
    vector_vertex_reserve(&normals, 0x200);

    unsigned int skipped = 0, line_idx = 0;
    bool _smooth = false;

    char line[512];
    while (fgets(line, sizeof(line), file))
    {
        ++line_idx;
        if (line[0] == 'v')
        {
            if(line[1] == ' ')  // List of geometric vertices, with (x, y, z, [w]) coordinates, w is optional and defaults to 1.0.
            {
                Vertex v;
                if (sscanf(line, "v %f %f %f %f", &v.p[0], &v.p[1], &v.p[2], &v.p[3]) == 4)
                {
                    vector_vertex_push_back(&positions, v);
                }
                else if (sscanf(line, "v %f %f %f", &v.p[0], &v.p[1], &v.p[2]) == 3)
                {
                    v.p[3] = 1.0f;
                    vector_vertex_push_back(&positions, v);
                }
                else
                {
                    fprintf(stderr, "Unrecognized vertex format!\n");
                    return (Mesh) { 0 };
                }
            }
            else if (line[1] == 't') // List of texture coordinates, in (u, [v, w]) coordinates, these will vary between 0 and 1. v, w are optional and default to 0.
            {
                Vertex v;
                if (sscanf(line, "vt %f %f %f", &v.t[0], &v.t[1], &v.t[2]) == 3)
                {
                    vector_vertex_push_back(&texcoords, v);
                }
                else if (sscanf(line, "vt %f %f", &v.t[0], &v.t[1]) == 2)
                {
                    v.t[2] = 0.0f;
                    vector_vertex_push_back(&texcoords, v);
                }
                else if (sscanf(line, "vt %f", &v.t[0]) == 1)
                {
                    v.t[1] = v.t[2] = 0.0f;
                    vector_vertex_push_back(&normals, v);
                }
                else
                {
                    fprintf(stderr, "Unrecognized texture coordinate format!\n");
                    return (Mesh) { 0 };
                }
            }
            else if (line[1] == 'n') // List of vertex normals in (i, j, k) form; normals might not be unit vectors.
            {
                Vertex v;
                if (sscanf(line, "vn %f %f %f", &v.n[0], &v.n[1], &v.n[2]) == 3)
                {
                    vector_vertex_push_back(&normals, v);
                }
                else
                {
                    fprintf(stderr, "Unrecognized vertex normal format!\n");
                    return (Mesh) { 0 };
                }
            }
        }
        else if (line[0] == 's')
        {
            int t = 0;
            if (strcmp(line, "s off\n") == 0)
            {
                _smooth = false;
            }
            else if (sscanf(line, "s %d", &t) == 1)
            {
                _smooth = t;
            }
            else
            {
                fprintf(stderr, "Unrecognized smoothing group format!\n");
                return (Mesh) { 0 };
            }
        }
        else if (line[0] == 'f')
        {
            enum { UNK = 0, POS = 1, TEX = 2, NORM = 4 } v_type = UNK;
            unsigned int v_cnt = 0;
            unsigned int p_in, p_val[max_v_per_face],
                         n_in, n_val[max_v_per_face],
                         t_in, t_val[max_v_per_face];

            char *t_line = strtok(line, " \t");
            while (t_line)
            {
                if (v_cnt > max_v_per_face)
                {
                    printf("Face with %u or more vertices detected, consider increasing the limit.\n", max_v_per_face);
                    break;
                }

                if (sscanf(t_line, "%u/%u/%u", &p_in, &t_in, &n_in) == 3)
                {
                    p_val[v_cnt] = p_in;
                    t_val[v_cnt] = t_in;
                    n_val[v_cnt] = n_in;
                    ++v_cnt;
                    v_type = POS | TEX | NORM;
                }
                else if (sscanf(t_line, "%u//%u", &p_in, &n_in) == 2)
                {
                    p_val[v_cnt] = p_in;
                    n_val[v_cnt] = n_in;
                    ++v_cnt;
                    v_type = POS | NORM;
                }
                else if (sscanf(t_line, "%u/%u", &p_in, &t_in) == 2)
                {
                    p_val[v_cnt] = p_in;
                    t_val[v_cnt] = t_in;
                    ++v_cnt;
                    v_type = POS | TEX;
                }
                else if (sscanf(t_line, "%u", &p_in) == 1)
                {
                    p_val[v_cnt++] = p_in;
                    v_type = POS;
                }
                else if (strcmp(t_line, "f") != 0)
                {
                    fprintf(stderr, "Unrecognized face format on line %u!\n", line_idx);
                    v_type = UNK;
                    ++skipped;
                    break;
                }

                t_line = strtok(NULL, " \t");
            }

            if (v_type == UNK)
                continue;

            while (v_cnt > 2)
            {
                Triangle tri = { 0 };
                tri.smooth = _smooth;

                glm_vec4_copy(vector_vertex_at(&positions, p_val[0] - 1)->p ,tri.v[0].p);
                glm_vec4_copy(vector_vertex_at(&positions, p_val[v_cnt - 2] - 1)->p ,tri.v[1].p);
                glm_vec4_copy(vector_vertex_at(&positions, p_val[v_cnt - 1] - 1)->p ,tri.v[2].p);
                if (v_type & TEX)
                {
                    glm_vec3_copy(vector_vertex_at(&texcoords, t_val[0] - 1)->t ,tri.v[0].t);
                    glm_vec3_copy(vector_vertex_at(&texcoords, t_val[v_cnt - 2] - 1)->t ,tri.v[1].t);
                    glm_vec3_copy(vector_vertex_at(&texcoords, t_val[v_cnt - 1] - 1)->t ,tri.v[2].t);
                }
                if (v_type & NORM)
                {
                    glm_vec3_copy(vector_vertex_at(&normals, n_val[0] - 1)->n ,tri.v[0].n);
                    glm_vec3_copy(vector_vertex_at(&normals, n_val[v_cnt - 2] - 1)->n ,tri.v[1].n);
                    glm_vec3_copy(vector_vertex_at(&normals, n_val[v_cnt - 1] - 1)->n ,tri.v[2].n);
                }
                vector_triangle_push_back(&mesh.tris, tri);
                --v_cnt;
            }
        }
    }

    if (skipped > 0)
        printf("Skipped %u faces.\n", skipped);

    vector_vertex_destroy(&positions);
    vector_vertex_destroy(&normals);
    fclose(file);
    return mesh;
}


#endif //__WAVEFRONT_OBJ_H__
