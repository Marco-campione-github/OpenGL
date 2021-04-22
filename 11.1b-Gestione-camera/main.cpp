/**
    Questo codice mostra come muovere la camera in avanti/indietro e destra/sinistra
    con i tasti freccia.

    La gestione dei tasti freccia è fatta impostando la callback glutSpecialFunc che
    forwarda le chiamate al metodo onSpecialKeyboard nella classe Camera dove si 
    modifica la trasformazione di camera.
*/


#include <iostream>
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

    int WINDOW_WIDTH = 1024; // Larghezza della finestra 
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

void init(int argc, char*argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
    glutInitWindowSize(global.WINDOW_WIDTH, global.WINDOW_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Informatica Grafica");

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
    modelT.scale(0.5); 
    modelT.translate(0,0,0); 

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
    }
}

void MySpecialKeyboard(int Key, int x, int y) {
    global.camera.onSpecialKeyboard(Key);
    glutPostRedisplay();
}

// Funzione globale che si occupa di gestire la chiusura della finestra.
void MyClose(void) {
    std::cout << "Tearing down the system..." << std::endl;
}

int main(int argc, char* argv[])
{
    init(argc,argv);

    load_shader_program();

    create_scene();

    glutMainLoop();
    
    return 0;
}