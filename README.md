# PixelFactory
Trying to implement a game engine...
## Features
- Use [spdlog](https://github.com/gabime/spdlog) for logging
- GLFW window setup
    - A naive event system `EventHanler` and `Event`
    - `Application` handles the event loop and a global clock
    - `Window` create [GLFW](https://github.com/glfw/glfw) window
    - `GLContext` set up OpenGL context by [glad](https://github.com/weixu000/glad)
- OpenGL objects wrappers
    - `GLContext` handles global states
    - Various classes wraps buffers, vertex arrays, renderbuffers, framebuffers, texures, and shaders
    - Use [DSA](https://www.khronos.org/opengl/wiki/Direct_State_Access)
- Deferred renderer
    - Create G-Buffer for a large number of meshes
    - Add cube map as shadow map for each point light
    - Phong shading
- Entity-component
    - `Entity` is a node in the scene graph
    - `Component` implements part of attributes of an `Entity`
## TODO
- OpenGL objects wrappers
    - [ ] Instanced rendering
    - [ ] Uniform block
- Deferred renderer
    - [ ] PBR
    - [ ] Optimization
- Entity-component
    - [ ] Entity-Component-System
-[ ] UI
-[ ] Documentation
