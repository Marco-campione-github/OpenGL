#version 330

// Questa variabile di input ha lo stesso nome di quella nel Vertex Shader
// E' importante che i nomi siano uguali perchè solo in questo modo si ha
// il passaggio delle informazioni.
//
// Notate l'uso della keyword modificatore flat all'inizio.
//
// Di base gli attributi sono interpolati. Se però si usa la keyword flat come 
// modificatore della variabile, quello che succede è che non viene abilitata 
// l'interpolazione degli attributi ma viene sempre usato l'attributo 
// dell'ultimo punto del triangolo.
//
// Il colore finale del triangolo sarà il colore dell'ultimo vertice 
// del triangolo (blu).
flat in vec3 fragcolor;

out vec4 color;

void main()
{
	// Ci limitiamo a passare il colore di input in output.
    // Dato che il colore di input è un vec3 e a noi serve un vec4, ne creiamo
    // uno al volo da quello di input aggiungendo la quarta componente.
    color = vec4(fragcolor, 1.0);
}