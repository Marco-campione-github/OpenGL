#include "camera.h"

#define  GLM_FORCE_RADIANS
#include "glm/gtc/matrix_transform.hpp"

#include "GL/freeglut.h"

#include <iostream>

static std::ostream &operator<<(std::ostream &os, const glm::vec3 &v) {
	for(int y=0; y<3; ++y) {
		os<<v[y]<<" ";
	}
	return os;
}

Camera::Camera() {
	reset();
	_speed = 0.05f;
}

void Camera::reset() {
	_combined   = _projection = _camera = glm::mat4(1.0f);
	_position   = glm::vec3(0,0,0);
	_lookat_dir = glm::vec3(0,0,-1);
	_up         = glm::vec3(0,1,0);

}

void Camera::update() {
	_combined = _projection * _camera;  
}

const glm::mat4& Camera::CP() const {
	return _combined;
}

void Camera::set_camera(const glm::vec3 &position, const glm::vec3 &lookat, const glm::vec3 &up){
	_position = position;
	_up = up;

	_lookat_dir = glm::normalize(lookat - _position);

	_camera = camera_setting(_position,_lookat_dir+_position,_up);
	update();
}

glm::mat4 Camera::camera_setting(const glm::vec3 &position, const glm::vec3 &lookat, const glm::vec3 &up) {

	glm::mat4 V = glm::lookAt(position,lookat,up);

	return V;
}

const glm::mat4& Camera::camera() const {
	return _camera;
}

void Camera::set_perspective(float FOVDeg, float width, float height, float znear, float zfar) {
	_projection = perspective_projection(FOVDeg,width,height,znear,zfar);
	update();
}

glm::mat4 Camera::perspective_projection(float FOVDeg, float width, float height, float znear, float zfar) {
	assert(zfar > znear);
	assert(width>0);
	assert(height>0);

  glm::mat4 p = glm::perspective(
    	glm::radians(FOVDeg),
    	width/height,
    	znear, 
    	zfar);

	return p;
}

const glm::mat4& Camera::projection() const {
	return _projection;
}

void Camera::set_speed(float speed) {
	_speed = speed;
}

const float& Camera::speed() const {
	return _speed;
}

bool Camera::onSpecialKeyboard(int key) {
	glm::vec3 tmp;

	// Lo spostamento in avanti/indietro avviene nella direzione
	// di lookat.

	// Lo spostamento a sinistra o a destra avviene nella direzione dell'asse
	// identificato dal prodotto vettoriale tra up e lookat.

	switch (key) {
		case GLUT_KEY_UP:
			_position += (_lookat_dir * _speed);
		break;

		case GLUT_KEY_DOWN:
			_position -= (_lookat_dir * _speed);
		break;
		
		case GLUT_KEY_LEFT:
			tmp = glm::cross(_up,_lookat_dir);
			tmp = glm::normalize(tmp);
			_position += (tmp * _speed);
		break;
		
		case GLUT_KEY_RIGHT:
			tmp = glm::cross(_lookat_dir,_up);
			tmp = glm::normalize(tmp);
			_position += (_speed * tmp);
		break;

		default:
			return false;
		break;
	}

	_camera = camera_setting(_position, _position + _lookat_dir, _up);
	update();

	return true;
}