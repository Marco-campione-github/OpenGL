#version 330

// Struttura dati di lavoro per contenere le informazioni sulla luce
// ambientale
struct AmbientLightStruct {
	vec3 color;
	float intensity;
};


// Questa variabile di input ha lo stesso nome di quella nel Vertex Shader
// E' importante che i nomi siano uguali perchè solo in questo modo si ha
// il passaggio delle informazioni.
flat in vec3 fragment_color;

// Informazioni di luce ambientale 
uniform AmbientLightStruct AmbientLight;

// Componente ambientale del colore dell'oggetto 
vec3 I_amb;

out vec4 out_color;


void main()
{
	I_amb =  fragment_color * (AmbientLight.color * AmbientLight.intensity);

	out_color = vec4(I_amb, 1.0);
}