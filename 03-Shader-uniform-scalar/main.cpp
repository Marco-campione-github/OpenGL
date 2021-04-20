/**

    Codice che mostra come si passano dei parametri scalari agli shader.
    La scena è animata con il triangolo che è riscalato con un fattore Scale 
    che varia in modo oscillatorio con il sin(Scale). 
*/

#include <iostream>
#include "GL/glew.h" // prima di freeglut
#include "GL/freeglut.h"
#include "glm/glm.hpp"

#include "utilities.h" // LoadShader, CreateProgram

GLuint VAO; // Vertex Array Object

GLuint Program; // Programma degli shader

float Scale=0;

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

    s.push_back(LoadShader(GL_VERTEX_SHADER,"03.vert"));
    s.push_back(LoadShader(GL_FRAGMENT_SHADER,"03.frag"));

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

    // Aggiorniamo la nostra variabile globale
    // Non teniamo conto del possibile (ma improbabile) overflow
    Scale += 0.005f;

    // Nel Vertex Shader c'e' una variabile di tipo Uniform che 
    // può essere usata per passare allo shader dei parametri.
    // Prima però dobbiamo recuperare l'identificativo della variabile.
    // Ricordate che non possiamo accedere direttamente agli shader dal main. 
    // Per avere queste informazioni, il program degli shader deve essere stato
    // attivato.
    GLuint scaleLocation = glGetUniformLocation(Program,"scale");
     
    // Passiamo il valore della varibile Scale del main (in realtà il seno
    // della variabile Scale) alla variabile corrispondente del Vertex Shader
    // mediante il suo identificativo (scaleLocation).
    // Esistono diverse funzioni che si chiamano glUniformXX con XX
    // un suffisso  che identifica il tipo di dato da passare.
    glUniform1f(scaleLocation, fabs(sinf(Scale)));

    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glutSwapBuffers();

    // In questo codice, abbiamo una animazione e quindi dobbiamo renderizzare la scena
    // in modo continuativo. Un modo per farlo è quello di indicare che la finestra 
    // di visualizzazione deve essere aggiornata.
    // Fino ad ora, la finestra veniva disegnata una sola volta e quando la finestra
    // veniva spostata.
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
