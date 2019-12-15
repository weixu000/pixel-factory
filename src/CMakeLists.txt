add_library(PixelFactory STATIC
        ../include/PixelFactory/Event.h
        GLContext.cpp ../include/PixelFactory/GL/GLContext.h
        ../include/PixelFactory/GL/GLBuffer.h
        ../include/PixelFactory/GL/GLVertexArray.h
        ../include/PixelFactory/GL/GLUniform.h
        GLShader.cpp ../include/PixelFactory/GL/GLShader.h
        ../include/PixelFactory/DrawOptions.h
        ../include/PixelFactory/Transform.h
        Entity.cpp ../include/PixelFactory/Entity.h
        ../include/PixelFactory/Component.h
        Trackball.cpp ../include/PixelFactory/Trackball.h
        Camera.cpp ../include/PixelFactory/Camera.h
        Axes.cpp ../include/PixelFactory/Axes.h
        Mesh.cpp ../include/PixelFactory/Mesh.h AABB.cpp ../include/PixelFactory/AABB.h
        ../include/PixelFactory/EventHandler.h)
target_link_libraries(PixelFactory PUBLIC glad glfw glm tinyobjloader)
target_include_directories(PixelFactory PUBLIC ../include)
target_compile_features(PixelFactory PUBLIC cxx_std_17)