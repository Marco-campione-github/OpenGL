#version 330

// Struttura dati di lavoro per contenere le informazioni sulla luce
// ambientale
struct AmbientLightStruct {
	vec3 color;
	float intensity;
};

// Struttura dati di lavoro per contenere le informazioni sulla luce
// diffusiva
struct DiffusiveLightStruct {
	vec3 color;
	vec3 direction;
	float intensity;
};

// Questa variabile di input ha lo stesso nome di quella nel Vertex Shader
// E' importante che i nomi siano uguali perchè solo in questo modo si ha
// il passaggio delle informazioni.
in vec3 fragment_color;

// Vettori della normali ricevuti dal vertex shader
in vec3 fragment_normal;


// Informazioni di luce ambientale 
uniform AmbientLightStruct AmbientLight;

// Informazioni di luce diffusiva 
uniform DiffusiveLightStruct DiffusiveLight;


out vec4 out_color;


void main()
{
	vec3 I_amb =  fragment_color * (AmbientLight.color * AmbientLight.intensity);

	vec3 I_dif = vec3(0,0,0);

	// Normalizziamo il vettore delle normali
	vec3 normal = normalize(fragment_normal);

	// La direzione della luce è un vettore che parte dalla luce fino 
	// agli oggetti. E' necessario invertire la direzione prima di 
	// calcolare il fattore coseno.
	// Bisogna sempre essere sicuri di avere le normali in forma di 
	// versori  
	float cosTheta = dot(normal,-DiffusiveLight.direction);

	if (cosTheta>0) {
		I_dif = fragment_color * (DiffusiveLight.color * DiffusiveLight.intensity) * cosTheta;
	}


	out_color = vec4(I_amb + I_dif, 1.0);
}