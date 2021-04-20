#version 330

layout (location = 0) in vec3 position;

uniform mat4 Model2World;

void main()
{
    gl_Position = Model2World * vec4(position, 1.0);
}