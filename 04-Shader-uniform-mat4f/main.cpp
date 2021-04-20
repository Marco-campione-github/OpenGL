/**
    Codice che mostra come modificare le coordinate dei vertici con
    una matrice di trasformazione passata agli shader.
*/


#include <iostream>
#include "GL/glew.h" // prima di freeglut
#include "GL/freeglut.h"
#include "glm/glm.hpp"

#include "utilities.h"

GLuint VAO; // Vertex Array Object

GLuint Program;

// Variabile globale usata per modificare la posizione orizzontale 
// del triangolo
float Offset = 0.f;

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

    s.push_back(LoadShader(GL_VERTEX_SHADER,"04.vert"));
    s.push_back(LoadShader(GL_FRAGMENT_SHADER,"04.frag"));

    Program = CreateProgram(s); 
}

void create_scene() {
    glm::vec3 Vertices[3]={
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3( 1.0f, -1.0f, 0.0f),
        glm::vec3( 0.0f,  1.0f, 0.0f)
    };
   
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void MyRenderScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(Program);
 
    // Matrixe 4x4 locale
    // Le matrici della librerie glm sono istanziate come matrici identità.
    glm::mat4 Translation(1.0f);

    // Aggiorniamo il valore della variabile globale Offset
    Offset+=0.002;

    // Inseriamo un valore nella quarta colonna della matrice,
    // prima riga. In pratica stiamo settando una matrice di traslazione
    // che sposta le coordinate x del triangolo.
    // Guardate le slide "Trasformazioni 3D"
    Translation[3][0]=sinf(Offset); // Attenzione le matrici glm sono column major 
                                    // Translation[col][row];

    // Recuperiamo l'id della variabile Model2World del Vertex Shader 
    // La variabile è una matrice 4x4 che verrà usata per
    // trasformare le coordinate dei vertici del triangolo.
    GLuint transformLocation = glGetUniformLocation(Program,"Model2World");

    // Copiamo il valore della matrice 4x4 locale, nella matrice del Vertex Shader.
    // In questo caso dobbiamo passare alla funzione apposita: 
    // l'identificativo della matrice nello shader
    // quante matrici 4x4 copiare
    // se la matrice deve essere trasposta (se per caso è codificata row major. 
    //    glm codifica le matrici column major)
    // l'indirizzo (puntatore) al dato
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, &Translation[0][0]);

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

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