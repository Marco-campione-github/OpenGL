/**
    Questo codice mostra come si può disegnare una piramide a base triangolare.
    Dobbiamo disegnare quattro triangoli ciascuno dei quali è composto da 3 vertici.
    In totale useremo un array di 12 vertici per descrivere la piramide.
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

		// Abilitiamo l'uso depth buffer in GLUT. Dobbiamo farlo
	  // anche in OpenGL
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
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


    // Fino ad ora abbiamo sempre disegnato un triangolo. Dato che adesso abbiamo un
    // composta da diversi triangoli, dobbiamo abilitare delle operazioni di clipping
    // che ci permettono di rimuovere dal disegno le parti dell'oggetto che non sono
    // visibili.  

    // Un triangolo ha due facce (front e back). Solo una delle due è una faccia da 
    // disegnare, l'altra è considerata nascosta e non deve essere disegnata. 
    // Dobbiamo istruire OpenGl su quali facce dei triangoli ci interessa disegnare.
    // Questo comando OpenGL attiva il face culling cioè la rimozione di triangoli che
    // mostrano alla camera la faccia nascosta.   
    glEnable(GL_CULL_FACE);

    // Dato che un triangolo ha due facce (front e back), dobbiamo dire a OpenGL
    // quale delle due deve essere considerata nascosta. Nel nostro caso, le
    // facce non visibili sono le back. 
    glCullFace(GL_BACK);

    // Dobbiamo però anche dire ad OpenGL come si capisce che una faccia è front oppure back.
    // Nel nostro caso, le facce front sono quelle che, se osservate dalla camera, sono descritte 
    // dai vertici dati in ordine antiorario. 
    glFrontFace(GL_CCW);

    // Abilitiamo anche il depth dest per rimuovere i pixel nascosti
    glEnable(GL_DEPTH_TEST);

    // OpenGL permette di settare liberamente questi parametri.
}

void load_shader_program() {
    
    Shaders s;

    s.push_back(LoadShader(GL_VERTEX_SHADER,"08.vert"));
    s.push_back(LoadShader(GL_FRAGMENT_SHADER,"08.frag"));

    Program = CreateProgram(s); 
}

void create_scene() {

    // La piramide a base triangolare è composta da quattro triangoli.
    // Ciascun triangolo è composto da tre vertici.
    // In totale abbiamo 12 vertici. Assegnamo ai vertici di ciascun triangolo
    // lo stesso colore.
    //
    // Notate che in questo modo ripetiamo tre volte le coordinate spaziali degli
    // stessi vertici (ogni vertice è condiviso da tre triangoli).
    //
    // La piramide è posta in scena con uno spigolo in mezzo alla finestra. 
    //
    // Guardando la piramide frontalmente, essa è composta da una faccia a sinistra 
    // rossa, una faccia a destra verde, una faccia nascosta blu e una base nascosta
    // gialla. I vertici delle facce visibili sono dati in senso antiorario. I vertici
    // delle facce non visibili sono dati in senso orario. 
    //
    Vertex Vertices[12] = {
        Vertex(glm::vec3(-1.0f, -1.0f,-1.0f), glm::vec3( 1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3( 1.0f, 0.0f, 0.0f)),
        Vertex(glm::vec3( 0.0f,  1.0f,-1.0f), glm::vec3( 1.0f, 0.0f, 0.0f)),

        Vertex(glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3( 0.0f, 1.0f, 0.0f)),
        Vertex(glm::vec3( 1.0f, -1.0f,-1.0f), glm::vec3( 0.0f, 1.0f, 0.0f)),
        Vertex(glm::vec3( 0.0f,  1.0f,-1.0f), glm::vec3( 0.0f, 1.0f, 0.0f)),

        Vertex(glm::vec3(-1.0f, -1.0f,-1.0f), glm::vec3( 0.0f, 0.0f, 1.0f)),
        Vertex(glm::vec3( 0.0f,  1.0f,-1.0f), glm::vec3( 0.0f, 0.0f, 1.0f)),
        Vertex(glm::vec3( 1.0f, -1.0f,-1.0f), glm::vec3( 0.0f, 0.0f, 1.0f)),

        Vertex(glm::vec3( 0.0f, -1.0f, 0.0f), glm::vec3( 1.0f, 1.0f, 0.0f)),
        Vertex(glm::vec3(-1.0f, -1.0f,-1.0f), glm::vec3( 1.0f, 1.0f, 0.0f)),
        Vertex(glm::vec3( 1.0f, -1.0f,-1.0f), glm::vec3( 1.0f, 1.0f, 0.0f)),
    };
   
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
   
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

}

void MyRenderScene() {

		// Dobbiamo azzerare anche il depth buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(Program);

    GLuint transformLocation = glGetUniformLocation(Program,"Model2World");
 
    static float theta = 0;
    theta += 0.001;

    glm::mat4 Rz(1.0f); 
    Rz[0][0] = cosf(theta); Rz[1][0] = -sinf(theta); 
    Rz[0][1] = sinf(theta); Rz[1][1] =  cosf(theta);

    glm::mat4 Rx(1.0f);
    Rx[1][1] = cosf(theta); Rx[2][1]= -sinf(theta);
    Rx[1][2] = sinf(theta); Rx[2][2]=  cosf(theta);

    glm::mat4 Ry(1.0f);
    Ry[0][0] =  cosf(0.8*theta); Ry[2][0]=  sinf(0.8*theta);
    Ry[0][2] = -sinf(0.8*theta); Ry[2][2]=  cosf(0.8*theta);

    glm::mat4 S(1.0f);
    S[0][0]=0.5;
    S[1][1]=0.5;
    S[2][2]=0.5;

    // Per comodità scaliamo la piramide della metà.
    // Per vedere tutte le facce della piramide dobbiamo ruotarla.  
    glm::mat4 M=Ry*Rx*S;

    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, &M[0][0]);

    glBindVertexArray(VAO);
 
    glDrawArrays(GL_TRIANGLES, 0, 12);

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