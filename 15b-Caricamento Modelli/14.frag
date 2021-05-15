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
	// La funzione texture ritorna un vec4. 
	vec4 material_color = texture(TextSampler, fragment_textcoord);

	vec3 amb =  (AmbientLight.color * AmbientLight.intensity);

	vec3 dif = vec3(0,0,0);

	vec3 normal = normalize(fragment_normal);

	float cosTheta = dot(normal,-DiffusiveLight.direction);

	if (cosTheta>0) {
		dif = (DiffusiveLight.color * DiffusiveLight.intensity) * cosTheta;
	}

	vec3 spec = vec3(0,0,0);

	vec3 view_dir    = normalize(CameraPosition - fragment_position);
	vec3 reflect_dir = normalize(reflect(DiffusiveLight.direction,normal));

	float cosAlpha = dot(view_dir,reflect_dir);
	if (cosAlpha>0) {
		spec = (DiffusiveLight.color * SpecularLight.intensity) * pow(cosAlpha,SpecularLight.shininess);
	}

	out_color = vec4(material_color.rgb*(amb + dif + spec), material_color.a);
}