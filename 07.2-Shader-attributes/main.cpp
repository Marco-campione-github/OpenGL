/**
    Questo codice mostra come usare gli attributi dei vertici.
    In questo esempio il triangolo verrà colorato in base ai colori
    attribuiti ai suoi vertici.

    Gli attributi sono memorizzati in un array di float come sequenze separate.
*/

#include <iostream>
#include "GL/glew.h" // prima di freeglut
#include "GL/freeglut.h"
#include "glm/glm.hpp"

#include "utilities.h"

GLuint VAO; // Vertex Array Object

GLuint Program;


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
    // Usiamo un solo array di vec3 e le informazioni sono separate:
    // coordinate1, coordinate2, coordinate3, colore1, colore2, colore3

    glm::vec3 Vertices[6]={
        glm::vec3(-1.0f, -1.0f, 0.0f), // coordinate spaziali primo vertice
        glm::vec3( 1.0f, -1.0f, 0.0f), // coordinate spaziali secondo vertice
        glm::vec3( 0.0f,  1.0f, 0.0f), // coordinate spaziali terzo vertice

        glm::vec3( 1.0f,  0.0f, 0.0f), // colore RGB primo vertice
        glm::vec3( 0.0f,  1.0f, 0.0f), // colore RGB secondo vertice
        glm::vec3( 0.0f,  0.0f, 1.00f) // colore RGB terzo vertice

    };
   
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    // L'array dei vertici ora contiene diversi attributi: le coordinate spaziali e il colore 
    // Sono due attributi che vanno attivati se si vogliono usare.
    // Bisogna dire a OpenGL come recuperare questi attributi. 
    //
    // Il primo attributo (0) sono le coordinate spaziali: sono 3 float da non normalizzare e due
    // coordinate spaziali sono separate tra loro da 2*sizeof(glm::vec3) bytes. 
    // Rispetto l'inizio dell'array, le prime coordinate spaziali si trovano ad un offset di 0 bytes.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    glEnableVertexAttribArray(0);

    // Il secondo attributo (1) è la tripletta RGB del colore del vertice: sono 3 float da non 
    // normalizzare e due triplette RGB sono separate tra loro da 2*sizeof(glm::vec3) bytes. 
    // Rispetto l'inizio dell'array, la prima tripletta RGB si trova ad un offset di 3*sizeof(glm::vec3) 
    // bytes (sono le coordinate dei vertici).L'offset va dato come puntatore (a void) ed è per questo 
    //che è necessario eseguire il cast. 0 è equivalente al puntatore null.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 
        reinterpret_cast<GLvoid*>(3*sizeof(glm::vec3)));
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