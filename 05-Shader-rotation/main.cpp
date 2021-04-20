/**
    Codice che mostra come applicare le matrici di rotazione.
    Premendo il tasto 'x' ruoterà intorno all'asse x.
    Premendo il tasto 'y' ruoterà intorno all'asse y.
    Premendo il tasto 'z' ruoterà intorno all'asse z.    
    Premendo il tasto 'a' ruoterà intorno ai rte assi contemporaneamente.    

    All'avvio il triangolo ruota intorno all'asse z.
*/

#include <iostream>
#include "GL/glew.h" // prima di freeglut
#include "GL/freeglut.h"
#include "glm/glm.hpp"

#include "utilities.h"

GLuint VAO; // Vertex Array Object

GLuint Program;

// Variabile globale che ci permette di sapere intorno a quale asse
// deve ruotare il triangolo. E' settata dalla callback MyKeboard  
char rotation_axis = 'z';

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

    s.push_back(LoadShader(GL_VERTEX_SHADER,"05.vert"));
    s.push_back(LoadShader(GL_FRAGMENT_SHADER,"05.frag"));

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

    // Per comodità definiamo la variabile locale statica
    // Una variabile locale statica è una variabile che mantiene il
    // valore da una chiamata all'altra. Viene inizializzata una sola volta.
    // Theta contiene il valore dell'angolo di rotazione 
    static float theta = 0;
    theta += 0.01;

    // Creiamo le tre matrici di rotazione
    // Ricordate che glm ha le matrici column-major M[x][y]

    // Rotazione intorno all'asse z
    glm::mat4 Rz(1.0f); 
    Rz[0][0] = cosf(theta); Rz[1][0] = -sinf(theta); 
    Rz[0][1] = sinf(theta); Rz[1][1] =  cosf(theta);

    // Rotazione intorno all'asse x
    glm::mat4 Rx(1.0f);
    Rx[1][1] = cosf(theta); Rx[2][1]=-sinf(theta);
    Rx[1][2] = sinf(theta); Rx[2][2]= cosf(theta);

    // Rotazione intorno all'asse y
    glm::mat4 Ry(1.0f);;
    Ry[0][0] =  cosf(theta); Ry[2][1]= sinf(theta);
    Ry[0][2] = -sinf(theta); Ry[2][2]= cosf(theta);

    // Recuperiamo la matrice di rotazione da usare dalla variabile 
    // globale rotation_axis 
    glm::mat4 R(1.0f);

    switch (rotation_axis) {
        case 'x':
            R=Rx;
        break;

        case 'y':
            R=Ry;
        break;

        case 'z':
            R=Rz;
        break;

        case 'a':
            R = Rx * Ry * Rz;
        break;
    }

    GLuint transformLocation = glGetUniformLocation(Program,"Model2World");

    // Passiamo la matrice al Vertex Shader. Potevamo usare subito una delle
    // tre matrici Rx, Ry o Rz.  
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, &R[0][0]);

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

        case 'x':
            rotation_axis = 'x';
        break;

        case 'y':
            rotation_axis = 'y';
        break;

        case 'z':
            rotation_axis = 'z';
        break;

        case 'a':
            rotation_axis = 'a';
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