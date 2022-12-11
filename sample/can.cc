#include <zukou.h>

#include "cylinder.h"
#include "jpeg-texture.h"

class Can final : public zukou::IBoundedDelegate, public zukou::ISystemDelegate
{
 public:
  Can(const char* texture_path)
      : system_(this),
        bounded_(&system_, this),
        texture_path_(texture_path),
        cylinder_(&system_, &bounded_, 8, 8, false)
  {}

  bool Init(float radius)
  {
    if (!system_.Init()) return false;
    if (!bounded_.Init(glm::vec3(radius))) return false;

    auto jpeg_texture = std::make_unique<JpegTexture>(&system_);

    if (!jpeg_texture->Init()) return false;
    if (!jpeg_texture->Load(texture_path_)) return false;

    cylinder_.Bind(std::move(jpeg_texture));

    return true;
  }

  int Run() { return system_.Run(); }

  void Configure(glm::vec3 half_size, uint32_t serial) override
  {
    float radius = glm::min(half_size.x, half_size.z);
    float height = half_size.y;
    cylinder_.Render(radius, height, glm::mat4(1));

    bounded_.AckConfigure(serial);
    bounded_.Commit();
  }

 private:
  zukou::System system_;
  zukou::Bounded bounded_;

  const char* texture_path_;

  Cylinder cylinder_;
};

const char* usage =
    "Usage: %s <texture>\n"
    "\n"
    "    texture: Surface texture in JPEG format"
    "\n";

int
main(int argc, char const* argv[])
{
  if (argc != 2) {
    fprintf(stderr, "jpeg-file is not specified\n\n");
    fprintf(stderr, usage, argv[0]);
    return EXIT_FAILURE;
  }

  Can can(argv[1]);

  if (!can.Init(0.1)) return EXIT_FAILURE;

  return can.Run();
}
