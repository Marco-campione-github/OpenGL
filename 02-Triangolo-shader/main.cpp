/**
    Codice che mostra come usare gli shader per disegnare un 
    triangolo.
*/

#include <iostream>
#include "GL/glew.h" // prima di freeglut
#include "GL/freeglut.h"
#include "glm/glm.hpp"

#include "utilities.h" // LoadShader, CreateProgram

GLuint VAO; // Vertex Array Object

GLuint Program; // Programma degli shader

/**
  Prototipi della nostre funzioni di callback. 
  Sono definite più avanti nel codice.
*/
void MyRenderScene(void);
void MyKeyboard(unsigned char key, int x, int y);
void MyClose(void); 


// Inizializzazione delle librerie e del contesto OpenGL
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

/**
    Questa funzione serve per caricare, compilare e linkare i nostri shader.
    Gli shader sono memorizzati infile di testo.
    Usiamo le funzioni di supporto definite in utilities.cpp
*/
void load_shader_program() {
    
    // Istanziamo il vettore degli shader che verrà riempito
    Shaders s;

    // Carichiamo e riempiamo il vettore con gli shader compilati
    s.push_back(LoadShader(GL_VERTEX_SHADER,  "02.vert"));
    s.push_back(LoadShader(GL_FRAGMENT_SHADER,"02.frag"));

    // Creariamo il programma degli shader 
    Program = CreateProgram(s); 

    for (int i=0; i< s.size(); ++i) glDeleteShader(s[i]);
}

// Creazione della scena 
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

    // Diciamo a OpenGL di usare l'attributo di indice 0 con gli shader
    // Se guardate il codice del Vertex Shader "02.vert", noterete che c'e'
    // una variabile di input chiamata position che è definita come:
    //
    // layout (location = 0) in vec3 position;
    //
    // L'indice della location è lo stesso dell'attributo attivato.
    // In pratica la variabile position sarà riempita con le informazioni 
    // provenienti dall'attributo 0 definito conle due istruzioni seguenti.
    // L'effetti finale è che ogni volta che lo shader sarà invocato riceverà 
    // le coordinate di un vertice.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

}

// Funzione globale che si occupa di renderizzare la scena.
void MyRenderScene() {
    glClear(GL_COLOR_BUFFER_BIT);

    // In questo codice usiamo i nostri shader invece che quelli di default.
    // Per questo dobbiamo attivarli dicendo ad OpenGL di usare il nostro
    // programma di shader 
    glUseProgram(Program);

    glBindVertexArray(VAO);

    // Disegnamo il triangolo. 
    // ATTENZIONE. Adesso il vertex shader sta modificando le coordinate dei vertici.
    // Guardate il file 02.vert per vedere come.
    // Il fragment shader invece colora tutti i punti del triangolo di rosso.
    // Potete modificare gli shader e, senza ricompilarlo, rieseguire il programma
    // per vedere gli effetti delle modifiche. Gli shader sono caricati
    // a runtime.
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Disabilitiamo il programma degli shader
    glUseProgram(0);

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

// Funzione globale che si occupa di gestire la chiusura della finestra.
void MyClose(void) {
    std::cout << "Tearing down the system..." << std::endl;

    glDeleteVertexArrays(1,&VAO);
    glDeleteProgram(Program);
}

int main(int argc, char* argv[])
{

    // Inizializzazione
    init(argc,argv);

    // Carica gli shader
    load_shader_program();

    // Creazione della scena in memoria
    create_scene();

    // Loop infinito di FREEGLUT
    glutMainLoop();
    
    return 0;
}