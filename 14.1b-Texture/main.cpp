/**

  Questo codice mostra come usare le texture per colorare le superfici degli oggetti
  3D. Per gestire una texture è stata creata una classe Texture che si occupa di
  caricare in memoria l'immagine della texture e attivarla quando deve essere usata 
  per il rendering. 
  
  Il caricamento della texture da un file immagine si appoggia ad una librerie
  esterna nel file stb_image.h

  La texture è caricata nella funzione create_scene.

  Per usare una texture, è necessario aggiungere ai vertici, gli attributi delle
  coordinate di texture. Per questo motivo, la struct Vertex è stata estesa.

  Ora i vertici hanno quattro attributi: 
  posizione dei vertici, colore, normali, coordinate di texture associate. 

  Al momento il colore dei vertici non è usato nello shader ma è stato 
  comunque lasciato nel codice.

  Quando si vuole usare una texture, è necessario farne il bind nel contesto 
  associandola ad una Texture Unit. 
  Una Texture Unit è responsabile dell'estrazione dei pixel dalla texture (sampling).
  L'informazione di quale TextureUnit deve essere usata per il sampling dei pixel deve 
  essere passata nel fragment shader e usata da una apposita funzione.
  La classe MyShaderClass è stata estesa per supportare il settaggio della texture unit.

  Tutto questo è fatto nella funzione render_scene.
*/


#include <iostream>
#include <sstream>
#include "GL/glew.h" // prima di freeglut
#include "GL/freeglut.h"
#include "glm/glm.hpp"

#include "transform.h"
#include "camera.h"
#include "shaderclass.h"
#include "myshaderclass.h"

#include "texture.h"

/**
  Struttura di comodo dove sono memorizzate tutte le variabili globali
*/
struct global_struct {

  int WINDOW_WIDTH  = 1024; // Larghezza della finestra 
  int WINDOW_HEIGHT = 768; // Altezza della finestra

  GLuint VAO; // Vertex Array Object

  Camera camera;

  // Istanza della classe di gestione degli shader 
  MyShaderClass shaders;

  AmbientLight   ambient_light;
  DiffusiveLight diffusive_light;
  SpecularLight  specular_light;

  Texture cube_texture;

  const float SPEED = 1;
  float gradX;
  float gradY; 

  global_struct() : gradX(0.0f), gradY(0.0f) {}

} global;

/**
  Struttura dati che contiene gli attributi di un vertice.
*/
struct Vertex {
  glm::vec3 position; ///< Coordinate spaziali
  glm::vec3 color;    ///< Colore
  glm::vec3 normal;   ///< Normale
  glm::vec2 textcoord;///< Coordinate di texture

  Vertex(
      float x, float y, float z, 
      float r, float g, float b, 
      float xn, float yn, float zn,
      float s, float t) {
      
      position = glm::vec3(x,y,z);
      color = glm::vec3(r,g,b);
      normal = glm::vec3(xn,yn,zn);
      textcoord = glm::vec2(s,t);
  }

  Vertex(const glm::vec3 &xyz, const glm::vec3 &rgb, const glm::vec3 &norm, const glm::vec2 &txt) 
  : position(xyz), color(rgb), normal(norm), textcoord(txt) {}
};


/**
Prototipi della nostre funzioni di callback. 
Sono definite più avanti nel codice.
*/
void MyRenderScene(void);
void MyIdle(void);
void MyKeyboard(unsigned char key, int x, int y);
void MyClose(void);
void MySpecialKeyboard(int Key, int x, int y);
void MyMouse(int x, int y);

void init(int argc, char*argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);


  glutInitWindowSize(global.WINDOW_WIDTH, global.WINDOW_HEIGHT);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Informatica Grafica");

/*
  // Imposta la modalità a schemo intero e nasconde il cursore
  std::stringstream game_mode;
  game_mode << global.WINDOW_WIDTH << "x" << global.WINDOW_HEIGHT << ":32";
  glutGameModeString(game_mode.str().c_str());
  glutEnterGameMode();
*/
  glutSetCursor(GLUT_CURSOR_NONE);

  // Portiamo il mouse fisso al centro della finestra
  global.camera.set_mouse_init_position(global.WINDOW_WIDTH/2, global.WINDOW_HEIGHT/2);
  global.camera.lock_mouse_position(true);
  glutWarpPointer(global.WINDOW_WIDTH/2, global.WINDOW_HEIGHT/2);

 // Must be done after glut is initialized!
  GLenum res = glewInit();
  if (res != GLEW_OK) {
      std::cerr<<"Error : "<<glewGetErrorString(res)<<std::endl;
    exit(1);
  }

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  glutDisplayFunc(MyRenderScene);

  glutKeyboardFunc(MyKeyboard);

  glutCloseFunc(MyClose);

  glutSpecialFunc(MySpecialKeyboard);

  // Callback per la gesione degli eventi del mouse.
  glutPassiveMotionFunc(MyMouse);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_DEPTH_TEST);
}

void create_scene() {
  Vertex Vertices[] = {
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1,0,0), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(1,0)),
    Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1,0,0), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(0,1)), 
    Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1,0,0), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(0,0)), 
    Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1,0,0), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(0,1)), 
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1,0,0), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(1,0)), 
    Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(1,0,0), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(1,1)), 

    Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0,1,0), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(0,0)),
    Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0,1,0), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(1,0)),
    Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0,1,0), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(1,1)),
    Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0,1,0), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(1,1)),
    Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0,1,0), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(0,1)),
    Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0,1,0), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(0,0)),

    Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0,0,1), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1,1)),
    Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0,0,1), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0,1)),
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0,0,1), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0,0)),
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0,0,1), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0,0)),
    Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0,0,1), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1,0)),
    Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0,0,1), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1,1)),

    Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1,1,0), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(0,1)),
    Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1,1,0), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(1,0)),
    Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1,1,0), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(1,1)),
    Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1,1,0), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(1,0)),
    Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1,1,0), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(0,1)),
    Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1,1,0), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(0,0)),

    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1,0,1), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(0,0)),
    Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1,0,1), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(0,1)),
    Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1,0,1), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(1,1)),
    Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1,0,1), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(1,1)),
    Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(1,0,1), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(1,0)),
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1,0,1), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(0,0)),

    Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0,1,1), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(0,1)),
    Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0,1,1), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(1,0)),
    Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0,1,1), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(1,1)),
    Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0,1,1), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(1,0)),
    Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0,1,1), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(0,1)),
    Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0,1,1), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(0,0))
  };
  
  glGenVertexArrays(1, &(global.VAO));
  glBindVertexArray(global.VAO);
 
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
    reinterpret_cast<GLvoid*>(offsetof(struct Vertex, position)));
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
    reinterpret_cast<GLvoid*>(offsetof(struct Vertex, color)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
    reinterpret_cast<GLvoid*>(offsetof(struct Vertex, normal)));
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
    reinterpret_cast<GLvoid*>(offsetof(struct Vertex, textcoord)));
  glEnableVertexAttribArray(3);

  global.camera.set_perspective(
    45.0f,
    global.WINDOW_WIDTH,
    global.WINDOW_HEIGHT,
    0.1,
    100
  );

  // Inizializza la classe degli shaders
  // Vengono caricati gli shader indicati nel metodo init e 
  // automaticamente linkato il programma 
  if (!global.shaders.init()) {
    std::cerr << "Error initializing shaders..." << std::endl;
    exit(0);
  }


  // Abilitiamo il programma degli shader
  global.shaders.enable();

  global.ambient_light = AmbientLight(glm::vec3(1,1,1),0.2); 
  global.diffusive_light = DiffusiveLight(glm::vec3(1,1,1),glm::vec3(0,0,-1),0.5); // 0.5
  global.specular_light = SpecularLight(0.5,30);

  // Carichiamo in memoria la texture
  global.cube_texture.load("test.png");
}

void MyRenderScene() {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  LocalTransform modelT;
  modelT.rotate(global.gradX, global.gradY ,0.0f);
  modelT.translate(0,0,-4);

  global.shaders.set_model_transform(modelT.T());
  global.shaders.set_camera_transform(global.camera.CP());
  global.shaders.set_ambient_light(global.ambient_light);
  global.shaders.set_diffusive_light(global.diffusive_light);
  global.shaders.set_specular_light(global.specular_light);
  global.shaders.set_camera_position(global.camera.position());

  // Attiviamo la texture e bindiamola alla texture unit 0 
  global.cube_texture.bind(0);

  // Settiamo l'oggetto TextSampler nello shader affinchè sia associato
  // alla texture unit 0
  global.shaders.set_sampler(0);

  glBindVertexArray(global.VAO);

  glDrawArrays(GL_TRIANGLES, 0, 36);

  glBindVertexArray(0);

  glutSwapBuffers();

}

// Funzione globale che si occupa di gestire l'input da tastiera.
void MyKeyboard(unsigned char key, int x, int y) {
  switch ( key )
  {
    case 27: // Escape key
      glutDestroyWindow(glutGetWindow());
      return;
    break;

    case 'a':
      global.gradY -= global.SPEED;
    break;
    case 'd':
      global.gradY += global.SPEED;
    break;
    case 'w':
      global.gradX -= global.SPEED;
    break;
    case 's':
      global.gradX += global.SPEED;
    break;

    // Variamo l'intensità di luce ambientale
    case '1':
      global.ambient_light.dec(0.05);
    break;

    // Variamo l'intensità di luce ambientale
    case '2':
      global.ambient_light.inc(0.05);
    break;

    // Variamo l'intensità di luce diffusiva
    case '3':
      global.diffusive_light.dec(0.05);
    break;

    // Variamo l'intensità di luce diffusiva
    case '4':
      global.diffusive_light.inc(0.05);
    break;

    // Variamo l'intensità di luce speculare
    case '5':
      global.specular_light.dec(0.05);
    break;

    // Variamo l'intensità di luce speculare
    case '6':
      global.specular_light.inc(0.05);
    break;

    // Variamo l'esponente della luce speculare
    case '7':
      global.specular_light.dec_shine(1);
    break;

    // Variamo l'esponente della luce speculare
    case '8':
      global.specular_light.inc_shine(1);
    break;

    case ' ': // Reimpostiamo la camera
      global.camera.set_camera(
          glm::vec3(0,0,0),
          glm::vec3(0,0,-1),
          glm::vec3(0,1,0)
      );
    break;
  }

  glutPostRedisplay();
}

void MySpecialKeyboard(int Key, int x, int y) {
  global.camera.onSpecialKeyboard(Key);
  glutPostRedisplay();
}


void MyMouse(int x, int y) {
  if (global.camera.onMouse(x,y)) {
    // Risposto il mouse al centro della finestra
    glutWarpPointer(global.WINDOW_WIDTH/2, global.WINDOW_HEIGHT/2);
  }
  glutPostRedisplay();
}


// Funzione globale che si occupa di gestire la chiusura della finestra.
void MyClose(void) {
  std::cout << "Tearing down the system..." << std::endl;
  // Clean up here

  // A schermo intero dobbiamo uccidere l'applicazione.
  exit(0);
}

int main(int argc, char* argv[])
{
  init(argc,argv);

  create_scene();

  glutMainLoop();
  
  return 0;
}