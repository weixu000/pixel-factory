add_subdirectory("glad")
add_subdirectory("glm")
add_subdirectory("tinyobjloader")
add_subdirectory("stb_image")

set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory("glfw")