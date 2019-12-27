#include "PixelFactory/components/PointLight.h"

#include "PixelFactory/ModelLoader.h"
#include "PixelFactory/gl/GlBuffer.h"
#include "PixelFactory/gl/GlContext.h"
#include "PixelFactory/gl/GlTextureCubemap.h"
#include "PixelFactory/gl/GlVertexArray.h"

PointLight::PointLight(GlContext &context) : context_(context) {
  shadow_map =
      std::make_unique<GlTextureCubemap>(context_.CreateTextureCubemap());
  shadow_map->ImmutableStorage(1, GL_DEPTH_COMPONENT16, 1024);
  shadow_map->SetFilter(GL_NEAREST, GL_NEAREST);

  if (vao == nullptr) {
    auto [indices, attributes] = LoadVerticesFromObjFile("meshes/sphere.obj");

    count = indices.size();

    auto vbo = context_.CreateBuffer();
    vbo.ImmutableStorage(sizeof(Vertex) * attributes.size(), attributes.data());
    auto ebo = context_.CreateBuffer();
    ebo.ImmutableStorage(sizeof(GLuint) * indices.size(), indices.data());

    vao = std::make_unique<GlVertexArray>(context_.CreateVertexArray());
    vao->BindVertexBuffer(0, vbo, 0, sizeof(Vertex));
    vao->SetAttribFormat<Vertex>(0);
    vao->BindElementBuffer(ebo);
  }
}
