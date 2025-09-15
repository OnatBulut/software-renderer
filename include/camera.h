#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <cglm/cglm.h>


#define DEG_TO_RAD(a) ((a)*M_PI/180)
#define RAD_TO_DEG(a) ((a)*180/M_PI)

typedef struct
{
    vec3 pos;   // position: x, y, z
    vec3 att;   // attitude: roll, pitch, yaw
    mat4 view;
    bool dirty;
} Camera;

static inline void euler_to_mat4(vec3 euler, mat4 out)
{
    mat4 tmp;
    glm_mat4_identity(tmp);

    mat4 yawM;
    glm_rotate_y(tmp, euler[2], yawM);

    mat4 pitchM;
    glm_rotate_x(yawM, euler[1], pitchM);

    mat4 rollM;
    glm_rotate_z(pitchM, euler[0], rollM);

    glm_mat4_copy(rollM, out);
}

static inline void camera_init(Camera *cam, vec3 start_pos, vec3 start_att)
{
    glm_vec3_copy(start_pos, cam->pos);
    glm_vec3_copy(start_att, cam->att);
    cam->dirty = true;
}

static inline void camera_update_view(Camera *cam)
{
    if (!cam->dirty) return;

    mat4 orient;
    euler_to_mat4(cam->att, orient);

    vec3 forward, right, up;
    glm_mat4_mulv3(orient, (vec3) { 0.0f, 0.0f, -1.0f }, 1.0f, forward);
    glm_mat4_mulv3(orient, (vec3) { 1.0f, 0.0f, 0.0f },  1.0f, right);
    glm_mat4_mulv3(orient, (vec3) { 0.0f, 1.0f, 0.0f },  1.0f, up);

    vec3 target;
    glm_vec3_add(cam->pos, forward, target);
    glm_lookat(cam->pos, target, up, cam->view);

    cam->dirty = false;
}

static inline void camera_move(Camera *cam, vec3 deltaWorld)
{
    glm_vec3_add(cam->pos, deltaWorld, cam->pos);
    cam->dirty = true;
}

static inline void camera_move_forward(Camera *cam, float distance)
{
    vec3 forward;
    mat4 orient;
    euler_to_mat4(cam->att, orient);
    glm_mat4_mulv3(orient, (vec3){ 0.0f, 0.0f, -1.0f }, 1.0f, forward);
    glm_vec3_scale(forward, distance, forward);
    camera_move(cam, forward);
}

static inline void camera_move_right(Camera *cam, float distance)
{
    vec3 right;
    mat4 orient;
    euler_to_mat4(cam->att, orient);
    glm_mat4_mulv3(orient, (vec3){ 1.0f, 0.0f, 0.0f }, 1.0f, right);
    glm_vec3_scale(right, distance, right);
    camera_move(cam, right);
}

static inline void camera_move_up(Camera *cam, float distance)
{
    vec3 up;
    mat4 orient;
    euler_to_mat4(cam->att, orient);
    glm_mat4_mulv3(orient, (vec3){ 0.0f, 1.0f, 0.0f }, 1.0f, up);
    glm_vec3_scale(up, distance, up);
    camera_move(cam, up);
}

static inline void camera_yaw(Camera *cam, float angle_rad)
{
    cam->att[2] += angle_rad;
    cam->dirty = true;
}

static inline void camera_pitch(Camera *cam, float angle_rad)
{
    if ((cam->att[1] >= DEG_TO_RAD(89) && angle_rad >= 0) || (cam->att[1] <= DEG_TO_RAD(-89) && angle_rad <= 0)) return;
    cam->att[1] += angle_rad;
    cam->dirty = true;
}

static inline void camera_roll(Camera *cam, float angle_rad)
{
    cam->att[0] += angle_rad;
    cam->dirty = true;
}

#endif //__CAMERA_H__
