#include <PixelFactory/components/Mesh.h>
#include <PixelFactory/gl/GlContext.h>
#include <PixelFactory/gl/GlVertexArray.h>
#include <PixelFactory/gl/GlBuffer.h>
#include <PixelFactory/Entity.h>

Mesh::Mesh(GlContext &context, const std::vector<GLuint> &indices, const std::vector<VertexNormalTex> &attributes)
    : context_(context), count_(indices.size()) {
  auto vbo = context_.CreateBuffer();
  vbo.ImmutableStorage(sizeof(VertexNormalTex) * attributes.size(), attributes.data());
  auto ebo = context_.CreateBuffer();
  ebo.ImmutableStorage(sizeof(GLuint) * indices.size(), indices.data());

  vao_ = std::make_shared<GlVertexArray>(context_.CreateVertexArray());
  vao_->BindVertexBuffer(0, vbo, 0, sizeof(VertexNormalTex));
  vao_->SetAttribFormat<VertexNormalTex>(0);
  vao_->BindElementBuffer(ebo);
}

Mesh Mesh::FromObjFile(GlContext &context, const std::string &file_path) {
  auto[indices, attributes] = LoadObjFile(file_path);
  return Mesh(context, indices, attributes);
}
