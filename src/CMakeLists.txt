add_library(PixelFactory STATIC
        ../include/PixelFactory/Event.h
        ../include/PixelFactory/EventHandler.h
        Time.cpp ../include/PixelFactory/Time.h
        Window.cpp ../include/PixelFactory/Window.h
        Application.cpp ../include/PixelFactory/Application.h
        ModelLoader.cpp ../include/PixelFactory/ModelLoader.h
        Image.cpp ../include/PixelFactory/Image.h
        GLContext.cpp ../include/PixelFactory/gl/GlContext.h
        ../include/PixelFactory/gl/GlBuffer.h
        ../include/PixelFactory/gl/GlVertexArray.h
        ../include/PixelFactory/gl/GlUniform.h
        GLShader.cpp ../include/PixelFactory/gl/GlShader.h
        GlProgram.cpp ../include/PixelFactory/gl/GlProgram.h
        ../include/PixelFactory/gl/GlFramebuffer.h
        ../include/PixelFactory/gl/GlRenderbuffer.h
        ../include/PixelFactory/gl/GlTexture.h
        ../include/PixelFactory/gl/GlTexture2D.h
        DeferredRenderer.cpp ../include/PixelFactory/renderer/DeferredRenderer.h
        ../include/PixelFactory/renderer/PhongMaterial.h
        ../include/PixelFactory/renderer/DrawOptions.h
        ../include/PixelFactory/Transform.h
        Entity.cpp ../include/PixelFactory/Entity.h
        ../include/PixelFactory/components/Component.h
        Trackball.cpp ../include/PixelFactory/components/Trackball.h
        FreeMover.cpp ../include/PixelFactory/components/FreeMover.h
        FreeRotator.cpp ../include/PixelFactory/components/FreeRotator.h
        Camera.cpp ../include/PixelFactory/components/Camera.h
        Axes.cpp ../include/PixelFactory/components/Axes.h
        Mesh.cpp ../include/PixelFactory/components/Mesh.h
        PointLight.cpp ../include/PixelFactory/components/PointLight.h)
target_link_libraries(PixelFactory PUBLIC glad glfw glm tinyobjloader stb_image spdlog)
target_include_directories(PixelFactory PUBLIC ../include)
target_compile_features(PixelFactory PUBLIC cxx_std_17)