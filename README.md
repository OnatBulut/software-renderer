# To Do

- [X] Creating primitive drawing functions
- [X] Rendering a cube
- [X] Backface culling
- [X] Camera movement
- [X] Triangle rasterizer
- [X] Basic lighting
- [X] Wavefront obj import
- [X] Triangle clipping in Z axis
- [X] Depth buffer
- [ ] Textures
- [ ] Better wavefront obj handling
- [X] Object positioning and transformation
- [ ] Multithreading / Use of SIMD for faster rendering
- [ ] ...

# Building and running the application (Linux only for now)
```bash
$ git clone https://github.com/OnatBulut/software-renderer.git
$ cd software-renderer
$ make
$ build/software-renderer tes.obj
```

## Dependencies
[SDL3](https://github.com/libsdl-org/SDL) - needs to be manually installed on your system for now\
[cglm](https://github.com/recp/cglm) - cloned from source automatically upon build

## Build dependencies:
```
make
cmake
ninja
```
\
\
Special thanks to my friend [ddenizozgur](https://github.com/ddenizozgur) for his guidance on rendering topics.
