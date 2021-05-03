#ifndef TEXTURE_H
#define TEXTURE_H

#include "GL/glew.h" // prima di freeglut
#include <string>

/**
	Classe di supporto per caricare in memoria una texture da un file.

	Supporta solo immagini PNG. 
*/
class Texture {
public:
	/**
		Costruttore
	*/
	Texture();

	/**
		Carica in memoria una texture di tipo TextureTarget presa da
		file.

		@param TextureTarget tipo di texture (es. GL_TEXTURE_2D)
		@param FileName nome del file 

		@throw una eccezione ivllib::ImageFormatError in caso di errore
	*/
	bool load(const std::string& FileName);

	/**
		Attiva la textureUnit indicata e binda la texture ad essa.
	*/
	void bind(int TextureUnit) const;

	/**
		Controllo di validità della texture.
		@return true se la texture è stata caricata correttamente.
	*/
	bool is_valid(void) const;

private:
    std::string _filename; ///<< Nome del file
    GLenum _target; ///<< Tipo di texture
    GLuint _texture; ///<< Oggetto OpenGL che rappresenta la texture
    bool _valid; ///<< Flag di validità
};

#endif