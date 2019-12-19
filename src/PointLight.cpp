

#include <PixelFactory/PointLight.h>
#include <PixelFactory/ModelLoader.h>
#include <PixelFactory/GL/GLVertexArray.h>
#include <PixelFactory/GL/GLBuffer.h>

PointLight::PointLight() {
  if (vao == nullptr) {
    auto[indices, attributes] = LoadVerticesFromObjFile("meshes/sphere.obj");

    count = indices.size();

    vao = std::make_unique<GLVertexArray>();
    vao->Bind();
    GLBuffer vbo, ebo;

    vbo.Bind(GLBuffer::Target::ArrayBuffer);
    vbo.Upload(sizeof(Vertex) * attributes.size(), attributes.data());
    vao->SetAttribPointer<Vertex>();
    vbo.Unbind();

    ebo.Bind(GLBuffer::Target::ElementArrayBuffer);
    // Pass in the data.
    ebo.Upload(sizeof(GLuint) * indices.size(), indices.data());

    // Unbind from the VAO.
    vao->Unbind();
  }
}
