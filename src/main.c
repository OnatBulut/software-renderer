#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <stdlib.h>

#include "renderer.h"


#define USE_TEXTURE_RENDERER    1
#define SCREEN_WIDTH            1280
#define SCREEN_HEIGHT           720
#define RENDER_WIDTH            SCREEN_WIDTH / 2
#define RENDER_HEIGHT           SCREEN_HEIGHT / 2

static World g_world;
static Light g_light;
float g_fps = 0.0f;

static Object objects[3];
static int num_objects = 0;

static void draw_frame(Framebuffer *fb)
{
    framebuffer_clear(fb, color_rgba(60, 150, 240, 255));
    framebuffer_clear_depth(fb, FLT_MAX);

    graphics_world_update(&g_world);
    graphics_light_update(&g_light, g_world.cam->pos);

    for (int i = 0; i < num_objects; i++)
    {
        graphics_draw_object(fb, &g_world, &g_light, &objects[i]);
    }

    framebuffer_printf(fb, (point2_t) { 2, 2 }, color_rgba(255, 255, 255, 255), "FPS:%.0f\n", g_fps);
    /*
    framebuffer_printf(fb, (point2_t) { 2, fb->height - 110 }, color_rgba(255, 255, 255, 255),
            "ESC - Quit\n"
            "W - Move Forward\n"
            "S - Move Backwards\n"
            "A - Move Left\n"
            "D - Move Right\n"
            "Space - Move Up\n"
            "LCtrl - Move Down\n"
            "LShift - Move Faster\n"
            "Q - Tilt Left\n"
            "E - Tilt Right\n"
            "Left Mouse Button - Look Around");
    */
}

void keyboard_movement(World *world, float dt)
{
    const float MOVE_SPEED     = 8.0f;
    const float VERTICAL_SPEED = 6.0f;
    const float ROTATE_SPEED   = M_PI;
    const bool *kbd = SDL_GetKeyboardState(NULL);

    static float mtp = 1.0f;
    if (kbd[SDL_SCANCODE_LSHIFT])
        mtp = 4.0f;
    else
        mtp = 1.0f;

    if (kbd[SDL_SCANCODE_W] && !kbd[SDL_SCANCODE_S])
        camera_move_forward(world->cam, MOVE_SPEED * mtp * dt);
    if (kbd[SDL_SCANCODE_S] && !kbd[SDL_SCANCODE_W])
        camera_move_forward(world->cam, -MOVE_SPEED * mtp * dt);
    if (kbd[SDL_SCANCODE_A] && !kbd[SDL_SCANCODE_D])
        camera_move_right(world->cam, -MOVE_SPEED * mtp * dt);
    if (kbd[SDL_SCANCODE_D] && !kbd[SDL_SCANCODE_A])
        camera_move_right(world->cam, MOVE_SPEED * mtp * dt);

    if (kbd[SDL_SCANCODE_SPACE] && !kbd[SDL_SCANCODE_LCTRL])
        camera_move_up(world->cam, VERTICAL_SPEED * mtp * dt);
    if (kbd[SDL_SCANCODE_LCTRL] && !kbd[SDL_SCANCODE_SPACE])
        camera_move_up(world->cam, -VERTICAL_SPEED * mtp * dt);
    if (kbd[SDL_SCANCODE_Q] && !kbd[SDL_SCANCODE_E])
        camera_roll(world->cam, ROTATE_SPEED * dt);
    if (kbd[SDL_SCANCODE_E] && !kbd[SDL_SCANCODE_Q])
        camera_roll(world->cam, -ROTATE_SPEED * dt);
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <path>\n", argv[0]);
        return 1;
    }

    const char *filepath = argv[1];

    SDL_Window *window = NULL;
#if USE_TEXTURE_RENDERER
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
#else
    SDL_Surface *surface = NULL;
#endif
    SDL_Event event = { 0 };

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Soft", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_SetWindowResizable(window, true);

#if USE_TEXTURE_RENDERER
    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create renderer: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, RENDER_WIDTH, RENDER_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderVSync(renderer, 0);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, RENDER_WIDTH, RENDER_HEIGHT);
    if (!texture)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create texture: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    Framebuffer *framebuffer = framebuffer_create(RENDER_WIDTH, RENDER_HEIGHT);
#else
    surface = SDL_GetWindowSurface(window);
    if (!surface)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't get surface: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    Framebuffer *framebuffer = framebuffer_create(surface->w, surface->h);
#endif

    framebuffer_set_font(framebuffer, _font_ibm5155_cga_rom_memotek_gr_01__8x8);

    const float EMA_ALPHA = 0.01f;
    const uint64_t perf_freq = SDL_GetPerformanceFrequency();
    uint64_t curr_time = SDL_GetPerformanceCounter();
    uint64_t prev_time = curr_time;

    //Mesh cube_mesh = { 0 };
    //cube_mesh.tris = vector_triangle_create(graphics_cube_tris, ARRLEN(graphics_cube_tris));

    Mesh loaded_mesh = wavefront_obj_load_from_file(filepath);
    printf("Object mesh size: %lu triangles\n", (unsigned long)vector_triangle_size(&loaded_mesh.tris));

    graphics_object_init(&objects[0], loaded_mesh);
    graphics_object_set_position(&objects[0], (vec3) { -3.0f, 0.0f, 0.0f });

    graphics_object_init(&objects[1], loaded_mesh);
    graphics_object_set_position(&objects[1], (vec3) { 0.0f, 0.0f, 0.0f });

    graphics_object_init(&objects[2], loaded_mesh);
    graphics_object_set_position(&objects[2], (vec3) { 3.0f, 0.0f, 0.0f });

    num_objects = 3;

    Camera cam = { 0 };
    vec3 start_pos = { 0.0f, 0.0f, 5.0f };
    vec3 start_att = { 0.0f };
    camera_init(&cam, start_pos, start_att);

    graphics_world_init(framebuffer, &g_world, &cam, (vec3) { 0.12f, 0.29f, 0.47f });
    graphics_light_init(&g_light, 128, 128, 128);

    bool look_around = false;
    bool running = true;
    while (running)
    {
        curr_time = SDL_GetPerformanceCounter();
        double dt = (double)(curr_time - prev_time) / (double)perf_freq;
        prev_time = curr_time;

        if (dt > 0.0)
        {
            float instant_fps = 1.0f / (float)dt;
            g_fps = EMA_ALPHA * instant_fps + (1.0f - EMA_ALPHA) * g_fps;
        }

        keyboard_movement(&g_world, dt);

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_ESCAPE)
                    running = false;
            }
            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    SDL_SetWindowRelativeMouseMode(window, true);
                    look_around = true;
                }
            }
            if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    SDL_SetWindowRelativeMouseMode(window, false);
                    look_around = false;
                }
            }
            if (event.type == SDL_EVENT_MOUSE_MOTION && look_around)
            {
                camera_yaw(g_world.cam, event.motion.xrel * -0.005f);
                camera_pitch(g_world.cam, event.motion.yrel * -0.005f);
            }
            if (event.type == SDL_EVENT_QUIT)
            {
                running = false;
            }
#if USE_TEXTURE_RENDERER
#else
            if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                surface = SDL_GetWindowSurface(window);
                if (!surface)
                {
                    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't get surface: %s\n", SDL_GetError());
                    running = false;
                }
                framebuffer_resize(framebuffer, surface->w, surface->h);
            }
#endif
        }

        static float time = 0.0f;
        time += dt;

        graphics_object_set_rotation(&objects[0], (vec3) { 0.0f, time * 0.5f, 0.0f });
        graphics_object_set_rotation(&objects[1], (vec3) { time * 0.3f, time * 0.7f, 0.0f });
        graphics_object_set_rotation(&objects[2], (vec3) { 0.0f, time * -0.4f, time * 0.2f });

        draw_frame(framebuffer);

#if USE_TEXTURE_RENDERER
        void *pixels;
        int pitch;
        SDL_LockTexture(texture, NULL, &pixels, &pitch);
        framebuffer_copy_pixels(framebuffer, pixels);
        SDL_UnlockTexture(texture);

        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
#else
        SDL_LockSurface(surface);
        framebuffer_copy_pixels(framebuffer, surface->pixels);
        SDL_UnlockSurface(surface);

        SDL_UpdateWindowSurface(window);
#endif
    }

    vector_triangle_destroy(&loaded_mesh.tris);
    framebuffer_destroy(framebuffer);
#if USE_TEXTURE_RENDERER
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
#else
    SDL_DestroySurface(surface);
#endif
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
