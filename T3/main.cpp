#include <GL/glut.h>
#include "Tabuleiro.h"
#include "Triangulo.h"
#include "Vertice.h"
#include "Plano.h"
#include "Esfera.h"
#include "Bloco.h"
#include "Desenha.h"
#include "Pad.h"
#include "GameController.h"
#include "Aux.h"
#include "glcWavefrontObject.h"
#include <math.h>
#include <chrono>
#include <random>

#define NUM_OBJECTS 2

char objectFiles[NUM_OBJECTS][100] =
{
    "/home/caveira/Documents/GitProjects/T2_CG/T2/data/obj/SuperMarioNormal.obj",
    "/home/caveira/Documents/GitProjects/T2_CG/T2/data/obj/SuperMarioNormal.obj"
};

glcWavefrontObject *objectManager = NULL;

/// VARIAVEIS GLOBAIS
int width = 800, height = 600; // Viewport

int last_x, last_y; // MOUSE
double rotation;
double desiredFPS = 60; // IDLE

bool ortho; // coordenadas  ortho
int projecao; // 0 ortogonal, 1 perspectiva
bool cameraLivre = false;

double zdist; // distancia no eixo Z utilizada na função gluLookAt
double rotationX, rotationY, rotationZ; // posições iniciais em X e Y. Valores são alterados ao rotacionar o tabuleiro

/// ILUMINACAO
GLfloat ambient[] = { 0.3, 0.3, 0.3, 1.0 };
GLfloat emissao[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat brilho[] = { 128.0 };

// Material da base (amarelo)
GLfloat base_difusa[] = { 1.0, 1.0, 0.0, 1.0 };
GLfloat base_especular[] = { 1.0, 1.0, 0.0, 1.0 };

// Material dos objetos a serem iluminados
GLfloat object_difusa[] = { 1.0, 0.65, 0.2, 1.0 };
GLfloat object_especular[] = { 1.0, 0.65, 0.2, 1.0 };

// Material da lanterna
GLfloat lanterna1[] = { 1.0, 0.0, 0.0, 1.0 }; // corpo
GLfloat lanterna2[] = { 1.0, 1.0, 1.0, 1.0 }; // ponta

// Defini cor da luz ambiente
GLfloat cor_luz_amb[] = { 0.1, 0.1, 0.1, 1.0};

// Especificação da luz do spotlight
GLfloat cor_luz_ativa[] = { 1.0, 0.0, 0.0, 1.0};
GLfloat posicao_luz0[] = { 10.0, -15.0, 10.0, 1.0};
GLfloat cor_luz0[] = { 1.0, 1.0, 1.0, 1.0};
GLfloat spotDir[3] = { 0.0, 1.0, 0.0 };
GLfloat cutoffAngle = 45.0f;

// Especificação da luz que iluminará a lanterna
GLfloat cor_luz1[] = { 0.5, 0.5, 0.5, 1.0};

///OBJETOS
// GameController
GameController gameController;
// Desenha
Desenha desenha;
// Vertices
Vertice posInicialTab(0.0, 0.0, 0.0);
Vertice pontoPad(0.5, 0.1, 0.01);
Vertice pontoEsfera((0.5+0.9)/2, 0.3, 0.15);
// Tabuleiro
Tabuleiro tab(&posInicialTab, 1.5, 2.5, 0.2);
// Pad
Pad pad(&pontoPad, 0.4, 0.1, 0.1);
// Esfera
Esfera esfera(&pontoEsfera, 0.05);
// Matriz de blocos (instanciada no init)
Bloco*** matriz;
Aux aux;
// Vidas
Vertice pontoInicialVidas(tab.getTamBase()-0.1, tab.getTamAltura()+0.1, tab.getTamAlturaParedes()+0.05);

// spawnable objects

Vertice spawnPoint1(0.4, 2.3, 0.1);
Vertice spawnPoint2(1.15, 2.3, 0.1);
Esfera esferaSpawn1(&spawnPoint1, 0.1);
Esfera esferaSpawn2(&spawnPoint2, 0.1);
int colisaoReset;

// finds the time between the system clock
//(present time) and clock's epoch
unsigned seed = chrono::system_clock::now().time_since_epoch().count();

// minstd_rand0 is a standard
// linear_congruential_engine
minstd_rand0 generator (seed);

uniform_int_distribution<int> distribution(1,1);

///// Functions Declarations
/// OpenGL
void init(void);
void display(void);
void idle();
void motion(int x, int y);
void reshape (int w, int h);
void keyboard (unsigned char key, int x, int y);
void specialKeyboard (int key, int x, int y);
void mouse(int button, int state, int x, int y);
void look( int x, int y );
void setMaterial(int id);
void desenhaLanterna();


/// Main
int main(int argc, char** argv)
{
    // matriz de blocos
    gameController.criaMatrizBlocos(tab.getTamBase(), tab.getTamAltura());
    matriz = gameController.getMatrizBlocos();

    // glut init
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);// GLUT_DEPTH para alocar o z-buffer
    glutInitWindowSize (width, height);
    glutInitWindowPosition (600, 100);
    glutCreateWindow (argv[0]);

    /// interface
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    /// i/o
    glutMouseFunc( mouse );
    glutMotionFunc( motion );
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutSetCursor(GLUT_CURSOR_NONE);
    glutPassiveMotionFunc( look );

    /// Game
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}

/// Functions Definitions
/// OpenGL
void init(void)
{
    zdist = 4.0; // perspective
    rotationX = 0.0, rotationY = 0.0; // table rotation
    projecao = 1;
    ortho = 5;


    glClearColor (0.0, 0.0, 0.0, 0.0); // background Color
    glEnable(GL_CULL_FACE); // Back-face culling
    glShadeModel (GL_SMOOTH); // Flat Shading -> GL_FLAT; Gouraud Shading -> GL_SMOOTH
    glEnable(GL_DEPTH_TEST); // Para habilitar z-buffer

    glEnable(GL_LIGHTING); // habilita o OpenGL para fazer calculos com iluminacao
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);

    glCullFace(GL_BACK);

    // Define parametros do spotlight
    glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz0);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz0);
    glLightf(GL_LIGHT0,GL_SPOT_CUTOFF, cutoffAngle);
    glLightf(GL_LIGHT0,GL_SPOT_EXPONENT, 1.0);

    // Define parametros da luz que iluminará a lanterna
    glLightfv(GL_LIGHT1, GL_DIFFUSE, cor_luz0);
    glLightfv(GL_LIGHT1, GL_SPECULAR, cor_luz0);

    /// TODO LOAD OBJECTS
    objectManager = new glcWavefrontObject();
    objectManager->SetNumberOfObjects(NUM_OBJECTS);
    for(int i = 0; i < NUM_OBJECTS; i++)
    {
        objectManager->SelectObject(i);
        objectManager->ReadObject(objectFiles[i]);
        objectManager->Unitize();
        objectManager->FacetNormal();
        objectManager->VertexNormals(90.0);
        //objectManager->Scale(5);
    }

    esfera.setSpawn(0);
    esferaSpawn1.setSpawn(1);
    esferaSpawn2.setSpawn(2);
}

void display(void)
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT limpar z-buffer
    float w = width;
    float h = height;
    /// TODO LEMBRAR DE DESCOMENTAR ESSA CHAMADA PARA TRAVAR O MOUSE NO CENTRO
    if(!cameraLivre)glutWarpPointer(width/2, height/1.2);

    if(gameController.checaFinalDeJogo(&esfera, &pad))
    {
        gameController.restartGame(&esfera, &pad);
        gameController.resetaMatriz();

        // ganhou o jgoo
        if(gameController.getJogoVencido())
            cout << "Você Venceu!!!!!!" << endl;

        // perdeu o jogo
        //if(!gameController.getJogoVencido() && gameController.getNumVidas() <= 0)
        if(gameController.getFase() == 0 && gameController.getNumVidas() <= 0)
        {
            cout << "Você Perdeu!!!!!!" << endl;

            // volta para a fase 1 e o número de vidas iniciais
            //gameController.resetaFases();
            //gameController.resetaVidas();

            // reseta o jogo novamente após as mudanças nas fases e vidas
            //gameController.restartGame(&esfera, &pad);
            //gameController.resetaMatriz();
            //cout << "Jogo reiniciado" << endl;
        }
    }
    // inicializar sistema de projeção
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    if(!projecao)
    {
        if (width <= height)
        {
            glOrtho (0, 3, -1, 3*h/w, -200, 200);
        }
        else
        {
            glOrtho (-1, 2.5*w/h, -1, 3, -200, 200);
        }
    }
    else
    {
        gluPerspective(60.0f,(GLfloat)width/(GLfloat)height, 0.01f, 200.0f);
        if(!cameraLivre)gluLookAt (0.75, -0.5, 1.5, 0.75, 1.5, 0, 0, 1, 0);
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // rotação do tabuleiro com o mouse
    glPushMatrix();
    {
        /// iluminacao
        // Posiciona spotlight
        glPushMatrix();
        glTranslatef(2,2,5);
        glRotatef( -rotation, 0.0, 0.0, 1.0 );
        glTranslatef(-2, -2, -5);
        glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz0);
        glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,spotDir);
        glLightf(GL_LIGHT0,GL_SPOT_CUTOFF, cutoffAngle);
        glPopMatrix();


        if(cameraLivre)
        {
            gluLookAt (esfera.getPosicao()->getX(), esfera.getPosicao()->getY(), esfera.getPosicao()->getZ()+1.5,
                       esfera.getPosicao()->getX(), esfera.getPosicao()->getY(), esfera.getPosicao()->getZ(),
                       0, 1, 0);
            glRotatef( (GLfloat)rotationY *(180/M_PI), 0.0, 1.0, 0.0 );
            glRotatef( (GLfloat)rotationX *(180/M_PI), 1.0, 0.0, 0.0 );
            glRotatef( (GLfloat)rotationZ *(180/M_PI), 0.0, 0.0, 1.0 );
        }

        /// SET MATERIAL TABULEIRO
        setMaterial(0);
        /// DESENHA TABULEIRO
        desenha.desenhaTabuleiro(&tab);

        /// SET MATERIAL ESFERA
        setMaterial(1);
        /// DESENHA ESFERA
        desenha.desenhaEsfera(&esfera);

        /// SET MATERIAL DO PAD
        setMaterial(2);
        /// DESENHA PLAYER PAD
        desenha.desenhaBloco(pad.getPad());

        if(!gameController.getJogoIniciado())
        {
            desenha.desenhaSetaDirecao(&esfera,gameController.getAnguloDisparo());
        }

        // desenha bolinhas das vidas
        for(int ne = 0; ne < gameController.getNumVidas(); ne++)
        {
            Vertice* pv = new Vertice(pontoInicialVidas.getX() - ((double)ne)/10, pontoInicialVidas.getY(), pontoInicialVidas.getZ());
            Esfera* ev = new Esfera(pv, 0.03);
            desenha.desenhaEsfera(ev);
        }

        //setMaterial(3);
        setMaterial(gameController.getTipoMaterial());
        desenha.desenhaMatrizBlocos(matriz, gameController.getNumBlocosColunaMatriz(), gameController.getNumBlocosLinhaMatriz());

        desenha.desenhaVetorDirecaoEsfera(&esfera);
    }
    glPopMatrix();

    /// DESENHA OBJETOS IMPORTADOS
    glPushMatrix();
    {
        glTranslatef(esferaSpawn1.getPosicao()->getX(), esferaSpawn1.getPosicao()->getY(), esferaSpawn1.getPosicao()->getZ());
        objectManager->SelectObject(0);
        objectManager->SetShadingMode(SMOOTH_SHADING); // Possible values: FLAT_SHADING e SMOOTH_SHADING
        objectManager->SetRenderMode(USE_COLOR); // Possible values: USE_COLOR, USE_MATERIAL
        objectManager->Unitize();
        objectManager->Scale(0.3);
        objectManager->Draw();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(esferaSpawn2.getPosicao()->getX(), esferaSpawn2.getPosicao()->getY(), esferaSpawn2.getPosicao()->getZ());
        objectManager->SelectObject(1);
        objectManager->SetShadingMode(SMOOTH_SHADING); // Possible values: FLAT_SHADING e SMOOTH_SHADING
        objectManager->SetRenderMode(USE_MATERIAL); // Possible values: USE_COLOR, USE_MATERIAL
        objectManager->Unitize();
        objectManager->Scale(0.3);
        objectManager->Draw();
    }
    glPopMatrix();
    //if(drawboundingbox){objectManager->DrawBoundingBox();}

    glutSwapBuffers();
    glutPostRedisplay();
}

void idle ()
{
    if(gameController.getJogoIniciado())
    {
        float t, desiredFrameTime, frameTime;
        static float tLast = 0.0;

        // Get elapsed time
        t = glutGet(GLUT_ELAPSED_TIME);
        // convert milliseconds to seconds
        t /= 1000.0;

        // Calculate frame time
        frameTime = t - tLast;
        // Calculate desired frame time
        desiredFrameTime = 1.0 / (float) (desiredFPS);

        // Check if the desired frame time was achieved. If not, skip animation.
        if( frameTime <= desiredFrameTime)
        {
            return;
        }

        colisaoReset = aux.detectaColisao(&esfera, nullptr, matriz, &tab, &pad, &gameController, gameController.getNumBlocosColunaMatriz(), gameController.getNumBlocosLinhaMatriz(), gameController.getVelEsfera(), desiredFPS, true);
        aux.detectaColisao(&esferaSpawn1, &esfera, matriz, &tab, &pad, &gameController, gameController.getNumBlocosColunaMatriz(), gameController.getNumBlocosLinhaMatriz(), gameController.getVelEsfera(), desiredFPS, false);
        aux.detectaColisao(&esferaSpawn2, &esfera, matriz, &tab, &pad, &gameController, gameController.getNumBlocosColunaMatriz(), gameController.getNumBlocosLinhaMatriz(), gameController.getVelEsfera(), desiredFPS, false);
        aux.detectaColisao(&esferaSpawn1, &esferaSpawn2, matriz, &tab, &pad, &gameController, gameController.getNumBlocosColunaMatriz(), gameController.getNumBlocosLinhaMatriz(), gameController.getVelEsfera(), desiredFPS, false);

        // move esfera
        esfera.setPosicao(new Vertice(esfera.getPosicao()->getX() + (gameController.getVelEsfera()/desiredFPS) * esfera.getDirecao()->getX(),
                                      esfera.getPosicao()->getY() + (gameController.getVelEsfera()/desiredFPS) * esfera.getDirecao()->getY(),
                                      esfera.getPosicao()->getZ()));

        // move objetos de spawn
        if(!gameController.getSpawn1Fora())
        {
            esferaSpawn1.setPosicao(new Vertice(
                                        esferaSpawn1.getPosicao()->getX() + (gameController.getVelEsfera()/desiredFPS) * esferaSpawn1.getDirecao()->getX(),
                                        esferaSpawn1.getPosicao()->getY() + (gameController.getVelEsfera()/desiredFPS) * esferaSpawn1.getDirecao()->getY(),
                                        esferaSpawn1.getPosicao()->getZ()));
        }

        if(colisaoReset == 1 || gameController.getSpawn1Fora())
        {
            esferaSpawn1.setPosicao(new Vertice(10,10,0));
        }
        if(colisaoReset == 2 || !gameController.getJogoIniciado())
        {
            esferaSpawn1.setPosicao(new Vertice(0.4, 2.3, 0.1));
        }

        if(!gameController.getSpawn2Fora())
        {
            esferaSpawn2.setPosicao(new Vertice(esferaSpawn2.getPosicao()->getX() + (gameController.getVelEsfera()/desiredFPS) * esferaSpawn2.getDirecao()->getX(),
                                                esferaSpawn2.getPosicao()->getY() + (gameController.getVelEsfera()/desiredFPS) * esferaSpawn2.getDirecao()->getY(),
                                                esferaSpawn2.getPosicao()->getZ()
                                               ));
        }
        if(colisaoReset == 1 || gameController.getSpawn2Fora())
        {
            esferaSpawn2.setPosicao(new Vertice(10,10,0));
        }
        if(colisaoReset == 2 || !gameController.getJogoIniciado())
        {
            esferaSpawn2.setPosicao(new Vertice(1.15, 2.3, 0.1));
        }
        //cout << "Direcao: (" << esfera.getDirecao()->getX() << ", " << esfera.getDirecao()->getY() << ", " << esfera.getDirecao()->getZ() << ") "<< endl;
        tLast = t;
        glutPostRedisplay();
    }
    else
    {
        if(gameController.getAnguloDisparo() > -90)
        {
            Vertice* vertice = new Vertice(-fabs(cos((gameController.getAnguloDisparo()) * M_PI / 180.0)),
                                           fabs(sin((gameController.getAnguloDisparo()) * M_PI / 180.0)),
                                           esfera.getPosicao()->getZ());
            esfera.setDirecao(vertice);
        }
        else
        {
            Vertice* vertice = new Vertice(fabs(cos((gameController.getAnguloDisparo()) * M_PI / 180.0)),
                                           fabs(sin((gameController.getAnguloDisparo()) * M_PI / 180.0)),
                                           esfera.getPosicao()->getZ());
            esfera.setDirecao(vertice);
        }

//        if((esferaSpawn1.getDirecao()->getX() == 0.0 && esferaSpawn1.getDirecao()->getY() == 0.0 && esferaSpawn1.getDirecao()->getZ() == 0.0 ) ||
//        (esferaSpawn2.getDirecao()->getX() == 0.0 && esferaSpawn2.getDirecao()->getY() == 0.0 && esferaSpawn2.getDirecao()->getZ() == 0.0))
//        {
        double rX1 = distribution(generator);
        distribution.reset();
        double rY1 = distribution(generator);
        distribution.reset();
        double rX2 = distribution(generator);
        distribution.reset();
        double rY2 = distribution(generator);

        esferaSpawn1.setDirecao(new Vertice(rX1,rY1,0));
        esferaSpawn2.setDirecao(new Vertice(rX2,rY2,0));
//        }
    }
}

void reshape (int w, int h)
{
    width = w;
    height = h;

    glViewport (0, 0, (GLsizei) w, (GLsizei) h);
    glutPostRedisplay();
}

void keyboard (unsigned char key, int x, int y)
{

    switch (tolower(key))
    {
    //muda perspectiva
    case 'p':
        projecao= !projecao;
        if(!projecao)
        {
            printf("Projecao Ortogonal.\n");
            rotationX = 0.0, rotationY = 0.0;
        }
        else
        {
            printf("Projecao Perspectiva.\n");
        }
        break;

    case 32: // espaço deve pausar o jogo
        gameController.switchPause();
        break;
    case 'r':
        if(gameController.getJogoIniciado())
        {
            gameController.resetaFases();
            gameController.resetaVidas();

            // reseta o jogo novamente após as mudanças nas fases e vidas
            gameController.restartGame(&esfera, &pad);
            gameController.resetaMatriz();
            cout << "Jogo reiniciado" << endl;
        }
        break;
    case 'c': // movimentar camera
        cameraLivre = !cameraLivre;
        gameController.switchPause();
        break;
    case 27:
        exit(0);
        break;
    }
}

void specialKeyboard(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_F12:
        if(width != 800 && height != 600)
        {
            glutReshapeWindow(800,600);
        }
        else
        {
            glutFullScreen();
        }
        break;
    }
}

/** Motion callback
  *
  */
void motion(int x, int y)
{
    last_x = x;
    last_y = y;
    rotation += (float) (x - last_x);
}

/** Mouse callback
  *
  */
void mouse(int button, int state, int x, int y)
{
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
    {
        rotationX = 0.0, rotationY = 0.0;
        gameController.setJogoIniciado(true);
    }
    if(button == 3) // Scroll up
    {
        if(gameController.getAnguloDisparo() < -(90.0 - gameController.getAnguloDisparoMaximo()))
        {
            gameController.setAnguloDisparo(gameController.getAnguloDisparo() + gameController.getTaxaDeAumentoAngulo());
        }
    }
    if(button == 4) // Scroll Down
    {
        if(gameController.getAnguloDisparo() > -90.0 - gameController.getAnguloDisparoMaximo())
        {
            gameController.setAnguloDisparo(gameController.getAnguloDisparo()-gameController.getTaxaDeAumentoAngulo());
        }
    }
}

void look( int x, int y )
{
    if(gameController.getJogoIniciado() && (last_x-x > 0) && (!cameraLivre && !gameController.getJogoPausado()))
    {
        if(pad.getPad()->getVertice()->getX() <= (tab.getBase()->getTrianguloBase()->getVerticeA()->getX() + (pad.getPad()->getTamBase()/2)))
        {
            pad.getPad()->setVertice(new Vertice(
                                         (tab.getBase()->getTrianguloBase()->getVerticeA()->getX()),
                                         (pad.getPad()->getVertice()->getY()),
                                         (pad.getPad()->getVertice()->getZ())));
        }
        else
        {
            pad.getPad()->setVertice(new Vertice(
                                         (pad.getPad()->getVertice()->getX()-(gameController.getVelocidadePad()/desiredFPS)),
                                         (pad.getPad()->getVertice()->getY()),
                                         (pad.getPad()->getVertice()->getZ())));
        }
    }

    if(gameController.getJogoIniciado() && (last_x-x < 0) && (!cameraLivre && !gameController.getJogoPausado()))
    {
        if(pad.getPad()->getVertice()->getX() >= (tab.getBase()->getTrianguloBase()->getVerticeB()->getX() - (pad.getPad()->getTamBase())))
        {
            pad.getPad()->setVertice(new Vertice(
                                         (tab.getBase()->getTrianguloBase()->getVerticeB()->getX() - (pad.getPad()->getTamBase())),
                                         (pad.getPad()->getVertice()->getY()),
                                         (pad.getPad()->getVertice()->getZ())));
        }
        else
        {
            pad.getPad()->setVertice(new Vertice(
                                         (pad.getPad()->getVertice()->getX()+(gameController.getVelocidadePad()/desiredFPS)),
                                         (pad.getPad()->getVertice()->getY()),
                                         (pad.getPad()->getVertice()->getZ())));
        }
    }

    if(projecao != 0  && (cameraLivre && gameController.getJogoPausado()))
    {

        GLfloat deltaX = last_x - x;
        GLfloat deltaY = last_y - y;

        if ( deltaX < 0 )
        {
            deltaX = -1.0f;
        }
        if ( deltaX >  0 )
        {
            deltaX =  1.0f;
        }
        if ( deltaY < 0 )
        {
            deltaY = -1.0f;
        }
        if ( deltaY >  0 )
        {
            deltaY =  1.0f;
        }

        rotationX += (-deltaY)/desiredFPS;
        rotationY += (-deltaX)/desiredFPS;
        rotationZ += (-deltaX)/desiredFPS;
        last_x = x;
        last_y = y;
    }
}

void setMaterial(int id)
{
    glMaterialfv(GL_FRONT, GL_EMISSION, emissao);
    glMaterialfv(GL_FRONT, GL_SHININESS, brilho);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);

    switch(id)
    {
    case 0:
        glMaterialfv(GL_FRONT, GL_DIFFUSE, base_difusa);
        glMaterialfv(GL_FRONT, GL_SPECULAR, base_especular);
        break;
    case 1:
        glMaterialfv(GL_FRONT, GL_DIFFUSE, object_difusa);
        glMaterialfv(GL_FRONT, GL_SPECULAR, object_especular);
        break;
    case 2:
        glMaterialfv(GL_FRONT, GL_DIFFUSE, lanterna1);
        glMaterialfv(GL_FRONT, GL_SPECULAR, lanterna1);
        break;
    case 3:
        glMaterialfv(GL_FRONT, GL_DIFFUSE, lanterna2);
        glMaterialfv(GL_FRONT, GL_SPECULAR, lanterna2);
        break;
    }
}

void desenhaLanterna()
{
    glPushMatrix();
    setMaterial(2);
    glTranslatef(0.0, -1.0, 0.0);
    glutSolidCube(2.0);
    glTranslatef(0.0, 2.0, 0.0);
    glRotatef(90.0, 1.0, 0.0, 0.0);
    setMaterial(3);
    glutSolidCone(1.0, 3.0, 10, 10);
    glPopMatrix();
}

