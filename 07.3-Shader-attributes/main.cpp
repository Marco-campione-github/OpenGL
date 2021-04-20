/**
    Questo codice mostra come usare gli attributi dei vertici.
    In questo esempio il triangolo verrà colorato in base ai colori
    attribuiti ai suoi vertici.

    Gli attributi sono memorizzati in un array di struct Vertex.
*/

#include <iostream>
#include "GL/glew.h" // prima di freeglut
#include "GL/freeglut.h"
#include "glm/glm.hpp"

#include "utilities.h"

GLuint VAO; // Vertex Array Object

GLuint Program;


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
void MyKeyboard(unsigned char key, int x, int y);
void MyClose(void);


void init(int argc, char*argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutInitWindowSize(1024, 768);
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
}

void load_shader_program() {
    
    Shaders s;

    s.push_back(LoadShader(GL_VERTEX_SHADER,"07.vert"));
    s.push_back(LoadShader(GL_FRAGMENT_SHADER,"07.frag"));

    Program = CreateProgram(s); 
}

void create_scene() {

    // In questo codice, le informazioni della scena sono le coordinate spaziali dei vertici e
    // il loro colore. Abbiamo quindi due attributi associati per ogni vertice. 
    // Usiamo un array di strutture dati Vertex. Ogni Vertex contiene le coordinate spaziali del 
    // vertice e il suo colore. 
    Vertex Vertices[3]={
        Vertex(glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3( 1.0f,  0.0f, 0.0f)),
        Vertex(glm::vec3( 1.0f, -1.0f, 0.0f), glm::vec3( 0.0f,  1.0f, 0.0f)),
        Vertex(glm::vec3( 0.0f,  1.0f, 0.0f), glm::vec3( 0.0f,  0.0f, 1.0f))
    };
   
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    // L'array dei vertici ora contiene delle struct Vertex. Bisogna accedere ai campi dei 
    // singoli attributi saltando da struct a struct. 
    //
    // Il primo attributo (0) sono le coordinate spaziali: sono 3 float da non normalizzare e due
    // coordinate spaziali sono separate tra loro da sizeof(Vertex) bytes. 
    // Rispetto l'inizio dell'array, le prime coordinate spaziali si trovano ad un offset di n bytes.
    // In questo caso n lo calcoliamo usando la macro c++ offsetof per ottenere l'offset del campo 
    // position nella struct Vertex. L'offset va passato come puntatore.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
        reinterpret_cast<GLvoid*>(offsetof(struct Vertex, position)));
    glEnableVertexAttribArray(0);

    // Il secondo attributo (1) è la tripletta RGB del colore del vertice: sono 3 float da non 
    // normalizzare e due triplette RGB sono separate tra loro da sizeof(Vertex) bytes. 
    // Rispetto l'inizio dell'array, la prima tripletta RGB si trova ad un offset di n bytes.
    // In questo caso n lo calcoliamo usando la macro c++ offsetof per ottenere l'offset del campo 
    // color nella struct Vertex. L'offset va passato come puntatore.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
         reinterpret_cast<GLvoid*>(offsetof(struct Vertex, color)));
    glEnableVertexAttribArray(1);
}

void MyRenderScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(Program);

    GLuint transformLocation = glGetUniformLocation(Program,"Model2World");
 

    // Matrice di trasformazione identità. 
    // In questo codice non ci interessano le trasformazioni.
    glm::mat4 M(1.0f);


    // Passiamo la matrice M al Vertex Shader.
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, &M[0][0]);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    glutSwapBuffers();

    glutPostRedisplay();
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