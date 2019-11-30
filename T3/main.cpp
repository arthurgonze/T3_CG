#include <pthread.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <zlib.h>

#include <math.h>
#include <chrono>
#include <random>

#include "tabuleiro.h"
#include "triangulo.h"
#include "vertice.h"
#include "plano.h"
#include "esfera.h"
#include "bloco.h"
#include "desenha.h"
#include "pad.h"
#include "gameController.h"
#include "aux.h" // funcoes auxiliares ex CalcNormal
#include "glcWavefrontObject.h" // leitor de obj
#include "camera.h"

#define NUM_OBJECTS 2 // numero de objetos a serem importados

char arquivos_dos_objetos[NUM_OBJECTS][100] =
    {
        "/home/caveira/Documents/GitProjects/T3_CG/T3/data/obj/SuperMarioNormal.obj",
        "/home/caveira/Documents/GitProjects/T3_CG/T3/data/obj/SuperMarioNormal.obj"
    };

glcWavefrontObject *gerenciador_de_objetos = NULL;

/// VARIAVEIS GLOBAIS
int largura = 800, altura = 600; // Viewport
double skybox_largura, skybox_altura, skybox_profundidade;

int ultimo_x, ultimo_y; // MOUSE
double rotacao;
double fps_desejado = 60; // IDLE

bool orto_coord; // coordenadas  orto_coord
int projecao; // 0 ortogonal, 1 perspectiva
bool camera_livre = false;

double z_dist; // distancia no eixo Z utilizada na função gluLookAt
double rotacao_x, rotacao_y, rotacao_z; // posições iniciais em X e Y. Valores são alterados ao rotacionar o tabuleiro

int colisao_reset;

/// ILUMINACAO
GLfloat ambiente[4] = {0.3, 0.3, 0.3, 1.0};
GLfloat emissao[4] = {0.0, 0.0, 0.0, 0.0};
GLfloat brilho[4] = {128.0};

/// MATERIAIS
// Material da base (amarelo)
GLfloat base_difusa[4] = {1.0, 1.0, 0.0, 1.0};
GLfloat base_especular[4] = {1.0, 1.0, 0.0, 1.0};
// Material dos objetos a serem iluminados
GLfloat object_difusa[4] = {1.0, 0.65, 0.2, 1.0};
GLfloat object_especular[4] = {1.0, 0.65, 0.2, 1.0};
// Material da lanterna
GLfloat lanterna_1[4] = {1.0, 0.0, 0.0, 1.0}; // corpo
GLfloat lanterna_2[4] = {1.0, 1.0, 1.0, 1.0}; // ponta
// Define cor da luz ambiente
GLfloat cor_luz_amb[4] = {0.1, 0.1, 0.1, 1.0};
// Especificação da luz do spotlight
GLfloat cor_luz_ativa[4] = {1.0, 0.0, 0.0, 1.0};
GLfloat posicao_luz0[4] = {10.0, -15.0, 10.0, 1.0};
GLfloat cor_luz0[4] = {1.0, 1.0, 1.0, 1.0};
GLfloat spot_dir[3] = {0.0, 1.0, 0.0};
GLfloat angulo_de_corte = 45.0f;
// Especificação da luz que iluminará a lanterna
GLfloat cor_luz1[4] = {0.5, 0.5, 0.5, 1.0};

///OBJETOS
// GameController
GameController controlador_de_jogo;
// Desenha
Desenha desenha;
// Vertices
Vertice pos_inicial_tab(0.0, 0.0, 0.0);
Vertice ponto_pad(0.5, 0.1, 0.01);
Vertice ponto_esfera((0.5 + 0.9)/2, 0.3, 0.15);
Vertice ponto_skybox(-10.0, 0.0, -10.0);
// Tabuleiro
Tabuleiro tab(&pos_inicial_tab, 1.5, 2.5, 0.2);
// Pad
Pad pad(&ponto_pad, 0.4, 0.1, 0.1);
// Esfera
Esfera esfera(&ponto_esfera, 0.05);
//Skybox
//Vertice* verticePonto, double valorTamBase, double valorTamAltura, double valorTamProfundidade, bool blocoDestrutivel
Bloco *skybox;//
// Matriz de blocos (instanciada no init)
Bloco ***matriz;
Aux aux;
// Vidas
Vertice ponto_inicial_vidas(tab.pega_tam_base() - 0.1, tab.pega_tam_altura() + 0.1, tab.pega_tam_altura_paredes() + 0.05);
// Spawn objects
Vertice ponto_de_geracao_1(0.4, 2.3, 0.1);
Vertice ponto_de_geracao_2(1.15, 2.3, 0.1);
Esfera geracao_esfera_1(&ponto_de_geracao_1, 0.1);
Esfera geracao_esfera_2(&ponto_de_geracao_2, 0.1);

///GERACAO DE NUMEROS ALEATORIOS
// finds the time between the system clock(present time) and clock's epoch
unsigned seed = chrono::system_clock::now().time_since_epoch().count();
// minstd_rand0 is a standard linear_congruential_engine
minstd_rand0 generator(seed);
uniform_int_distribution<int> distribution(1, 1);

///CAMERA
Camera g_camera;
bool g_key[256];
bool fullscreen = false;    // Fullscreen Flag Set To Fullscreen Mode By Default
bool boost_speed = false; // Change keyboard speed
bool fly_mode = false;
bool release_mouse = false;

// Movement settings
float g_translation_speed = 0.05;
float g_rotation_speed = M_PI/180*0.2;
float initialY = 2; // initial height of the camera (flymode off value)

///// Functions Declarations
/// OpenGL
void init();
void display();
void idle();
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void specialKeyboard(int key, int x, int y);
void motion(int x, int y);
void mouse(int button, int state, int x, int y);
void KeyboardUp(unsigned char key, int x, int y);
void timer(int value);

/// auxiliares
void desenha_objetos();
void set_material(int id);
void iluminacao_tabuleiro();
void movimenta_camera_livre();
void perspectiva(float w, float h);
void is_game_over();
void checa_colisao();
void trata_colisao_esfera_esfera();
void move_objetos();
void angulo_de_disparo_inicial();
void configuracao_inicial_de_objetos_importados();

void movimenta_rebatedor(int x);
int main(int argc, char **argv)
{
    // matriz de blocos
    controlador_de_jogo.cria_matriz_blocos(tab.pega_tam_base(), tab.pega_tam_altura());
    matriz = controlador_de_jogo.pega_matriz_blocos();

    // glut init
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);// GLUT_DEPTH para alocar o z-buffer
    glutInitWindowSize(largura, altura);
    glutInitWindowPosition(600, 100);
    glutCreateWindow(argv[0]);

    glutIgnoreKeyRepeat(1);

    /// interface
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    /// i/o
    glutMouseFunc(mouse);
    //glutMotionFunc(motion);// for mouse clicks
    glutPassiveMotionFunc(motion);// for mouse motion without clicks
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutKeyboardUpFunc(KeyboardUp);

    glutTimerFunc(1, timer, 0);


    /// Game
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}

/// Functions Definitions
/// OpenGL
void init()
{
    z_dist = 4.0; // perspective
    rotacao_x = 0.0, rotacao_y = 0.0; // table rotacao
    projecao = 1;
    orto_coord = 5;

    glClearColor(0.0, 0.0, 0.0, 0.0); // background Color
    glEnable(GL_CULL_FACE); // Back-face culling
    glShadeModel(GL_SMOOTH); // Flat Shading -> GL_FLAT; Gouraud Shading -> GL_SMOOTH
    glEnable(GL_DEPTH_TEST); // Para habilitar z-buffer

    glEnable(GL_LIGHTING); // habilita o OpenGL para fazer calculos com iluminacao
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);

    glCullFace(GL_BACK);

    /// Define parametros do spotlight
    glLightfv(GL_LIGHT0, GL_AMBIENT, cor_luz_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, cor_luz0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, cor_luz0);
    glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz0);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, angulo_de_corte);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 1.0);

    // Define parametros da luz que iluminará a lanterna
    glLightfv(GL_LIGHT1, GL_DIFFUSE, cor_luz0);
    glLightfv(GL_LIGHT1, GL_SPECULAR, cor_luz0);

    /// LOAD OBJECTS
    gerenciador_de_objetos = new glcWavefrontObject();
    gerenciador_de_objetos->SetNumberOfObjects(NUM_OBJECTS);
    for (int i = 0; i < NUM_OBJECTS; i++)
    {
        gerenciador_de_objetos->SelectObject(i);
        gerenciador_de_objetos->ReadObject(arquivos_dos_objetos[i]);
        gerenciador_de_objetos->Unitize();
        gerenciador_de_objetos->FacetNormal();
        gerenciador_de_objetos->VertexNormals(90.0);
    }

    esfera.define_spawn(0);
    geracao_esfera_1.define_spawn(1);
    geracao_esfera_2.define_spawn(2);

    ///CAMERA
    glutSetCursor(GLUT_CURSOR_NONE);
    //0.694669 -0.125346 2.050183
    double pos[3] = {0.7, -0.125, 2.1};
    g_camera.SetPos(pos[0], pos[1], pos[2]);

    ///SKYBOX
    skybox_altura = 10;
    skybox_largura = 10;
    skybox_profundidade = 10;
    skybox = new Bloco(&ponto_skybox, skybox_largura, skybox_altura, skybox_profundidade, false);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // GL_DEPTH_BUFFER_BIT limpar z-buffer
    is_game_over();
    if (!camera_livre)
    {
        glutWarpPointer(largura/2, altura/1.2);
    }
    // inicializar sistema de projeção
    perspectiva(largura, altura);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // rotação do tabuleiro com o mouse
    glPushMatrix();
    {
        iluminacao_tabuleiro();

        movimenta_camera_livre();

        desenha_objetos();

        glutSwapBuffers();
        glutPostRedisplay();
    }
    glPopMatrix();
}

void idle()
{
    if (controlador_de_jogo.pega_jogo_iniciado())
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
        desiredFrameTime = 1.0/(float) (fps_desejado);

        // Check if the desired frame time was achieved. If not, skip animation.
        if (frameTime <= desiredFrameTime)
        {
            return;
        }

        checa_colisao();
        move_objetos();

        tLast = t;
        glutPostRedisplay();
    }
    else
    {
        angulo_de_disparo_inicial();

        configuracao_inicial_de_objetos_importados();

    }
}

void reshape(int w, int h)
{
    largura = w;
    altura = h;

    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode (GL_PROJECTION); //set the matrix to projection

    glLoadIdentity ();
    gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.1 , 1000.0); //set the perspective (angle of sight, width, height, ,depth)
    glMatrixMode (GL_MODELVIEW); //set the matrix back to model
//    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{

    switch (tolower(key))
    {
        //muda perspectiva
        case 'p':
            projecao = !projecao;
            if (!projecao)
            {
                printf("Projecao Ortogonal.\n");
                rotacao_x = 0.0, rotacao_y = 0.0;
            }
            else
            {
                printf("Projecao Perspectiva.\n");
            }
            break;

        case 32: // espaço deve pausar o jogo
            controlador_de_jogo.switch_pause();
            break;
        case 'r':
            if (controlador_de_jogo.pega_jogo_iniciado())
            {
                controlador_de_jogo.reseta_fases();
                controlador_de_jogo.reseta_vidas();

                // reseta o jogo novamente após as mudanças nas fases e vidas
                controlador_de_jogo.restart_game(&esfera, &pad);
                controlador_de_jogo.reseta_matriz();
                cout << "Jogo reiniciado" << endl;
            }
            break;
        case 'c': // movimentar camera
            camera_livre = !camera_livre;
            controlador_de_jogo.switch_pause();
            break;
        case 'b':
            boost_speed = !boost_speed;
            if (boost_speed)
            {
                g_translation_speed = 0.2;
            }
            else
            {
                g_translation_speed = 0.05;
            }
            if (boost_speed)
            {
                printf("BoostMode ON\n");
            }
            else
            {
                printf("BoostMode OFF\n");
            }
            break;
        case 'f':
            fly_mode = !fly_mode;
            if(fly_mode)
            {
                printf("FlyMode ON\n");
            }
            else
            {
                float x, y, z;
                printf("FlyMode OFF\n");
                g_camera.GetPos(x, y, z);
                g_camera.SetPos(x, initialY, z);
            }
            break;
        case 27:
            exit(0);
            break;
    }
    g_key[key] = true;
}

void specialKeyboard(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_F12:
            if (largura!=800 && altura!=600)
            {
                fullscreen = false;
                glutReshapeWindow(800, 600);
            }
            else
            {
                fullscreen = true;
                glutFullScreen();
            }
            break;
    }
}

void KeyboardUp(unsigned char key, int x, int y)
{
    g_key[key] = false;
}

void mouse(int button, int state, int x, int y)
{
    if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN) // Start Mouse click
    {
        rotacao_x = 0.0, rotacao_y = 0.0;
        controlador_de_jogo.define_jogo_iniciado(true);
    }
    if (button==3) // Scroll up
    {
        if (controlador_de_jogo.pega_angulo_disparo() < -(90.0 - controlador_de_jogo.pega_angulo_disparo_maximo()))
        {
            controlador_de_jogo.define_angulo_disparo(controlador_de_jogo.pega_angulo_disparo() + controlador_de_jogo.pega_taxa_de_aumento_angulo());
        }
    }
    if (button==4) // Scroll Down
    {
        if (controlador_de_jogo.pega_angulo_disparo() > -90.0 - controlador_de_jogo.pega_angulo_disparo_maximo())
        {
            controlador_de_jogo.define_angulo_disparo(controlador_de_jogo.pega_angulo_disparo() - controlador_de_jogo.pega_taxa_de_aumento_angulo());
        }
    }
}

void motion(int x, int y)
{
    movimenta_rebatedor(x);

    ///MOVIMENTA CAMERA
    // se a projecao nao for ortogonal e a camera estiver livre e o jogo pausado
    if (controlador_de_jogo.pega_jogo_iniciado() && projecao!=0 && (camera_livre && controlador_de_jogo.pega_jogo_pausado()))
    {
        //CAMERA
        // This variable is hack to stop glutWarpPointer from triggering an event callback to Mouse(...)
        // This avoids it being called recursively and hanging up the event loop
        static bool just_warped = false;

        if (just_warped)
        {
            just_warped = false;
            return;
        }

        int dx = x - largura/2;
        int dy = altura/2 - y;// camera invertida

        if (dx)
        {
            g_camera.RotateYaw(g_rotation_speed*dx);
        }
        if (dy)
        {
            g_camera.RotatePitch(g_rotation_speed*dy);
        }

        if (!release_mouse)
        {
            glutWarpPointer(largura/2, altura/2);
        }

        just_warped = true;
    }

    //motion
    ultimo_x = x;
    ultimo_y = y;
    rotacao += (double) (x - ultimo_x);
}

/// Auxiliares
void movimenta_rebatedor(int x)
{// se o jogo começou e o movimento em x for no eixo negativo e o jogo nao esta pausado nem em modo livre
    if (controlador_de_jogo.pega_jogo_iniciado() && (ultimo_x - x > 0.1) && (!camera_livre && !controlador_de_jogo.pega_jogo_pausado()))
    {
        // se a posicao x do rebatedor estiver fora dos limites da tela
        if (pad.pega_pad()->pega_vertice()->pega_x() <= (tab.pega_base()->pega_triangulo_base()->pega_vertice_a()->pega_x() +
            (pad.pega_pad()->pega_tam_base()/10)))
        {
            //a nova posiçao do pad vai ser o limite esquerdo da janela
            pad.pega_pad()->define_vertice(new Vertice(
                (tab.pega_base()->pega_triangulo_base()->pega_vertice_a()->pega_x()),
                (pad.pega_pad()->pega_vertice()->pega_y()),
                (pad.pega_pad()->pega_vertice()->pega_z())));
        }// Caso o rebatedor estiver dentro dos limites da tela
        else
        {
            //a nova posiçao do pad vai ser a pos x atual menos o deslocamento pelo FPS
            pad.pega_pad()->define_vertice(new Vertice(
                (pad.pega_pad()->pega_vertice()->pega_x() - (controlador_de_jogo.pega_velocidade_pad()/fps_desejado)),
                (pad.pega_pad()->pega_vertice()->pega_y()),
                (pad.pega_pad()->pega_vertice()->pega_z())));
        }
    }

    // se o jogo começou e o movimento em x for no eixo positivo e o jogo nao esta pausado nem em modo livre
    if (controlador_de_jogo.pega_jogo_iniciado() && (ultimo_x - x < -0.1) && (!camera_livre && !controlador_de_jogo.pega_jogo_pausado()))
    {
        // se a posicao x do rebatedor estiver fora dos limites da tela
        if (pad.pega_pad()->pega_vertice()->pega_x() >= (tab.pega_base()->pega_triangulo_base()->pega_vertice_b()->pega_x() -
            (pad.pega_pad()->pega_tam_base())))
        {
            //a nova posiçao do pad vai ser o limite direito da janela
            pad.pega_pad()->define_vertice(new Vertice(
                (tab.pega_base()->pega_triangulo_base()->pega_vertice_b()->pega_x() - (pad.pega_pad()->pega_tam_base())),
                (pad.pega_pad()->pega_vertice()->pega_y()),
                (pad.pega_pad()->pega_vertice()->pega_z())));
        }// Caso o rebatedor estiver dentro dos limites da tela
        else
        {
            //a nova posiçao do pad vai ser a pos x atual mais o deslocamento pelo FPS
            pad.pega_pad()->define_vertice(new Vertice(
                (pad.pega_pad()->pega_vertice()->pega_x() + (controlador_de_jogo.pega_velocidade_pad()/fps_desejado)),
                (pad.pega_pad()->pega_vertice()->pega_y()),
                (pad.pega_pad()->pega_vertice()->pega_z())));
        }
    }
}

void is_game_over()
{
    if (controlador_de_jogo.checa_final_de_jogo(&esfera, &pad))
    {
        controlador_de_jogo.restart_game(&esfera, &pad);
        controlador_de_jogo.reseta_matriz();

        // ganhou o jgoo
        if (controlador_de_jogo.pega_jogo_vencido())
        {
            cout << "Você Venceu!!!!!!" << endl;
        }

        // perdeu o jogo
        if (controlador_de_jogo.pega_fase()==0 && controlador_de_jogo.pega_num_vidas() <= 0)
        {
            cout << "Você Perdeu!!!!!!" << endl;
        }
    }
}

void perspectiva(float w, float h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (!projecao)
    {
        if (largura <= altura)
        {
            glOrtho(0, 3, -1, 3*h/w, -200, 200);
        }
        else
        {
            glOrtho(-1, 2.5*w/h, -1, 3, -200, 200);
        }
    }
    else
    {
        gluPerspective (60, (GLfloat)w / (GLfloat)h, 0.1 , 1000.0); //set the perspective (angle of sight, width, height, ,depth)
        if (!camera_livre)
        {
            gluLookAt(0.75, -0.5, 1.5, 0.75, 1.5, 0, 0, 1, 0);
        }
        else
        {
            g_camera.Refresh();
        }
    }
}

void movimenta_camera_livre()
{
    glPushMatrix();
    {
        if (camera_livre)
        {
            gluLookAt(esfera.pega_posicao()->pega_x(), esfera.pega_posicao()->pega_y(), esfera.pega_posicao()->pega_z() + 1.5,
                      esfera.pega_posicao()->pega_x(), esfera.pega_posicao()->pega_y(), esfera.pega_posicao()->pega_z(),
                      0, 1, 0);
            glRotatef((GLfloat) rotacao_y*(180/M_PI), 0.0, 1.0, 0.0);
            glRotatef((GLfloat) rotacao_x*(180/M_PI), 1.0, 0.0, 0.0);
            glRotatef((GLfloat) rotacao_z*(180/M_PI), 0.0, 0.0, 1.0);
        }
    }
    glPopMatrix();
}

void iluminacao_tabuleiro()
{/// iluminacao
    // Posiciona spotlight
    glPushMatrix();
    {
        glTranslatef(2, 2, 5);
        glRotatef(-rotacao, 0.0, 0.0, 1.0);
        glTranslatef(-2, -2, -5);
        glLightfv(GL_LIGHT0, GL_POSITION, posicao_luz0);
        glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_dir);
        glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, angulo_de_corte);
    }
    glPopMatrix();
}

void configuracao_inicial_de_objetos_importados()
{
    double rX1 = distribution(generator);
    distribution.reset();
    double rY1 = distribution(generator);
    distribution.reset();
    double rX2 = distribution(generator);
    distribution.reset();
    double rY2 = distribution(generator);

    geracao_esfera_1.define_direcao(new Vertice(rX1, rY1, 0));
    geracao_esfera_2.define_direcao(new Vertice(rX2, rY2, 0));
}

void angulo_de_disparo_inicial()
{
    if (controlador_de_jogo.pega_angulo_disparo() > -90)
    {
        Vertice *vertice = new Vertice(-fabs(cos((controlador_de_jogo.pega_angulo_disparo())*M_PI/180.0)),
                                       fabs(sin((controlador_de_jogo.pega_angulo_disparo())*M_PI/180.0)),
                                       esfera.pega_posicao()->pega_z());
        esfera.define_direcao(vertice);
    }
    else
    {
        Vertice *vertice = new Vertice(fabs(cos((controlador_de_jogo.pega_angulo_disparo())*M_PI/180.0)),
                                       fabs(sin((controlador_de_jogo.pega_angulo_disparo())*M_PI/180.0)),
                                       esfera.pega_posicao()->pega_z());
        esfera.define_direcao(vertice);
    }
}

void move_objetos()
{// move esfera
    esfera.define_posicao(new Vertice(esfera.pega_posicao()->pega_x() + (controlador_de_jogo.pega_vel_esfera()/fps_desejado)*esfera.pega_direcao()->pega_x(),
                                      esfera.pega_posicao()->pega_y() + (controlador_de_jogo.pega_vel_esfera()/fps_desejado)*esfera.pega_direcao()->pega_y(),
                                      esfera.pega_posicao()->pega_z()));

    // move objetos de spawn
    if (!controlador_de_jogo.pega_spawn1_fora())
    {
        geracao_esfera_1.define_posicao(new Vertice(
            geracao_esfera_1.pega_posicao()->pega_x() + (controlador_de_jogo.pega_vel_esfera()/fps_desejado)*geracao_esfera_1.pega_direcao()->pega_x(),
            geracao_esfera_1.pega_posicao()->pega_y() + (controlador_de_jogo.pega_vel_esfera()/fps_desejado)*geracao_esfera_1.pega_direcao()->pega_y(),
            geracao_esfera_1.pega_posicao()->pega_z()));
    }
}

void checa_colisao()
{
    colisao_reset = aux.detecta_colisao(&esfera, nullptr, matriz, &tab, &pad, &controlador_de_jogo,
                                        controlador_de_jogo.pega_num_blocos_coluna_matriz(), controlador_de_jogo.pega_num_blocos_linha_matriz(),
                                        controlador_de_jogo.pega_vel_esfera(), fps_desejado, true);

    aux.detecta_colisao(&geracao_esfera_1, &esfera, matriz, &tab, &pad, &controlador_de_jogo,
                        controlador_de_jogo.pega_num_blocos_coluna_matriz(), controlador_de_jogo.pega_num_blocos_linha_matriz(),
                        controlador_de_jogo.pega_vel_esfera(), fps_desejado, false);

    aux.detecta_colisao(&geracao_esfera_2, &esfera, matriz, &tab, &pad, &controlador_de_jogo,
                        controlador_de_jogo.pega_num_blocos_coluna_matriz(), controlador_de_jogo.pega_num_blocos_linha_matriz(),
                        controlador_de_jogo.pega_vel_esfera(), fps_desejado, false);

    aux.detecta_colisao(&geracao_esfera_1, &geracao_esfera_2, matriz, &tab, &pad, &controlador_de_jogo,
                        controlador_de_jogo.pega_num_blocos_coluna_matriz(), controlador_de_jogo.pega_num_blocos_linha_matriz(),
                        controlador_de_jogo.pega_vel_esfera(), fps_desejado, false);

    trata_colisao_esfera_esfera();

}

void trata_colisao_esfera_esfera()
{
    if (colisao_reset==1 || controlador_de_jogo.pega_spawn1_fora())
    {
        geracao_esfera_1.define_posicao(new Vertice(10, 10, 0));
    }
    if (colisao_reset==2 || !controlador_de_jogo.pega_jogo_iniciado())
    {
        geracao_esfera_1.define_posicao(new Vertice(0.4, 2.3, 0.1));
    }

    if (!controlador_de_jogo.pega_spawn2_fora())
    {
        geracao_esfera_2.define_posicao(new Vertice(geracao_esfera_2.pega_posicao()->pega_x() +
                                                        (controlador_de_jogo.pega_vel_esfera()/fps_desejado)*geracao_esfera_2.pega_direcao()->pega_x(),
                                                    geracao_esfera_2.pega_posicao()->pega_y() +
                                                        (controlador_de_jogo.pega_vel_esfera()/fps_desejado)*geracao_esfera_2.pega_direcao()->pega_y(),
                                                    geracao_esfera_2.pega_posicao()->pega_z()
        ));
    }
    if (colisao_reset==1 || controlador_de_jogo.pega_spawn2_fora())
    {
        geracao_esfera_2.define_posicao(new Vertice(10, 10, 0));
    }
    if (colisao_reset==2 || !controlador_de_jogo.pega_jogo_iniciado())
    {
        geracao_esfera_2.define_posicao(new Vertice(1.15, 2.3, 0.1));
    }
}

void desenha_objetos()
{
    glPushMatrix();
    {
        ///SET MATERIAL SKYBOX
//        set_material(4);
        ///DESENHA SKYBOX
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        desenha.desenha_skybox(skybox);

        /// SET MATERIAL TABULEIRO
        set_material(0);
        /// DESENHA TABULEIRO
        desenha.desenha_tabuleiro(&tab);

        /// SET MATERIAL ESFERA
        set_material(1);
        /// DESENHA ESFERA
        desenha.desenha_esfera(&esfera);

        /// SET MATERIAL DO PAD
        set_material(2);
        /// DESENHA PLAYER PAD
        desenha.desenha_bloco(pad.pega_pad());

        if (!controlador_de_jogo.pega_jogo_iniciado())
        {
            desenha.desenha_seta_direcao(&esfera, controlador_de_jogo.pega_angulo_disparo());
        }

        // desenha bolinhas das vidas
        for (int ne = 0; ne < controlador_de_jogo.pega_num_vidas(); ne++)
        {
            Vertice *pv = new Vertice(ponto_inicial_vidas.pega_x() - ((double) ne)/10, ponto_inicial_vidas.pega_y(),
                                      ponto_inicial_vidas.pega_z());

            Esfera *ev = new Esfera(pv, 0.03);
            desenha.desenha_esfera(ev);
        }

        //set_material(3);
        set_material(controlador_de_jogo.pega_tipo_material());
        desenha.desenha_matriz_blocos(matriz, controlador_de_jogo.pega_num_blocos_coluna_matriz(),
                                      controlador_de_jogo.pega_num_blocos_linha_matriz());

        desenha.desenha_vetor_direcao_esfera(&esfera);
    }
    glPopMatrix();

/// DESENHA OBJETOS IMPORTADOS
    glPushMatrix();
    {
        glTranslatef(geracao_esfera_1.pega_posicao()->pega_x(), geracao_esfera_1.pega_posicao()->pega_y(),
                     geracao_esfera_1.pega_posicao()->pega_z());

        gerenciador_de_objetos->SelectObject(0);
        gerenciador_de_objetos->SetShadingMode(SMOOTH_SHADING); // Possible values: FLAT_SHADING e SMOOTH_SHADING
        gerenciador_de_objetos->SetRenderMode(USE_COLOR); // Possible values: USE_COLOR, USE_MATERIAL
        gerenciador_de_objetos->Unitize();
        gerenciador_de_objetos->Scale(0.3);
        gerenciador_de_objetos->Draw();
    }
    glPopMatrix();

    glPushMatrix();
    {
        glTranslatef(geracao_esfera_2.pega_posicao()->pega_x(), geracao_esfera_2.pega_posicao()->pega_y(),
                     geracao_esfera_2.pega_posicao()->pega_z());

        gerenciador_de_objetos->SelectObject(1);
        gerenciador_de_objetos->SetShadingMode(SMOOTH_SHADING); // Possible values: FLAT_SHADING e SMOOTH_SHADING
        gerenciador_de_objetos->SetRenderMode(USE_MATERIAL); // Possible values: USE_COLOR, USE_MATERIAL
        gerenciador_de_objetos->Unitize();
        gerenciador_de_objetos->Scale(0.3);
        gerenciador_de_objetos->Draw();
    }
    glPopMatrix();
}

void set_material(int id)
{
    glMaterialfv(GL_FRONT, GL_EMISSION, emissao);
    glMaterialfv(GL_FRONT, GL_SHININESS, brilho);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambiente);

    switch (id)
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
            glMaterialfv(GL_FRONT, GL_DIFFUSE, lanterna_1);
            glMaterialfv(GL_FRONT, GL_SPECULAR, lanterna_1);
            break;
        case 3:
            glMaterialfv(GL_FRONT, GL_DIFFUSE, lanterna_2);
            glMaterialfv(GL_FRONT, GL_SPECULAR, lanterna_2);
            break;
        case 4:
            glMaterialfv(GL_FRONT, GL_DIFFUSE, object_difusa);
            glMaterialfv(GL_FRONT, GL_SPECULAR, object_especular);
            break;
    }
}

void timer(int value)
{
    float speed = g_translation_speed;

    if (g_key['w'] || g_key['W'])
    {
        g_camera.Move(speed, fly_mode);
    }
    else if (g_key['s'] || g_key['S'])
    {
        g_camera.Move(-speed, fly_mode);
    }
    else if (g_key['a'] || g_key['A'])
    {
        g_camera.Strafe(speed);
    }
    else if (g_key['d'] || g_key['D'])
    {
        g_camera.Strafe(-speed);
    }

    glutTimerFunc(1, timer, 0);
}
