#include "myshaderclass.h"
#include "utilities.h"

void MyShaderClass::set_model_transform(const glm::mat4 &transform) {
  glUniformMatrix4fv(_model_transform_location, 1, GL_FALSE, const_cast<float *>(&transform[0][0]));       
}

void MyShaderClass::set_camera_transform(const glm::mat4 &transform) {
  glUniformMatrix4fv(_camera_transform_location, 1, GL_FALSE, const_cast<float *>(&transform[0][0]));       
}


bool MyShaderClass::load_shaders() {
  return  add_shader(GL_VERTEX_SHADER,"12.vert") &&
          add_shader(GL_FRAGMENT_SHADER,"12.frag");
}

bool MyShaderClass::load_done() {
  _model_transform_location = get_uniform_location("Model2World");
  _camera_transform_location = get_uniform_location("World2Camera");


  return  (_model_transform_location != INVALID_UNIFORM_LOCATION) &&
          (_camera_transform_location != INVALID_UNIFORM_LOCATION);
}
