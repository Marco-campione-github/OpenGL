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

// Struttura dati di lavoro per contenere le informazioni sulla luce
// speculare
struct SpecularLightStruct {
	float intensity;
	float shininess;
};


// Questa variabile di input ha lo stesso nome di quella nel Vertex Shader
// E' importante che i nomi siano uguali perchè solo in questo modo si ha
// il passaggio delle informazioni.
in vec3 fragment_color;

// Vettori della normali ricevuti dal vertex shader
in vec3 fragment_normal;

// Coordinate spaziali dei punti ricervuti dal vertex shader
in vec3 fragment_position;

// Coordinate di texture dei punti ricervuti dal vertex shader
in vec2 fragment_textcoord;

// Informazioni di luce ambientale 
uniform AmbientLightStruct AmbientLight;

// Informazioni di luce diffusiva 
uniform DiffusiveLightStruct DiffusiveLight;

// Informazioni di luce speculare 
uniform SpecularLightStruct SpecularLight;

// Posizione della camera in coordinate mondo
uniform vec3 CameraPosition;

uniform sampler2D TextSampler;

out vec4 out_color;

void main()
{
	// La funzione texture ritorna un vec4. Nel codice noi rappresentiamo
	// i colori con vec3 e dobbiamo quindi estrarre solo 3 componenti.
	vec3 material_color = texture(TextSampler, fragment_textcoord).rgb;

	vec3 I_amb =  material_color * (AmbientLight.color * AmbientLight.intensity);

	vec3 I_dif = vec3(0,0,0);

	vec3 normal = normalize(fragment_normal);

	float cosTheta = dot(normal,-DiffusiveLight.direction);

	if (cosTheta>0) {
		I_dif = material_color * (DiffusiveLight.color * DiffusiveLight.intensity) * cosTheta;
	}

	vec3 I_spec = vec3(0,0,0);

	vec3 view_dir    = normalize(CameraPosition - fragment_position);
	vec3 reflect_dir = normalize(reflect(DiffusiveLight.direction,normal));

	float cosAlpha = dot(view_dir,reflect_dir);
	if (cosAlpha>0) {
		I_spec = material_color * (DiffusiveLight.color * SpecularLight.intensity) * pow(cosAlpha,SpecularLight.shininess);
	}

	out_color = vec4(I_amb + I_dif + I_spec, 1.0);
}