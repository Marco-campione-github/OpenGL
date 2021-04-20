/**
    Codice che mostra come si disegna un triangolo in OpenGL.
    Sono richiamate diverse funzioni introdotte nelle slide 
    "Introduzione a OpenGL" pt1 e pt2.
*/

#include <iostream>
#include "GL/glew.h" // prima di freeglut
#include "GL/freeglut.h"
#include "glm/glm.hpp"

// Istanziamo una variabile Vertex Array Object che conterrà le informazioni
// su un oggetto di scena da renderizzare.
GLuint VAO;

/**
  Prototipi della nostre funzioni di callback. 
  Sono definite più avanti nel codice.
*/
void MyRenderScene(void);
void MyKeyboard(unsigned char key, int x, int y);
void MyClose(void); 

/**
    Funzione di inizializzazione delle varie librerie e del contesto OpenGL.
    Inizializziamo FREEGLUT e impostiamo la finestra di visualizzazione con le sue 
    proprietà: dimensioni, posizione e titolo. GLUT è inizializzato in modo
    tale da usare il double buffering e un FrameBuffer con pixel codificati 
    in RGBA.
    Impostiamo anche il colore di sfondo della finestra (ClearColor).
    Impostiamo la funzione di rendering che viene chiamata automaticamente dal 
    ciclo di esecuzione di FREEGLUT lanciato nel main.
*/
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

    // Impostiamo colore di sfondo
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Impostiamo callback di rendering
    glutDisplayFunc(MyRenderScene);

    // La funzione per gestire l'input da tastiera
    glutKeyboardFunc(MyKeyboard);

    // La funzione per gestire la chiusura della finestra
    glutCloseFunc(MyClose);}

/**
    Funzione che crea la "scena".
    Questa funzione riempie il VBO con i vertici degli oggetti di scena.
    In questo esempio di codice, definiamo un array di vertici per descrivere
    un semplice triangolo.
    I vertici sono definiti come dei vettori a tre componenti della libreria glm.
    Gli oggetti della scena 3D sono descritti in coordinate cartesiane 3D. 
    Le coordinate omogenee sono usate solo durante le fasi di applicazione di
    trasformazioni geometriche.
    Le coordinate dei vertici sono date nel range [-1,1]. Questo perchè in questo
    momento vogliamo disegnare direttamente nello spazio di coordinate che vengono
    visualizzate a schermo. Questo spazio di coordinate è definito nel range [-1,1]
    in tutte le direzioni. In pratica lo spazio che stiamo considerando è quello
    relativo alla regione di clipping 3D (il normalized view volume) che ha tali range.

    Negli esempi successivi, gli oggetti in scena saranno definiti nel loro spazio
    di coordinate che andranno poi trasformate nel normalized view volume attraverso
    le varie trasformazioni geometriche. Per ora usiamo direttamente le coordinate
    del normalized view volume.

    Inoltre, considerate che la trasformazione viewport è automaticamente applicata.
    Questo comporta che le corodinate (x,y)=(-1,-1) sono mappate con l'angolo inferiore 
    sinistro della finestra di visualizzazione. le coordinate (x,y)=(1,1) sono mappate 
    con l'angolo superiore destro della finestra di visualizzazione. le coordinate
    (0,0) sonomappate al  centro della finestra di visualizzazione. 

    Notate anche l'ordine con cui sono dati i vertici del triangolo. E' un ordine in
    senso ANTIORARIO. 

    Il triangolo ha vertici con coordinata z pari a 0. In questo esempio, la coordinata
    z non influisce sulla visualizzazione.

    Il nostro triangolo sarà disegnato con la base coincidente al lato inferiore della 
    finestra di visualizzazione e vertice in alto in mezzo al lato superiore della finestra.
    
    Se provate a ridimensionare la finestra di visualizzazione, vedrete che il triangolo
    viene comunque fittato nella nuova finestra. 
*/
void create_scene() {
    // Descriviamo il triangolo come sequenza di vertici 3D
    // Notate come i vertici siano dati in senso antiorario:
    // vertice in basso a sinistra, in basso a destra, in alto al centro
    // Guardate le slide "Modellazione 3D" 
    glm::vec3 Vertices[3]={
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3( 1.0f, -1.0f, 0.0f),
        glm::vec3( 0.0f,  1.0f, 0.0f)
    };
   
   	// Creiamo un Vertex Array Object per contenere le informazioni necessarie
   	// a disegnare il nostro oggetto 3D.
    glGenVertexArrays(1,&VAO);

    // Rendiamo il VAO attivo
		glBindVertexArray(VAO);

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

    // Copiamo nel target ARRAY_BUFFER (e quindi nel nostro VBO) i dati dei vertici. 
    // Con questa chiamata stiamo trasferendo i dati dalla memoria del PC alla memoria
    // della scheda grafica. NON stiamo ancora disegnando nulla stiamo solo preparando
    // i dati. 
    // Dobbiamo dire in che target stiamo trasferendo i dati (ARRAY_BUFFER). 
    // Quale è la dimensione totale dei dati da trasferire e quale è l'indirizzo
    // a cui si trovano i dati (puntatore).
    // GL_STATIC_DRAW informa OpenGL che i dati contenuti nel buffer non cambiano
    // nel tempo. E' un flag che permette ad OpenGL di ottimizzare l'accesso ai dati.
    // Allo stato attuale, i dati trasferiti sono solo coordinate di vertici. Vedremo
    // in seguito che è possibile avere più informazioni insieme. 
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    // Dobbiamo dire a OpenGL come sono fatti i dati presenti dell'ARRAY_BUFFER 
    // Per farlo, OpenGL usa un meccanismo di indicizzazione dei dati (o attributi).
    // In pratica dobbiamo dire a OpenGL quale/quali attributi dei vertici sono da usare
    // (sono abilitati). E poi dire, per ogni attributo, come sono fatti i dati.
    // Nel nostro caso abbiamo un solo attributo (posizione 3D dei vertici). Quindi
    // il suo identificativo a valore 0 (gli attributi sono indicizzati a base 0). 
    // Questa informazione è necessaria per usare gli shader dati che gli attributi
    // sono passati usando il loro indice (vedremo più avanti che significa).
    //  
    // Al momento non stiamo usando esplicitamente degli shader. Ma dato che la 
    // pipeline di rendering programmabile è obbligatoria (e quindi gli shader), OpenGL
    // ha di default attivi un vertex shader e un fragment shader di base.
    // Il vertex shader di base è semplicemente un pass-through. Prende un vertice e 
    // lo ritorna in output senza fare nulla. Il fragment shader di base assegna a tutti 
    // i punti della scena il colore bianco.

    // Questa funzione dice ad OpenGL come sono fatti i dati associati all'attributo 0.
    // Quando abbiamo trasferito i nostri vertici sulla scheda grafica, i dati sono 
    // stati trasferiti come  una sequenza di byte. Abbiamo perso l'informazione 
    // sulla struttura interna dei dati stessi. Con la prossima funzione diciamo ad 
    // OpenGL come interpretare questo array di bytes. 
    //
    // In pratica diciamo che l'attributo 0 (nel nostro caso le coordinate dei vertici, 
    // l'unico attributo che abbiamo), è costuito da 
    // 3 dati (coordinate x,y,z)
    // di tipo float (GL_FLOAT)
    // che non vogliamo che siano normalizzati (lo sono già)
    // che due dati sono separati da sizeof(glm::vec3) bytes
    // a che offset (in byte) si trova il primo attributo nell'array di bytes
    //
    // Sarà tutto un poco più chiaro quando avremo più attributi insieme nello stesso
    // array. 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

    // La funzione seguente, dice che l'attributo 0 associato ai dati (vertici) deve essere
    // usato dagli shader.  
    glEnableVertexAttribArray(0);

    // Disabilitiamo il VAO corrente
    glBindVertexArray(0);   
}


/**
    Nostra funzione di rendering che verrà chiamata dal loop di FREEGLUT.
    In questa funzione dobbiamo mettere tutte le istruzioni che servono per
    dire a OpenGL quali sono i dati da renderizzare nella scena e come devono
    essere disegnati. 
*/
void MyRenderScene() {
    // Diciamo a OpenGL che quando deve inziare un ciclo di disegno deve cancellare
    // il contenuto del framebuffer.
    glClear(GL_COLOR_BUFFER_BIT);
    
		// Abilitiamo il Vertex Array Object che abbiamo impostato in precedenza  
		glBindVertexArray(VAO);

    // Diciamo a OpenGL che, usando il VAO, deve disegnare dei triangoli. 
    // Le informazioni di disegno sono recuperate dal VAO,
    // come descritto in precedenza.
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Quando abbiamo finito di disegnare, disabilitiamo il VAO corrente
    glBindVertexArray(0);

    // Scambia il contenuto del framebuffer interno con il contenuto della finestra
    // di visualzzazione. Ricordate che abbiamo attivato il doublebuffering.
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
}

int main(int argc, char* argv[])
{
    // Inizializzazione
    init(argc,argv);

    // Creazione della scena in memoria
    create_scene();

    // Loop infinito di FREEGLUT
    glutMainLoop();
    
    return 0;
}