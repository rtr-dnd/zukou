// Cylinder is generated along y axis
// (seen from +y/-y, it looks like a circle)
//
// Circular caps are divided into 4 * angular_resolution segments.
//
// ex) angular_resolution = 3, viewing from +y
//
//  -z
//  --- __
//  |  /   -
//  | /   __--
//  |/ _--    -
//  |---------- +x
//  |\ --__   -
//  | \    ---
//  |  \  _-
//  --- .
//  +z
//
// Vertices are stored in this order:
// (the center point of top circle) => (top circle) => (2nd top circle) => ...
//            => (bottom circle) => (the center point of top circle)
//
// Each vertices of circle are stored counterclockwise when viewed from +y,
// starting from (x, z) = (+x, 0) (right-handed coordinate).
//
// for y axis, there are (vertical_resolution + 1) points.

#pragma once

#include <zukou.h>

#include <vector>

class Cylinder
{
  struct Vertex {
    Vertex(float x, float y, float z, float u, float v);
    float x, y, z;
    float u, v;
  };

 public:
  DISABLE_MOVE_AND_COPY(Cylinder);
  Cylinder() = delete;
  Cylinder(zukou::System* system, zukou::VirtualObject* virtual_object,
      int32_t vertical_resolution = 1, int32_t angular_resolution = 8,
      bool wire = false);
  ~Cylinder();

  void Bind(std::unique_ptr<zukou::GlTexture> texture);

  bool Render(float radius, float half_height, glm::mat4 transform);

 private:
  bool Init();

  void ConstructVertices();
  void ConstructElements();

  bool initialized_ = false;

  zukou::VirtualObject* virtual_object_;

  const int32_t vertical_resolution_;
  const int32_t angular_resolution_;
  const bool wire_;

  int fd_ = 0;
  zukou::ShmPool pool_;
  zukou::Buffer vertex_buffer_;
  zukou::Buffer element_array_buffer_;

  zukou::GlBuffer gl_vertex_buffer_;
  zukou::GlBuffer gl_element_array_buffer_;
  zukou::GlVertexArray vertex_array_;
  zukou::GlShader vertex_shader_;
  zukou::GlShader fragment_shader_;
  zukou::GlProgram program_;
  std::unique_ptr<zukou::GlTexture> texture_;
  zukou::GlSampler sampler_;

  zukou::RenderingUnit rendering_unit_;
  zukou::GlBaseTechnique base_technique_;

  std::vector<Vertex> vertices_;
  std::vector<ushort> elements_;

  inline size_t vertex_buffer_size();

  inline size_t element_array_buffer_size();

  inline size_t pool_size();
};

inline size_t
Cylinder::vertex_buffer_size()
{
  return sizeof(decltype(vertices_)::value_type) * vertices_.size();
}

inline size_t
Cylinder::element_array_buffer_size()
{
  return sizeof(decltype(elements_)::value_type) * elements_.size();
}

inline size_t
Cylinder::pool_size()
{
  return vertex_buffer_size() + element_array_buffer_size();
}
