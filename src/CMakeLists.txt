add_library(PixelFactory STATIC GLContext.cpp ../include/PixelFactory/GLContext.h)
target_link_libraries(PixelFactory PUBLIC glad glfw)
target_include_directories(PixelFactory PUBLIC ../include)
target_compile_features(PixelFactory PUBLIC cxx_std_17)