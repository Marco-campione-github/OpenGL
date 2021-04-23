/**
    Questo codice mostra come cambiare il lookat della camera con il mouse.
    Spostando il mouse a destra/sinistra si ruota la camera rispetto all'asse verticale.
    Spostando il mouse in su/giu si alza/abbassa lo sguardo.

    In questo codice la finestra è mostrata a schermo interno alla risoluzione di 1920x1080.
    Cambiate i valori di WINDOW_WIDTH e WINDOW_HEIGHT alla risoluzione del vostro schermo.

    Premendo il tasto spazio la camera si resetta alla posizione iniziale.
*/


#include <iostream>
#include <sstream>
#include "GL/glew.h" // prima di freeglut
#include "GL/freeglut.h"
#include "glm/glm.hpp"

#include "utilities.h"
#include "transform.h"
#include "camera.h"

/**
    Struttura di comodo dove sono memorizzate tutte le variabili globali
*/
struct global_struct {

    int WINDOW_WIDTH  = 1366; // Larghezza della finestra 
    int WINDOW_HEIGHT = 768; // Altezza della finestra

    GLuint VAO; // Vertex Array Object

    GLuint program; // id del programma degli shader

    Camera camera;

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

    // Imposta la modalità a schemo intero e nasconde il cursore
    std::stringstream game_mode;
    game_mode << global.WINDOW_WIDTH << "x" << global.WINDOW_HEIGHT << ":32";
    glutGameModeString(game_mode.str().c_str());
    glutEnterGameMode();
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

void load_shader_program() {
    
    Shaders s;

    s.push_back(LoadShader(GL_VERTEX_SHADER,"11.vert"));
    s.push_back(LoadShader(GL_FRAGMENT_SHADER,"11.frag"));

    global.program = CreateProgram(s); 
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
        glm::vec3(0,0,4),
        glm::vec3(0,0,0),
        glm::vec3(0,1,0)
    );

    global.camera.set_perspective(
        45.0f,
        global.WINDOW_WIDTH,
        global.WINDOW_HEIGHT,
        0.1,
        100);
}

void MyRenderScene() {
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(global.program);

    GLuint transformLocation = glGetUniformLocation(global.program,"Model2World");
 
    LocalTransform modelT;

    // Creiamo la trasformazione completa moltiplicando la matrice di proiezione per la
    // matrice composita
    glm::mat4 M = global.camera.CP() * modelT.T();

    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, &M[0][0]);

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
        break;
    
        case ' ': // Reimpostiamo la camera
            global.camera.set_camera(
                glm::vec3(0,0,4),
                glm::vec3(0,0,0),
                glm::vec3(0,1,0)
            );
            glutWarpPointer(global.WINDOW_WIDTH/2, global.WINDOW_HEIGHT/2);
        break;
    }
}

void MySpecialKeyboard(int Key, int x, int y) {
    global.camera.onSpecialKeyboard(Key);
    glutPostRedisplay();
}


void MyMouse(int x, int y) {
    global.camera.onMouse(x,y);

    // Risposto il mouse al centro della finestra
    glutWarpPointer(global.WINDOW_WIDTH/2, global.WINDOW_HEIGHT/2);
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

    load_shader_program();

    create_scene();

    glutMainLoop();
    
    return 0;
}