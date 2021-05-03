#include "light.h"

AmbientLight::AmbientLight() :
	_color(glm::vec3(1.0,1.0,1.0)), _intensity(1.0) {}

AmbientLight::AmbientLight(const glm::vec3 &col, float i) :
	_color(col), _intensity(i) {}

void AmbientLight::inc(float value) {
	_intensity += value;
	if (_intensity>1.0) _intensity = 1.0;
}

void AmbientLight::dec(float value) {
	_intensity -= value;
	if (_intensity < 0.0) _intensity = 0.0;
}

glm::vec3 AmbientLight::color() const {
	return _color;
}

float AmbientLight::intensity() const {
	return _intensity;
}


DiffusiveLight::DiffusiveLight() :
	_color(glm::vec3(1.0f,1.0f,1.0f)), 
	_direction(glm::vec3(0.0f,-1.0f,0.0f)),
	_intensity(1.0f) {}

DiffusiveLight::DiffusiveLight(const glm::vec3 &col, const glm::vec3 &dir, float i) :
	_color(col), _direction(dir), _intensity(i) {}

void DiffusiveLight::inc(float value) {
	_intensity += value;
	if (_intensity>1.0) _intensity = 1.0;
}

void DiffusiveLight::dec(float value) {
	_intensity -= value;
	if (_intensity < 0.0) _intensity = 0.0;
}

glm::vec3 DiffusiveLight::color() const {
	return _color;
}

glm::vec3 DiffusiveLight::direction() const {
	return _direction;
}

float DiffusiveLight::intensity() const {
	return _intensity;
}


SpecularLight::SpecularLight() :
  _shininess(30.0f),
  _intensity(1.0f) {}

SpecularLight::SpecularLight(float i, float s) :
    _intensity(i), _shininess(s) {}

void SpecularLight::inc(float value) {
  _intensity += value;
    if (_intensity>1.0) _intensity = 1.0;
}

void SpecularLight::dec(float value) {
  _intensity -= value;
  if (_intensity < 0.0) _intensity = 0.0;
}

void SpecularLight::inc_shine(float value) {
  _shininess += value;
}

void SpecularLight::dec_shine(float value) {
  _shininess -= value;
  if (_shininess < 0.0) _shininess = 0.0;
}

float SpecularLight::intensity() const {
  return _intensity;
}

float SpecularLight::shininess() const {
  return _shininess;
}