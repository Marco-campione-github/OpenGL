/**

  Questo codice mostra come usare le texture per colorare le superfici degli oggetti
  3D. 

  Questo codice è equivalente al precedente ma sono state fatte delle ristrutturazioni:
  
  Le istruzioni per definire il modello 3D e copiarlo sulla GPU, nonchè le istruzioni
  di rendering sono state spostate in una classe a parte: Cube.
  Anche l'oggetto MyShaderClass è stato spostato dentro la classe Cube in quanto è
  intimamente legato a come è definito il modello (i.e. attributi).

  Alla fine, nel main sono rimaste solo due istruzioni:
  1. L'istanziazione dell'oggetto Cube (nella struct global)
  2. La chiamata al rendering dell'oggetto (nella MyRenderScene)

  Oggetti diversi che usano gli stessi attributi di Cube, possono essere rappresentati
  in modo simile (in classi diverse) usando anche la stessa classe MyShaderClass.

  NOTA: la ristrutturazione è solo a scopo didattico, non è necessariamente 
        ottimizzata
*/


#include <iostream>
#include <sstream>
#include "GL/glew.h" // prima di freeglut
#include "GL/freeglut.h"
#include "glm/glm.hpp"

#include "transform.h"
#include "camera.h"

#include "cube.h"


/**
  Struttura di comodo dove sono memorizzate tutte le variabili globali
*/
struct global_struct {

  int WINDOW_WIDTH  = 1024; // Larghezza della finestra 
  int WINDOW_HEIGHT = 768; // Altezza della finestra

  Camera camera;

  AmbientLight   ambient_light;
  DiffusiveLight diffusive_light;
  SpecularLight  specular_light;

  Cube cube; // Oggetto di scena

  const float SPEED = 1;
  float gradX;
  float gradY; 

  global_struct() : gradX(0.0f), gradY(0.0f) {}

} global;



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

  glutPassiveMotionFunc(MyMouse);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_DEPTH_TEST);
}

void create_scene() {

  global.camera.set_perspective(
    45.0f,
    global.WINDOW_WIDTH,
    global.WINDOW_HEIGHT,
    0.1,
    100
  );

  global.ambient_light = AmbientLight(glm::vec3(1,1,1),0.2); 
  global.diffusive_light = DiffusiveLight(glm::vec3(1,1,1),glm::vec3(0,0,-1),0.5); // 0.5
  global.specular_light = SpecularLight(0.5,30);
}

void MyRenderScene() {
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  LocalTransform modelT;
  modelT.rotate(global.gradX, global.gradY ,0.0f);
  modelT.translate(0,0,-4);

  global.cube.shaders().set_model_transform(modelT.T());
  global.cube.shaders().set_camera_transform(global.camera.CP());
  global.cube.shaders().set_ambient_light(global.ambient_light);
  global.cube.shaders().set_diffusive_light(global.diffusive_light);
  global.cube.shaders().set_specular_light(global.specular_light);
  global.cube.shaders().set_camera_position(global.camera.position());

  // Renderizziamo il cubo usando il suo shader e la sua texture
  global.cube.render();

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
          glm::vec3(0, 0, 0),
          glm::vec3(0, 0,-1),
          glm::vec3(0, 1, 0)
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