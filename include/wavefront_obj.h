#ifndef __WAVEFRONT_OBJ_H__
#define __WAVEFRONT_OBJ_H__

#include <cglm/cglm.h>
#include <stdio.h>
#include <string.h>

#include "vec.h"
#include "graphics.h"


CREATE_VECTOR_TYPE(Vertex, vertex);

Mesh wavefront_obj_load_from_file(const char *filename)
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

    vector_vertex normals = vector_vertex_create(NULL, 0);
    vector_vertex_reserve(&normals, 0x200);

    char line[512];
    while (fgets(line, sizeof(line), file))
    {
        if (line[0] == 'v')
        {
            if(line[1] == ' ')
            {
                Vertex v;
                if (sscanf(line, "v %f %f %f", &v.p[0], &v.p[1], &v.p[2]) == 3)
                {
                    vector_vertex_push_back(&positions, v);
                }
                else
                {
                    fprintf(stderr, "Unrecognized format!\n");
                    return (Mesh) { 0 };
                }
            }
            else if (line[1] == 'n')
            {
                Vertex v;
                if (sscanf(line, "vn %f %f %f", &v.n[0], &v.n[1], &v.n[2]) == 3)
                {
                    vector_vertex_push_back(&normals, v);
                }
                else
                {
                    fprintf(stderr, "Unrecognized format!\n");
                    return (Mesh) { 0 };
                }
            }
        }
        else if (line[0] == 'f')
        {
            ivec3 pos, norm, tex;
            ivec4 pos4, norm4, tex4;
            if (sscanf(line, "f %u %u %u %u", &pos4[0], &pos4[1], &pos4[2], &pos4[3]) == 4)
            {
                Triangle tri = { 0 };
                glm_vec3_copy(vector_vertex_at(&positions, pos4[0] - 1)->p ,tri.v[0].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[1] - 1)->p ,tri.v[1].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[2] - 1)->p ,tri.v[2].p);
                vector_triangle_push_back(&mesh.tris, tri);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[0] - 1)->p ,tri.v[0].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[2] - 1)->p ,tri.v[1].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[3] - 1)->p ,tri.v[2].p);
                vector_triangle_push_back(&mesh.tris, tri);
            }
            else if (sscanf(line, "f %u/%u %u/%u %u/%u %u/%u",
                        &pos4[0], &tex4[0],
                        &pos4[1], &tex4[1],
                        &pos4[2], &tex4[2],
                        &pos4[3], &tex4[3]) == 8)
            {
                Triangle tri = { 0 };
                glm_vec3_copy(vector_vertex_at(&positions, pos4[0] - 1)->p ,tri.v[0].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[1] - 1)->p ,tri.v[1].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[2] - 1)->p ,tri.v[2].p);
                vector_triangle_push_back(&mesh.tris, tri);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[0] - 1)->p ,tri.v[0].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[2] - 1)->p ,tri.v[1].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[3] - 1)->p ,tri.v[2].p);
                vector_triangle_push_back(&mesh.tris, tri);
            }
            else if (sscanf(line, "f %u//%u %u//%u %u//%u %u//%u",
                        &pos4[0], &norm4[0],
                        &pos4[1], &norm4[1],
                        &pos4[2], &norm4[2],
                        &pos4[3], &norm4[3]) == 8)
            {
                Triangle tri = { 0 };
                glm_vec3_copy(vector_vertex_at(&positions, pos4[0] - 1)->p ,tri.v[0].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[1] - 1)->p ,tri.v[1].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[2] - 1)->p ,tri.v[2].p);
                glm_vec3_copy(vector_vertex_at(&normals, norm4[0] - 1)->n ,tri.v[0].n);
                glm_vec3_copy(vector_vertex_at(&normals, norm4[1] - 1)->n ,tri.v[1].n);
                glm_vec3_copy(vector_vertex_at(&normals, norm4[2] - 1)->n ,tri.v[2].n);
                vector_triangle_push_back(&mesh.tris, tri);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[0] - 1)->p ,tri.v[0].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[2] - 1)->p ,tri.v[1].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[3] - 1)->p ,tri.v[2].p);
                glm_vec3_copy(vector_vertex_at(&normals, norm4[0] - 1)->n ,tri.v[0].n);
                glm_vec3_copy(vector_vertex_at(&normals, norm4[2] - 1)->n ,tri.v[1].n);
                glm_vec3_copy(vector_vertex_at(&normals, norm4[3] - 1)->n ,tri.v[2].n);
                vector_triangle_push_back(&mesh.tris, tri);
            }
            else if (sscanf(line, "f %u/%u/%u %u/%u/%u %u/%u/%u %u/%u/%u",
                        &pos4[0], &tex4[0], &norm4[0],
                        &pos4[1], &tex4[1], &norm4[1],
                        &pos4[2], &tex4[2], &norm4[2],
                        &pos4[3], &tex4[3], &norm4[3]) == 12)
            {
                Triangle tri = { 0 };
                glm_vec3_copy(vector_vertex_at(&positions, pos4[0] - 1)->p ,tri.v[0].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[1] - 1)->p ,tri.v[1].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[2] - 1)->p ,tri.v[2].p);
                glm_vec3_copy(vector_vertex_at(&normals, norm4[0] - 1)->n ,tri.v[0].n);
                glm_vec3_copy(vector_vertex_at(&normals, norm4[1] - 1)->n ,tri.v[1].n);
                glm_vec3_copy(vector_vertex_at(&normals, norm4[2] - 1)->n ,tri.v[2].n);
                vector_triangle_push_back(&mesh.tris, tri);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[0] - 1)->p ,tri.v[0].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[2] - 1)->p ,tri.v[1].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[3] - 1)->p ,tri.v[2].p);
                glm_vec3_copy(vector_vertex_at(&normals, norm4[0] - 1)->n ,tri.v[0].n);
                glm_vec3_copy(vector_vertex_at(&normals, norm4[2] - 1)->n ,tri.v[1].n);
                glm_vec3_copy(vector_vertex_at(&normals, norm4[3] - 1)->n ,tri.v[2].n);
                vector_triangle_push_back(&mesh.tris, tri);
            }
            else if (sscanf(line, "f %u %u %u", &pos[0], &pos[1], &pos[2]) == 3)
            {
                Triangle tri = { 0 };
                glm_vec3_copy(vector_vertex_at(&positions, pos[0] - 1)->p ,tri.v[0].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos[1] - 1)->p ,tri.v[1].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos[2] - 1)->p ,tri.v[2].p);
                vector_triangle_push_back(&mesh.tris, tri);
            }
            else if (sscanf(line, "f %u/%u %u/%u %u/%u",
                        &pos4[0], &tex4[0],
                        &pos4[1], &tex4[1],
                        &pos4[2], &tex4[2]) == 6)
            {
                Triangle tri = { 0 };
                glm_vec3_copy(vector_vertex_at(&positions, pos4[0] - 1)->p ,tri.v[0].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[1] - 1)->p ,tri.v[1].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos4[2] - 1)->p ,tri.v[2].p);
                vector_triangle_push_back(&mesh.tris, tri);
            }
            else if (sscanf(line, "f %u//%u %u//%u %u//%u",
                        &pos[0], &norm[0],
                        &pos[1], &norm[1],
                        &pos[2], &norm[2]) == 6)
            {
                Triangle tri = { 0 };
                glm_vec3_copy(vector_vertex_at(&positions, pos[0] - 1)->p ,tri.v[0].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos[1] - 1)->p ,tri.v[1].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos[2] - 1)->p ,tri.v[2].p);
                glm_vec3_copy(vector_vertex_at(&normals, norm[0] - 1)->n ,tri.v[0].n);
                glm_vec3_copy(vector_vertex_at(&normals, norm[1] - 1)->n ,tri.v[1].n);
                glm_vec3_copy(vector_vertex_at(&normals, norm[2] - 1)->n ,tri.v[2].n);
                vector_triangle_push_back(&mesh.tris, tri);
            }
            else if (sscanf(line, "f %u/%u/%u %u/%u/%u %u/%u/%u",
                        &pos[0], &tex[0], &norm[0],
                        &pos[1], &tex[1], &norm[1],
                        &pos[2], &tex[2], &norm[2]) == 9)
            {
                Triangle tri = { 0 };
                glm_vec3_copy(vector_vertex_at(&positions, pos[0] - 1)->p ,tri.v[0].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos[1] - 1)->p ,tri.v[1].p);
                glm_vec3_copy(vector_vertex_at(&positions, pos[2] - 1)->p ,tri.v[2].p);
                glm_vec3_copy(vector_vertex_at(&normals, norm[0] - 1)->n ,tri.v[0].n);
                glm_vec3_copy(vector_vertex_at(&normals, norm[1] - 1)->n ,tri.v[1].n);
                glm_vec3_copy(vector_vertex_at(&normals, norm[2] - 1)->n ,tri.v[2].n);
                vector_triangle_push_back(&mesh.tris, tri);
            }
            else
            {
                fprintf(stderr, "Unrecognized format!\n");
                return (Mesh) { 0 };
            }
        }
    }

    vector_vertex_destroy(&positions);
    vector_vertex_destroy(&normals);
    fclose(file);
    return mesh;
}

#endif //__WAVEFRONT_OBJ_H__
