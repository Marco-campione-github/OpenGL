#ifndef CUBE_H
#define CUBE_H

#include <string>
#include "GL/glew.h" // prima di freeglut
#include "texture.h"
#include "myshaderclass.h"

class Cube {
public:

  /**
    Costruttore
  */
  Cube();

  /**
    Renderizza l'oggetto. Se necessario, l'oggetto viene prima inizializzato
    chiamando la init().
  */
  void render(void);

  /**
    Metodo che dà accesso al dato shader di Cube. Serve per impostare le
    varie proprietà dello shader (es. luci).
  */
  MyShaderClass &shaders();

private:
  GLuint _VAO;       // Vertex Array Object
  Texture _texture;  // Texture
  bool _initialized; // Oggetto usabile?

  MyShaderClass _shaders; // Shaders da usare con Cube

  /**
    Inizializzazione degli oggetti OpenGL. E' fatta in un secondo tempo
    poichè è necessario avere inizializzato GLUT prima di qualunque
    uso delle funzioni di OpenGL. La init è chiamata, se necessario,
    dalla render().
  */
  void init(void);

  // Blocchiamo le operazioni di copia di una istanza di Cube:
  // non possiamo condividere un VAO
  Cube&operator=(const Cube &other);
  Cube(const Cube &other);

};

#endif 