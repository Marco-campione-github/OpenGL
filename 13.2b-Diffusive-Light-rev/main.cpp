/**
/**
    Questo codice introduce la componente di luce diffusiva per illuminare l'oggetto in scena.

    Per determinare la componente di luce diffusiva che viene diffusa da un punto, è necessario avere
    la normale al punto stesso. Il modello 3D è esteso con l'informazione delle normali ai vertici
    delle facce dei triangoli: ad ogni vertice è associata una normale. 

    Il vertex shader è stato modificato per accettare in input le normali dei verticie forwardarle al fragment 
    shader dove verranno usate.

    Modelliamo la luce diffusiva con 3 informazioni: il colore della luce, l'intensità
    della luce e la sua DIREZIONE. In pratica stiamo modellando la luce solare dando,
    invece che la posizione della luce, la dirzione dei raggi. Questo si fa quando la
    sorgente di luce è molto lontana. 

    In questo codice, la direzione della luce è impostata a (0,0,-1), cioè alle spalle
    della camera nella sua posizione iniziale.

    In generale il colore della luce diffusiva può essere identica a quella ambientale (e speculare) ma 
    possiamo variarlo se vogliamo (dipende dal materiale). In questo codice è stato scelto un colore rossastro.

    La componente di luce diffusiva è definita come:

    I_diff = pixel_color * (diffuse_color * diffuse_intensity) * cos(theta)

    Dove il cos(theta) è il coseno dell'angolo compreso tra il versore normale N al punto sulla superficie e 
    il versore corrispondente alla direzione (invertita) di luce diffusiva (guardate a slide).
    Il cos(theta) serve a modulare la luce incidente. diffuse_intensity è un ulteriore fattore che serve 
    a modulare il contributo della luce (di norma vale 1.0). 
	
		Nelle slides, la luce diffusiva è modellata come:
		I_diff = k_diff * I_diff * cos(theta)

    La luce diffusiva è stata rappresentata con una struttura simile a quella ambientale: DiffusiveLightStruct.
    Le informazioni contenute in questa struct sono passate agli shader. 

    Potete variare l'intesità di luce diffusiva con i tasti '3' (decrementa) e '4' (incrementa).

    La luce diffusiva, di default, è orientata verticalmente verso la scena.

    Per avere delle normali coerenti con le superfici, il cubo è descritto dando
    esplicitamente gli attributi dei vertici di ogni triangolo.

    E' stata introdotta anche una classe DiffusiveLight (in light.h, light.cpp)
    per modellare le proprietà della luce.

    La classe MyShaderClass è stata estesa per gestire la luce diffusiva.
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
    glm::vec3 normal;   ///< Normale

    Vertex(
        float x, float y, float z, 
        float r, float g, float b, 
        float xn, float yn, float zn) {
        
        position = glm::vec3(x,y,z);
        color = glm::vec3(r,g,b);
        normal = glm::vec3(xn,yn,zn);
    }

    Vertex(const glm::vec3 &xyz, const glm::vec3 &rgb, const glm::vec3 &norm) 
    : position(xyz), color(rgb), normal(norm) {}
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
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1,0,0), glm::vec3( 0.0f,  0.0f, -1.0f)),
    Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1,0,0), glm::vec3( 0.0f,  0.0f, -1.0f)), 
    Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1,0,0), glm::vec3( 0.0f,  0.0f, -1.0f)), 
    Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1,0,0), glm::vec3( 0.0f,  0.0f, -1.0f)), 
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1,0,0), glm::vec3( 0.0f,  0.0f, -1.0f)), 
    Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(1,0,0), glm::vec3( 0.0f,  0.0f, -1.0f)), 

    Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0,1,0), glm::vec3( 0.0f,  0.0f, 1.0f)),
    Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0,1,0), glm::vec3( 0.0f,  0.0f, 1.0f)),
    Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0,1,0), glm::vec3( 0.0f,  0.0f, 1.0f)),
    Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0,1,0), glm::vec3( 0.0f,  0.0f, 1.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0,1,0), glm::vec3( 0.0f,  0.0f, 1.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0,1,0), glm::vec3( 0.0f,  0.0f, 1.0f)),

    Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0,0,1), glm::vec3(-1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0,0,1), glm::vec3(-1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0,0,1), glm::vec3(-1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0,0,1), glm::vec3(-1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0,0,1), glm::vec3(-1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0,0,1), glm::vec3(-1.0f,  0.0f,  0.0f)),

    Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1,1,0), glm::vec3( 1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1,1,0), glm::vec3( 1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1,1,0), glm::vec3( 1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1,1,0), glm::vec3( 1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1,1,0), glm::vec3( 1.0f,  0.0f,  0.0f)),
    Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1,1,0), glm::vec3( 1.0f,  0.0f,  0.0f)),

    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1,0,1), glm::vec3( 0.0f, -1.0f,  0.0f)),
    Vertex(glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1,0,1), glm::vec3( 0.0f, -1.0f,  0.0f)),
    Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1,0,1), glm::vec3( 0.0f, -1.0f,  0.0f)),
    Vertex(glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1,0,1), glm::vec3( 0.0f, -1.0f,  0.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(1,0,1), glm::vec3( 0.0f, -1.0f,  0.0f)),
    Vertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1,0,1), glm::vec3( 0.0f, -1.0f,  0.0f)),

    Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0,1,1), glm::vec3( 0.0f,  1.0f,  0.0f)),
    Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0,1,1), glm::vec3( 0.0f,  1.0f,  0.0f)),
    Vertex(glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(0,1,1), glm::vec3( 0.0f,  1.0f,  0.0f)),
    Vertex(glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(0,1,1), glm::vec3( 0.0f,  1.0f,  0.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0,1,1), glm::vec3( 0.0f,  1.0f,  0.0f)),
    Vertex(glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0,1,1), glm::vec3( 0.0f,  1.0f,  0.0f))
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

    global.ambient_light = AmbientLight(glm::vec3(1,1,1),0.2);
    global.diffusive_light = DiffusiveLight(glm::vec3(1,1,1),glm::vec3(0,0,-1),1);
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