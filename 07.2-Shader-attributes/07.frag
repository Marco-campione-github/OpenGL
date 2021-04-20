#version 330

// Questa variabile di input ha lo stesso nome di quella nel Vertex Shader
// E' importante che i nomi siano uguali perchè solo in questo modo si ha
// il passaggio delle informazioni.
//
// Attenzione: Il Fragment Shader viene chiamato ogni volta che bisogna stabilire
// il colore di un pixel del triangolo. Noi abbiamo solo i colori dei tre vertici.
// Come si definisscono i colori degli altri pixel?
// Tramite interpolazione dei colori dei vertici del triangolo. 
// In generale, gli attributi di un triangolo vengono interpolati e i loro valori
// se richiesti passati al Fragment Shader per ottenere gli attributi di tutti i 
// punti interni al triangolo stesso. Questo vale per tutti gli attributi.
//
// Il colore finale del triangolo sarà un mix di colori interpolati a partire da quelli 
// ai suoi vertici.

in vec3 fragcolor;

out vec4 color;

void main()
{
	// Ci limitiamo a passare il colore di input in output.
    // Dato che il colore di input è un vec3 e a noi serve un vec4, ne creiamo
    // uno al volo da quello di input aggiungendo la quarta componente.
    color = vec4(fragcolor, 1.0);
}