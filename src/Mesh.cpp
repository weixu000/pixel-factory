#include <PixelFactory/components/Mesh.h>
#include <PixelFactory/GL/GLVertexArray.h>
#include <PixelFactory/GL/GLBuffer.h>
#include <PixelFactory/Entity.h>

Mesh::Mesh(const std::vector<GLuint> &indices, const std::vector<VertexNormalTex> &attributes)
    : count_(indices.size()), vao_(std::make_shared<GLVertexArray>()) {
  vao_->Bind();
  GLBuffer vbo, ebo;

  vbo.Bind(GLBuffer::Target::ArrayBuffer);
  vbo.Upload(sizeof(VertexNormalTex) * attributes.size(), attributes.data());
  vao_->SetAttribPointer<VertexNormalTex>();
  vbo.Unbind();

  ebo.Bind(GLBuffer::Target::ElementArrayBuffer);
  // Pass in the data.
  ebo.Upload(sizeof(GLuint) * indices.size(), indices.data());

  // Unbind from the VAO.
  vao_->Unbind();
}

Mesh Mesh::FromObjFile(const std::string &file_path) {
  auto[indices, attributes] = LoadObjFile(file_path);
  return Mesh(indices, attributes);
}
