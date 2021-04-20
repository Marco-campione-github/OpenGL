#version 330

layout (location = 0) in vec3 position;

// Questa variabile di tipo float può essere settata dal main.
// Gli uniform sono variabili che servono per passare dei parametri
// allo shader.
uniform float scale;

void main()
{
    gl_Position = vec4(scale*position.x,scale*position.y,position.z,1.0);
}