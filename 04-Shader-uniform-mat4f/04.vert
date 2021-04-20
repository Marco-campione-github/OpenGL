#version 330

layout (location = 0) in vec3 position;

// Model2World è una matrice 4x4. Viene settata dal main.cpp
uniform mat4 Model2World;

void main()
{
	// position è un vettore di lunghezza 3.
	// Le trasformazioni vanno fatte in coordinate omogenee
	// Dobbiamo usare un vettore di lunghezza 4.
	// E' possibile riempire un vettore vec4 da un ve3 aggiungendogli il
	// quarto valore. In questo caso, essendo dei punti, il quarto valore
	// in coordinate omogenee di un punto vale sempre 1.

    gl_Position = Model2World * vec4(position, 1.0);
}