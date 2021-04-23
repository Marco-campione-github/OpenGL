#include "myshaderclass.h"
#include "utilities.h"

void MyShaderClass::set_model_transform(const glm::mat4 &transform) {
    glUniformMatrix4fv(_model_transform_location, 1, GL_FALSE, const_cast<float *>(&transform[0][0]));       
}

void MyShaderClass::set_camera_transform(const glm::mat4 &transform) {
    glUniformMatrix4fv(_camera_transform_location, 1, GL_FALSE, const_cast<float *>(&transform[0][0]));       
}

void MyShaderClass::set_ambient_light(const AmbientLight &al) {
    glUniform3fv(_ambient_color_location, 1, const_cast<float *>(&al.color()[0]));
    glUniform1f(_ambient_intensity_location, al.intensity());
}

bool MyShaderClass::load_shaders() {
    return  add_shader(GL_VERTEX_SHADER,"13.vert") &&
            add_shader(GL_FRAGMENT_SHADER,"13.frag");
}

bool MyShaderClass::load_done() {
    _model_transform_location = get_uniform_location("Model2World");
    _camera_transform_location = get_uniform_location("World2Camera");

    _ambient_color_location     = get_uniform_location("AmbientLight.color");
    _ambient_intensity_location = get_uniform_location("AmbientLight.intensity");

    return  (_model_transform_location != INVALID_UNIFORM_LOCATION) &&
            (_camera_transform_location != INVALID_UNIFORM_LOCATION) &&
            (_ambient_color_location != INVALID_UNIFORM_LOCATION) &&
            (_ambient_intensity_location != INVALID_UNIFORM_LOCATION);
}
