#include "dot.h"

//Vertex Array Object che contiene i vertici da visualizzare
GLuint VAO;

void init(int argc, char* argv[]) {
    glutInit(&argc, argv);        //inizializza glut (parametri opzionali -sync -gldebug)

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); //double buffer e color buffer
    glutInitWindowSize(800, 600);                 //dimensione della window
    glutInitWindowPosition(100, 100);             //posizione della window
    glutCreateWindow("Dot");                      //nome della window

    //inizialize glew
    GLenum res = glewInit();
    if (res != GLEW_OK) {
        std::cerr<<"Error : "<<glewGetErrorString(res)<<std::endl;
      exit(1);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);         // scegliamo il colore di sfondo (R,G,B,alpha)

    glutDisplayFunc(dot);   
}

void dot() {
    // Diciamo a OpenGL che quando deve inziare un ciclo di disegno deve cancellare
    // il contenuto del framebuffer.
    glClear(GL_COLOR_BUFFER_BIT);
  
	// Abilitiamo il Vertex Array Object che abbiamo impostato in precedenza
	glBindVertexArray(VAO);

    // disegno
    glDrawArrays(GL_POINTS, 0, 1);

    // Quando abbiamo finito di disegnare, disabilitiamo il VAO corrente
    glBindVertexArray(0);

    // Scambia il contenuto del framebuffer interno con il contenuto della finestra
    // di visualzzazione. Ricordate che abbiamo attivato il doublebuffering.
    glutSwapBuffers();
}

void MyScene(){
     glm::vec3 Vertices[1]={
        glm::vec3(0.0f, 0.0f, 0.0f)
    };
    
   	// Creiamo un Vertex Array Object per contenere le informazioni necessarie
   	// a disegnare il nostro oggetto 3D.
    glGenVertexArrays(1,&VAO);

    //Istanziamo una variabile Vertex Buffer Object (VBO) che conterrà
    //l'id dell'oggetto creato nella memoria della scheda grafica.
    //Un VBO è una specie di struttura dati che conterrà tutte le informazioni
    //degli oggetti da renderizzare della scena.
    GLuint VBO;

    // Creiamo un Vertex Buffer Object
    glGenBuffers(1, &VBO);

    // Settiamo/attiviamo nello stato interno del VAO il nostro VBO come ARRAY_BUFFER.
    // ARRAY_BUFFER è un target che viene usato per impostare le informazioni di disegno
    // come i vertici (e se presenti i loro attributi) degli oggetti in scena.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    // La funzione seguente, dice che l'attributo 0 associato ai dati (vertici) deve essere
    // usato dagli shader.
    glEnableVertexAttribArray(0);

    // Disabilitiamo il VAO corrente
    glBindVertexArray(0);
}

void startDot(int argc, char* argv[]) {
    // Inizializzazione
    init(argc,argv);

    // Creazione della scena in memoria
    MyScene();

    // Loop infinito di FREEGLUT
    glutMainLoop();
}