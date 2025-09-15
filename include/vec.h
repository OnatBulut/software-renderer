#ifndef __VEC_H_
#define __VEC_H_

#include <stdbool.h>
#include <stdlib.h>


#define CREATE_VECTOR_TYPE(TYPE, NAME)                                          \
    typedef struct                                                              \
    {                                                                           \
        TYPE *data;                                                             \
        size_t size;                                                            \
        size_t capacity;                                                        \
    } vector_##NAME;                                                            \
                                                                                \
    vector_##NAME vector_##NAME##_create(TYPE *data, size_t size)               \
    {                                                                           \
        vector_##NAME vec = { .data = NULL };                                   \
        if (data && size > 0)                                                   \
        {                                                                       \
            vec.data = malloc(size * sizeof(TYPE));                             \
            if (!vec.data) return (vector_##NAME) { .data = NULL };             \
            memcpy(vec.data, data, size * sizeof(TYPE));                        \
            vec.size = size;                                                    \
            vec.capacity = size;                                                \
        }                                                                       \
        return vec;                                                             \
    }                                                                           \
                                                                                \
    void vector_##NAME##_destroy(vector_##NAME *vec)                            \
    {                                                                           \
        if (!vec) return;                                                       \
        free(vec->data);                                                        \
        vec->data = NULL;                                                       \
        vec->size = 0;                                                          \
        vec->capacity = 0;                                                      \
    }                                                                           \
                                                                                \
    bool vector_##NAME##_push_back(vector_##NAME *vec, TYPE value)              \
    {                                                                           \
        if (!vec) return false;                                                 \
        if (vec->size >= vec->capacity)                                         \
        {                                                                       \
            size_t new_capacity = vec->capacity == 0 ? 4 : vec->capacity << 1;  \
            TYPE *new_data = realloc(vec->data, new_capacity * sizeof(TYPE));   \
            if (!new_data) return false;                                        \
            vec->data = new_data;                                               \
            vec->capacity = new_capacity;                                       \
        }                                                                       \
        vec->data[vec->size++] = value;                                         \
        return true;                                                            \
    }                                                                           \
                                                                                \
    bool vector_##NAME##_pop_back(vector_##NAME *vec)                           \
    {                                                                           \
        if (!vec || vec->size == 0) return false;                               \
        vec->size--;                                                            \
        return true;                                                            \
    }                                                                           \
                                                                                \
    TYPE* vector_##NAME##_at(vector_##NAME *vec, size_t index)                  \
    {                                                                           \
        if (!vec || index >= vec->size) return NULL;                            \
        return &vec->data[index];                                               \
    }                                                                           \
                                                                                \
    TYPE* vector_##NAME##_front(vector_##NAME *vec)                             \
    {                                                                           \
        if (!vec || vec->size == 0) return NULL;                                \
        return &vec->data[0];                                                   \
    }                                                                           \
                                                                                \
    TYPE* vector_##NAME##_back(vector_##NAME *vec)                              \
    {                                                                           \
        if (!vec || vec->size == 0) return NULL;                                \
        return &vec->data[vec->size - 1];                                       \
    }                                                                           \
                                                                                \
    size_t vector_##NAME##_size(vector_##NAME *vec)                             \
    {                                                                           \
        return vec ? vec->size : 0;                                             \
    }                                                                           \
                                                                                \
    size_t vector_##NAME##_capacity(vector_##NAME *vec)                         \
    {                                                                           \
        return vec ? vec->capacity : 0;                                         \
    }                                                                           \
                                                                                \
    bool vector_##NAME##_empty(vector_##NAME *vec)                              \
    {                                                                           \
        return vec ? (vec->size == 0) : true;                                   \
    }                                                                           \
                                                                                \
    void vector_##NAME##_clear(vector_##NAME *vec)                              \
    {                                                                           \
        if (vec) vec->size = 0;                                                 \
    }                                                                           \
                                                                                \
    bool vector_##NAME##_reserve(vector_##NAME *vec, size_t new_capacity)       \
    {                                                                           \
        if (!vec || new_capacity <= vec->capacity) return true;                 \
        TYPE *new_data = realloc(vec->data, new_capacity * sizeof(TYPE));       \
        if (!new_data) return false;                                            \
        vec->data = new_data;                                                   \
        vec->capacity = new_capacity;                                           \
        return true;                                                            \
    }                                                                           \

#endif //__VEC_H_
