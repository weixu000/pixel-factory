#include <PixelFactory/components/PointLight.h>
#include <PixelFactory/ModelLoader.h>
#include <PixelFactory/gl/GlVertexArray.h>
#include <PixelFactory/gl/GlBuffer.h>
#include <PixelFactory/gl/GlTextureCubemap.h>

PointLight::PointLight()
    : shadow_map(std::make_unique<GlTextureCubemap>(1, GL_DEPTH_COMPONENT16, 1024)) {
  shadow_map->SetFilter(GL_NEAREST, GL_NEAREST);

  if (vao == nullptr) {
    auto[indices, attributes] = LoadVerticesFromObjFile("meshes/sphere.obj");

    count = indices.size();

    vao = std::make_unique<GlVertexArray>();

    GlBuffer vbo(sizeof(Vertex) * attributes.size(), attributes.data());
    GlBuffer ebo(sizeof(GLuint) * indices.size(), indices.data());

    vao->BindVertexBuffer(0, vbo, 0, sizeof(Vertex));
    vao->SetAttribFormat<Vertex>(0);
    vao->BindElementBuffer(ebo);
  }
}
