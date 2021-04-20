#version 330

// In questa istruzione stiamo dicendo allo shader che le informazioni
// contenute nell'attributo di posizione/indice location=0 devono
// essere memorizzate nella variabile, di tipo vec3, position
// Questo indice è lo stesso che è stato indicato nel file main.cpp
// con le istruzioni 
// glEnableVertexAttribArray(0)
// glVertexAttribPointer(0,...)
layout (location = 0) in vec3 position;

void main()
{
	// Negli shader si usano le coordinate omogenee
	// In output mandiamo un punto in coordinate omogenee che ha
	// le coordinate x e y del punto di input scalate di un fattore 0.8. 
	// gl_Position è una variabile predefinita OpenGL 
    gl_Position = vec4(0.8*position.x,0.8*position.y,position.z,1.0);
}