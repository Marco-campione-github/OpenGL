/**
  Questo codice introduce la classe astratta ShaderClass che serve a definire
  le operazioni base di gestione degli shader come il caricamento dei file, 
  linking programma e gestione degli errori. La classe deve essere usata come
  classe base per definire una propria classe di gestione degli shader che viene
  customizzata con le funzionalità specifiche degli shader usati.

  Ad esempio mettendo dei metodi pubblici per settare più facilmente i parametri 
  sparsi nei vari shader.

  Essendo una classe astratta, è obbligatorio implementare (adattandoli di volta in volta)
  i metodi privati load_shaders e load_done che eseguono le operazioni per caricare
  gli shader necessari (load_shaders) ed eseguire le operazioni post-caricamento
  (load_done) che consistono nel recupero degli handler dei parametri che vogliamo
  manipolare. Gli handler sono memorizzati come dati membro della nostra classe.

  Per leggibilità la classe concreta MyShaderClass usata in questo codice è definita 
  nei file myshaderclass.cpp e myshaderclass.h
  Con la classe è possibile settare separatamente:
   la trasformazione del modello
   la trasformazione di camera
  

  Per facilitare l'individuazione delle nuove istruzioni, è stata eliminata la maggior
  parte dei commenti vecchi.

*/


#include <iostream>
#include <sstream>
#include "GL/glew.h" // prima di freeglut
#include "GL/freeglut.h"
#include "glm/glm.hpp"

#include "utilities.h"
#include "transform.h"
#include "camera.h"
#include "shaderclass.h"
#include "myshaderclass.h"


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

  const float SPEED = 10;
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
  Vertex(float x, float y, float z, float r, float g, float b) {
      position = glm::vec3(x,y,z);
      color = glm::vec3(r,g,b);
  }

  Vertex(const glm::vec3 &xyz, const glm::vec3 &rgb) : position(xyz), color(rgb) {}
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

  Vertex Vertices[4] = {
      Vertex(glm::vec3(-1.0f, -1.0f,-1.0f), glm::vec3( 1.0f, 0.0f, 0.0f)),
      Vertex(glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3( 0.0f, 1.0f, 0.0f)),
      Vertex(glm::vec3( 0.0f,  1.0f,-1.0f), glm::vec3( 0.0f, 0.0f, 1.0f)),
      Vertex(glm::vec3( 1.0f, -1.0f,-1.0f), glm::vec3( 1.0f, 1.0f, 0.0f))
  };

  unsigned int Indices[] = {
      1, 2, 0,
      3, 2, 1,
      3, 0, 2,
      1, 0, 3        
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

  GLuint IBO; // Index Buffer Object
  glGenBuffers(1, &IBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

  global.camera.set_camera(
      glm::vec3(0,0,0),
      glm::vec3(0,0,-1),
      glm::vec3(0,1,0)
  );

  global.camera.set_perspective(
      45.0f,
      global.WINDOW_WIDTH,
      global.WINDOW_HEIGHT,
      0.1,
      100);

  // Inizializza la classe degli shaders
  // Vengono caricati gli shader indicati nel metodo init e 
  // automaticamente linkato il programma 
  if (!global.shaders.init()) {
      std::cerr << "Error initializing shaders..." << std::endl;
      exit(0);
  }

  // Abilitiamo il programma degli shader
  global.shaders.enable();
}

void MyRenderScene() {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  LocalTransform modelT;
  modelT.rotate(global.gradX, global.gradY ,0.0f);
  modelT.translate(0,0,-4);

  global.shaders.set_model_transform(modelT.T());
  global.shaders.set_camera_transform(global.camera.CP());

  glBindVertexArray(global.VAO);

  glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

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