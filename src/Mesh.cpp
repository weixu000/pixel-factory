#include <PixelFactory/components/Mesh.h>
#include <PixelFactory/gl/GlVertexArray.h>
#include <PixelFactory/gl/GlBuffer.h>
#include <PixelFactory/Entity.h>

Mesh::Mesh(const std::vector<GLuint> &indices, const std::vector<VertexNormalTex> &attributes)
    : count_(indices.size()), vao_(std::make_shared<GlVertexArray>()) {
  GlBuffer vbo(sizeof(VertexNormalTex) * attributes.size(), attributes.data());
  GlBuffer ebo(sizeof(GLuint) * indices.size(), indices.data());

  vao_->BindVertexBuffer(0, vbo, 0, sizeof(VertexNormalTex));
  vao_->SetAttribFormat<VertexNormalTex>(0);
  vao_->BindElementBuffer(ebo);
}

Mesh Mesh::FromObjFile(const std::string &file_path) {
  auto[indices, attributes] = LoadObjFile(file_path);
  return Mesh(indices, attributes);
}
