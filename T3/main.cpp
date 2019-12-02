#include <pthread.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <zlib.h>

#include <math.h>
#include <chrono>
#include <random>
#include <include/skybox.h>

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
#include "glcTexture.h"

#define NUM_OBJECTS 2 // numero de objetos a serem importados

char arquivos_dos_objetos[NUM_OBJECTS][100] =
    {
        "./../T3/data/obj/SuperMarioNormal.obj",
        "./../T3/data/obj/SuperMarioNormal.obj"
    };

glcWavefrontObject *gerenciador_de_objetos = NULL;
glcTexture *textureManager;

/// VARIAVEIS GLOBAIS
int largura = 800, altura = 600; // Viewport
double skybox_largura, skybox_altura, skybox_profundidade;

int ultimo_x, ultimo_y; // MOUSE
double rotacao;

int projecao; // 0 ortogonal, 1 perspectiva
bool camera_livre = false;

int colisao_reset;

bool tela_inicial_ativa = true;

/** ILUMINAÇÃO **/
bool il_difusa = true;
bool il_ambiente = true;
bool il_especular = true;
// Guarda qual luz esta ativa e selecionada
int il_lightEnable[2] = {1, 1};
int il_activeLight = 2;
// Material do object (material branco)
GLfloat il_object_ambient[] = {0.3, 0.3, 0.3, 1.0};
GLfloat il_object_difusa[] = {1.0, 0.65, 0.2, 1.0};
GLfloat il_object_especular[] = {1.0, 0.65, 0.2, 1.0};
GLfloat il_object_emissao[] = {0.0, 0.0, 0.0, 0.0};
GLfloat il_object_brilho[] = {128.0};
// Material da base (amarelo)
GLfloat base_difusa[4] = {1.0, 1.0, 0.0, 1.0};
GLfloat base_especular[4] = {1.0, 1.0, 0.0, 1.0};

// Define cor para zerar emissao e luz ambiente
GLfloat il_sem_cor[] = {0.0, 0.0, 0.0, 1.0};

// Defini cor da luz ambiente
GLfloat il_cor_luz_amb[] = {0.3, 0.3, 0.3, 1.0};

// Modifica a cor da luz que esta ativa
GLfloat il_cor_luz_ativa[] = {1.0, 0.0, 0.0, 1.0};

// Cor e posicao da luz azul
GLfloat il_posicao_luz2[] = {5.0, 5.0, 8.0, 1.0};
GLfloat il_cor_luz2[] = {0.0, 0.0, 0.8, 1.0};

// Cor e posicao da luz amarela
GLfloat il_posicao_luz3[] = {-5.0, 5.0, 8.0, 1.0};
GLfloat il_cor_luz3[] = {1, 1, 1, 1.0};

// Material da lanterna
GLfloat lanterna_1[4] = {1.0, 0.0, 0.0, 1.0}; // corpo
GLfloat lanterna_2[4] = {1.0, 1.0, 1.0, 1.0}; // ponta
GLfloat lanterna_3[4] = {0.2, 0.0, 1.0, 1.0};
GLfloat lanterna_4[4] = {0.0, 0.5, 0.8, 1.0};
GLfloat lanterna_5[4] = {0.1, 0.7, 0.2, 1.0};
GLfloat angulo_de_corte = 90.0f;

///OBJETOS
// GameController
GameController controlador_de_jogo;
// Desenha
Desenha desenha;
// Vertices
Vertice pos_inicial_tab(0.0, 0.0, 0.0);
Vertice ponto_pad(0.5, 0.1, 0.01);
Vertice ponto_esfera((0.5 + 0.9)/2, 0.301, 0.15);
// Tabuleiro
Tabuleiro tab(&pos_inicial_tab, 1.5, 2.5, 0.2);
// Pad
Pad pad(&ponto_pad, 0.4, 0.1, 0.1);
// Esfera
Esfera esfera(&ponto_esfera, 0.05);
// Matriz de blocos (instanciada no init)
Bloco ***matriz;
Aux aux;
// Vidas
Vertice ponto_inicial_vidas(pos_inicial_tab.pega_x() + tab.pega_tam_base() - 0.1, tab.pega_tam_altura() + 0.1, tab.pega_tam_altura_paredes() + 0.05);
// Spawn objects
Vertice ponto_de_geracao_1(0.4, 2.3, 0.1);
Vertice ponto_de_geracao_2(1.15, 2.3, 0.1);
Esfera geracao_esfera_1(&ponto_de_geracao_1, 0.1);
Esfera geracao_esfera_2(&ponto_de_geracao_2, 0.1);
// Skybox
Skybox *skybox;

///GERACAO DE NUMEROS ALEATORIOS
// finds the time between the system clock(present time) and clock's epoch
unsigned seed = chrono::system_clock::now().time_since_epoch().count();
// minstd_rand0 is a standard linear_congruential_engine
minstd_rand0 generator(seed);
uniform_int_distribution<int> distribution(1, 1);

///CAMERA
Camera camera;
bool g_key[256];
bool fullscreen = false;    // Fullscreen Flag Set To Fullscreen Mode By Default
bool boost_speed = true; // Change keyboard speed
bool fly_mode = true;
bool release_mouse = false;

// Movement settings
float translation_speed = 0.2;
float rotation_speed = M_PI/180*0.2;
float initial_y = 2; // initial height of the camera (flymode off value)
bool rotacao_em_conjunto = false; // rotacionar camera e skybox junto no modo inspeção

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
void perspectiva(float w, float h);
void is_game_over();
void checa_colisao();
void trata_colisao_esfera_esfera();
void move_objetos();
void angulo_de_disparo_inicial();
void configuracao_inicial_de_objetos_importados();
void movimenta_rebatedor(int x);
void monta_skybox();
void iluminacao();

int main(int argc, char **argv)
{
    // matriz de blocos
    controlador_de_jogo.cria_matriz_blocos(tab.pega_tam_base(), tab.pega_tam_altura());
    matriz = controlador_de_jogo.pega_matriz_blocos();

    // glut init
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);// GLUT_DEPTH para alocar o z-buffer
    glutInitWindowSize(largura, altura);
    glutInitWindowPosition(500, 100);
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
    projecao = 1;

    glClearColor(0.0, 0.0, 0.0, 0.0); // background Color
//    glEnable(GL_CULL_FACE); // Back-face culling
    glShadeModel(GL_SMOOTH); // Flat Shading -> GL_FLAT; Gouraud Shading -> GL_SMOOTH
    glEnable(GL_DEPTH_TEST); // Para habilitar z-buffer

    glEnable(GL_LIGHTING); // habilita o OpenGL para fazer calculos com iluminacao
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);

//    glCullFace(GL_BACK);

    /** ILUMINAÇÃO **/
    glEnable(GL_LIGHT2);             // habilita luz 2
    glEnable(GL_LIGHT3);             // habilita luz 3
    // Define parametros da luz 2
    glLightfv(GL_LIGHT2, GL_AMBIENT, il_cor_luz_amb);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, il_cor_luz2);
    glLightfv(GL_LIGHT2, GL_SPECULAR, il_cor_luz2);
    glLightfv(GL_LIGHT2, GL_POSITION, il_posicao_luz2);
    // Define parametros da luz 3
    glLightfv(GL_LIGHT3, GL_AMBIENT, il_cor_luz_amb);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, il_cor_luz3);
    glLightfv(GL_LIGHT3, GL_SPECULAR, il_cor_luz3);
    glLightfv(GL_LIGHT3, GL_POSITION, il_posicao_luz3);

    printf("Controles da iluminação:\n");
    printf("  'u' para habilitar/desabilitar luz DIFUSA.\n");
    printf("  'i' para habilitar/desabilitar luz ESPECULAR.\n");
    printf("  'o' para habilitar/desabilitar luz AMBIENT.\n");
    printf("  '3' ou '4' para selecionar as fontes de luz.\n");
    printf("  'F1' ou 'F2' para ligar/desligar as fontes de luz.\n");
    printf("  Direcionais do teclado movem a luz selecionada em X e Y.\n");
    printf("  'PageUp' e 'PageDown' movem a luz selecionada em Z.\n");

    ///TEXTURAS
    glEnable(GL_ALPHA_TEST);      // O alpha test descarta fragmentos dependendo de uma comparação (abaixo)
    glAlphaFunc(GL_GREATER, 0.1); // Info: https://www.opengl.org/sdk/docs/man2/xhtml/glAlphaFunc.xml

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //  https://www.opengl.org/sdk/docs/man/html/glBlendFunc.xhtml

    textureManager = new glcTexture();            // Criação do arquivo que irá gerenciar as texturas
    textureManager->SetNumberOfTextures(5);       // Estabelece o número de texturas que será utilizado
    textureManager->CreateTexture("../T3/data/textures/skybox.png", 0);
    textureManager->CreateTexture("../T3/data/textures/startScreen.png", 1);
    textureManager->CreateTexture("../T3/data/textures/paredes_frente2.png", 2);
    textureManager->CreateTexture("../T3/data/textures/paredes_sup.png", 3);
    textureManager->CreateTexture("../T3/data/textures/rebatedor_frente.png", 4);

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
    double pos[3] = {1.66, 0.14, 2.06};
    camera.SetPos(pos[0], pos[1], pos[2]);

    ///SKYBOX
    skybox_altura = 50;
    skybox_largura = 50;
    skybox_profundidade = 50;

    monta_skybox();

    ///GAME CONTROLLER
    controlador_de_jogo.define_fps_desejado(60);
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

        desenha_objetos();

        if (tela_inicial_ativa)
        {
            textureManager->Bind(1);
            glPushMatrix();
            {
                textureManager->SetWrappingMode(GL_CLAMP);
                textureManager->SetMinFilterMode(GL_LINEAR);
                textureManager->SetMagFilterMode(GL_LINEAR);
                textureManager->SetColorMode(GL_MODULATE);

                glBegin(GL_QUADS);
                {
                    glNormal3f(0.0, 0.0, 1.0);

                    glTexCoord2f(0, 0);
                    glVertex3f(-0.8, -0.25, 0.2);

                    glTexCoord2f(1, 0);
                    glVertex3f(2, -0.25, 0.2);

                    glTexCoord2f(1, 1);
                    glVertex3f(2, 2, 1.1); //1.128

                    glTexCoord2f(0, 1);
                    glVertex3f(-0.8, 2, 1.1);
                }
                glEnd();
            }
            glPopMatrix();
        }

        iluminacao();

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
        desiredFrameTime = 1.0/(float) (controlador_de_jogo.pega_fps_desejado());

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
    glMatrixMode(GL_PROJECTION); //set the matrix to projection

    glLoadIdentity();
    gluPerspective(60, (GLfloat) w/(GLfloat) h, 0.1, 1000.0); //set the perspective (angle of sight, width, height, ,depth)
    glMatrixMode(GL_MODELVIEW); //set the matrix back to model
//    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
    if (!tela_inicial_ativa)
    {
        switch (tolower(key))
        {
            //muda perspectiva
            case 'p':
                projecao = !projecao;
                if (!projecao)
                {
                    printf("Projecao Ortogonal.\n");
                }
                else
                {
                    printf("Projecao Perspectiva.\n");
                }
                break;

            case 32: // espaço deve pausar/retomar o jogo somente se a camera estiver fixa
                if (!camera_livre)
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
                // SE jogo rodando e camera fixa
                if (!camera_livre && !controlador_de_jogo.pega_jogo_pausado())
                {
                    camera_livre = !camera_livre; // camera se torna livre
                    controlador_de_jogo.switch_pause(); // pausa o jogo
                }

                    // SE jogo pausado e camera fixa
                else if (!camera_livre && controlador_de_jogo.pega_jogo_pausado())
                    camera_livre = !camera_livre; // camera se torna livre

                    // SE camera livre (entao o jogo estará pausado)
                else if (camera_livre)
                    camera_livre = !camera_livre; // camera se torna fixa

                break;
            case 'b':
                boost_speed = !boost_speed;
                if (boost_speed)
                {
                    translation_speed = 0.2;
                }
                else
                {
                    translation_speed = 0.01;
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
                if (fly_mode)
                {
                    printf("FlyMode ON\n");
                }
                else
                {
                    float x, y, z;
                    printf("FlyMode OFF\n");
                    camera.GetPos(x, y, z);
                    camera.SetPos(x, initial_y, z);
                }
                break;
            case 'l':
                rotacao_em_conjunto = !rotacao_em_conjunto;
                if (rotacao_em_conjunto)
                {
                    printf("Rotacao em Conjunto ON\n");
                }
                else
                {
                    printf("Rotacao em Conjunto OFF\n");
                }
                break;

                /** ILUMINAÇÃO - TESTE **/
            case 'u' :
                if (camera_livre)
                    il_difusa = !il_difusa;
                break;
            case 'i' :
                if (camera_livre)
                    il_especular = !il_especular;
                break;
            case 'o' :
                if (camera_livre)
                    il_ambiente = !il_ambiente;
                break;
            case '3' :
                if (camera_livre)
                    il_activeLight = 2;
                break;
            case '4' :
                if (camera_livre)
                    il_activeLight = 3;
                break;
                /** ~ ILUMINAÇÃO - TESTE **/

            case 27:
                exit(0);
                break;
        }
        g_key[key] = true;
    }
    glutPostRedisplay();
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

            /** ILUMINAÇÃO - TESTE **/
        case GLUT_KEY_LEFT:
            if (camera_livre)
                (!il_activeLight) ? il_posicao_luz2[0] -= 0.2 : il_posicao_luz3[0] -= 0.2;
            break;
        case GLUT_KEY_RIGHT:
            if (camera_livre)
                (!il_activeLight) ? il_posicao_luz2[0] += 0.2 : il_posicao_luz3[0] += 0.2;
            break;
        case GLUT_KEY_UP:
            if (camera_livre)
                (!il_activeLight) ? il_posicao_luz2[1] += 0.2 : il_posicao_luz3[1] += 0.2;
            break;
        case GLUT_KEY_DOWN:
            if (camera_livre)
                (!il_activeLight) ? il_posicao_luz2[1] -= 0.2 : il_posicao_luz3[1] -= 0.2;
            break;
        case GLUT_KEY_PAGE_DOWN:
            if (camera_livre)
                (!il_activeLight) ? il_posicao_luz2[2] -= 0.2 : il_posicao_luz3[2] -= 0.2;
            break;
        case GLUT_KEY_PAGE_UP:
            if (camera_livre)
                (!il_activeLight) ? il_posicao_luz2[2] += 0.2 : il_posicao_luz3[2] += 0.2;
            break;
        case GLUT_KEY_F1:
            if (camera_livre)
            {
                (il_lightEnable[0]) ? glDisable(GL_LIGHT2) : glEnable(GL_LIGHT2);
                il_lightEnable[0] = !il_lightEnable[0];
            }
            break;
        case GLUT_KEY_F2:
            if (camera_livre)
            {
                (il_lightEnable[1]) ? glDisable(GL_LIGHT3) : glEnable(GL_LIGHT3);
                il_lightEnable[1] = !il_lightEnable[1];
            }
            break;
            /** ~ ILUMINAÇÃO - TESTE **/

    }
}

void KeyboardUp(unsigned char key, int x, int y)
{
    g_key[key] = false;
}

void mouse(int button, int state, int x, int y)
{
    if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN && tela_inicial_ativa)
    {
        tela_inicial_ativa = false;
    }
    else
    {
        // dispara a bolinha ao clicar somente se o jogo não tiver acabado
        if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN && controlador_de_jogo.pega_fase() > 0)
            //if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN) // Start Mouse click
        {
            controlador_de_jogo.define_jogo_iniciado(true);
        }
        if (button==3 && !tela_inicial_ativa) // Scroll up
        {
            if (controlador_de_jogo.pega_angulo_disparo() < -(90.0 - controlador_de_jogo.pega_angulo_disparo_maximo()))
            {
                controlador_de_jogo.define_angulo_disparo(controlador_de_jogo.pega_angulo_disparo() + controlador_de_jogo.pega_taxa_de_aumento_angulo());
            }
        }
        if (button==4 && !tela_inicial_ativa) // Scroll Down
        {
            if (controlador_de_jogo.pega_angulo_disparo() > -90.0 - controlador_de_jogo.pega_angulo_disparo_maximo())
            {
                controlador_de_jogo.define_angulo_disparo(controlador_de_jogo.pega_angulo_disparo() - controlador_de_jogo.pega_taxa_de_aumento_angulo());
            }
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
            camera.RotateYaw(rotation_speed*dx);
        }
        if (dy)
        {
            camera.RotatePitch(rotation_speed*dy);
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
    if (controlador_de_jogo.pega_jogo_iniciado() && (ultimo_x - x > 0) && (!camera_livre && !controlador_de_jogo.pega_jogo_pausado()))
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
                (pad.pega_pad()->pega_vertice()->pega_x() - (controlador_de_jogo.pega_velocidade_pad()/controlador_de_jogo.pega_fps_desejado())),
                (pad.pega_pad()->pega_vertice()->pega_y()),
                (pad.pega_pad()->pega_vertice()->pega_z())));
        }
    }

    // se o jogo começou e o movimento em x for no eixo positivo e o jogo nao esta pausado nem em modo livre
    if (controlador_de_jogo.pega_jogo_iniciado() && (ultimo_x - x < 0) && (!camera_livre && !controlador_de_jogo.pega_jogo_pausado()))
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
                (pad.pega_pad()->pega_vertice()->pega_x() + (controlador_de_jogo.pega_velocidade_pad()/controlador_de_jogo.pega_fps_desejado())),
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
        gluPerspective(60, (GLfloat) w/(GLfloat) h, 0.1, 1000.0); //set the perspective (angle of sight, width, height, ,depth)
        if (!camera_livre)
        {
            gluLookAt(0.53, -0.004, 1.95, 0.545, 0.5, 1.09, 0, 1, 0);
//            Camera: 0.527858 -0.003684 1.952040 Direction vector: 0.015084 0.503020 -0.864143
        }
        else
        {
            camera.Refresh();
        }
    }
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
    esfera.define_posicao(new Vertice(esfera.pega_posicao()->pega_x() + (controlador_de_jogo.pega_vel_esfera()/controlador_de_jogo.pega_fps_desejado())*esfera.pega_direcao()->pega_x(),
                                      esfera.pega_posicao()->pega_y() + (controlador_de_jogo.pega_vel_esfera()/controlador_de_jogo.pega_fps_desejado())*esfera.pega_direcao()->pega_y(),
                                      esfera.pega_posicao()->pega_z()));

    // move objetos de spawn
    if (!controlador_de_jogo.pega_spawn1_fora())
    {
        geracao_esfera_1.define_posicao(new Vertice(
            geracao_esfera_1.pega_posicao()->pega_x() + (controlador_de_jogo.pega_vel_esfera()/controlador_de_jogo.pega_fps_desejado())*geracao_esfera_1.pega_direcao()->pega_x(),
            geracao_esfera_1.pega_posicao()->pega_y() + (controlador_de_jogo.pega_vel_esfera()/controlador_de_jogo.pega_fps_desejado())*geracao_esfera_1.pega_direcao()->pega_y(),
            geracao_esfera_1.pega_posicao()->pega_z()));
    }
}

void checa_colisao()
{
    // detectar esfera x blocos
    aux.detecta_colisao_esfera_blocos(&esfera, matriz, &controlador_de_jogo, true);
    // detectar obj1 x blocos
    aux.detecta_colisao_esfera_blocos(&geracao_esfera_1, matriz, &controlador_de_jogo, false);
    // detectar obj2 x blocos
    aux.detecta_colisao_esfera_blocos(&geracao_esfera_2, matriz, &controlador_de_jogo, false);

    // detectar esfera x tabuleiro
    aux.detecta_colisao_esfera_tabuleiro(&esfera, &tab, &controlador_de_jogo, &pad, true);
    // detectar obj1 x tabuleiro
    aux.detecta_colisao_esfera_tabuleiro(&geracao_esfera_1, &tab, &controlador_de_jogo, &pad, false);
    // detectar obj2 x tabuleiro
    aux.detecta_colisao_esfera_tabuleiro(&geracao_esfera_2, &tab, &controlador_de_jogo, &pad, false);

    // detectar esfera x rebatedor
    aux.detecta_colisao_esfera_rebatedor(&esfera, &pad, &controlador_de_jogo);
    // detectar obj1 x rebatedor
    aux.detecta_colisao_esfera_rebatedor(&geracao_esfera_1, &pad, &controlador_de_jogo);
    // detectar obj2 x rebatedor
    aux.detecta_colisao_esfera_rebatedor(&geracao_esfera_2, &pad, &controlador_de_jogo);


    // detectar esfera x obj1
    aux.detecta_colisao_esfera_objetos_importados(&esfera, &geracao_esfera_1, &controlador_de_jogo);
    // detectar esfera x obj2
    aux.detecta_colisao_esfera_objetos_importados(&esfera, &geracao_esfera_2, &controlador_de_jogo);
    // detectar obj1 x obj2
    aux.detecta_colisao_esfera_objetos_importados(&geracao_esfera_1, &geracao_esfera_2, &controlador_de_jogo);

    glBegin(GL_TRIANGLES);
    {
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(tab.pega_Centro_x_leste(), tab.pega_Centro_y_leste(), esfera.pega_posicao()->pega_z());
        glVertex3f(esfera.pega_posicao()->pega_x(), esfera.pega_posicao()->pega_y(), esfera.pega_posicao()->pega_z());
        glVertex3f(0, 0, esfera.pega_posicao()->pega_z());
    }
    glEnd();

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
                                                        (controlador_de_jogo.pega_vel_esfera()/controlador_de_jogo.pega_fps_desejado())*geracao_esfera_2.pega_direcao()->pega_x(),
                                                    geracao_esfera_2.pega_posicao()->pega_y() +
                                                        (controlador_de_jogo.pega_vel_esfera()/controlador_de_jogo.pega_fps_desejado())*geracao_esfera_2.pega_direcao()->pega_y(),
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
        ///DESENHA SKYBOX
        desenha.desenha_skybox(textureManager, &camera, skybox, rotacao_em_conjunto);

        /// SET MATERIAL TABULEIRO
        set_material((controlador_de_jogo.pega_tipo_material() + 1)%3);
        /// DESENHA TABULEIRO
        desenha.desenha_tabuleiro(&tab, textureManager);

        /// SET MATERIAL ESFERA
        set_material((controlador_de_jogo.pega_tipo_material() + 2)%3);
        /// DESENHA ESFERA
        desenha.desenha_esfera(&esfera);

        /// SET MATERIAL DO PAD
        set_material((controlador_de_jogo.pega_tipo_material() + 5)%6);
        /// DESENHA PLAYER PAD
        desenha.desenha_rebatedor(&pad, textureManager);

        /// Desenha seta de direcao
        if (!controlador_de_jogo.pega_jogo_iniciado())
        {
            desenha.desenha_seta_direcao(&esfera, controlador_de_jogo.pega_angulo_disparo());
        }

        /// Desenha matriz de blocos
        set_material(controlador_de_jogo.pega_tipo_material());
        desenha.desenha_matriz_blocos(matriz, controlador_de_jogo.pega_num_blocos_coluna_matriz(),
                                      controlador_de_jogo.pega_num_blocos_linha_matriz());

        ///Desenha direcao esfera
        //desenha.desenha_vetor_direcao_esfera(&esfera);

        /// Desenha texto da fase
        desenha.desenha_texto_nivel(&controlador_de_jogo);

        /// Desenha vidas
        desenha.desenha_vidas(&controlador_de_jogo, &ponto_inicial_vidas);

    }
    glPopMatrix();

    /// DESENHA OBJETOS IMPORTADOS
    desenha.desenha_objetos_importados(gerenciador_de_objetos, &geracao_esfera_1, &geracao_esfera_2);
}

void set_material(int id)
{
    glMaterialfv(GL_FRONT, GL_EMISSION, il_object_emissao);
    glMaterialfv(GL_FRONT, GL_SHININESS, il_object_brilho);
    glMaterialfv(GL_FRONT, GL_AMBIENT, il_object_ambient);

    switch (id)
    {
        case 0:
            glMaterialfv(GL_FRONT, GL_DIFFUSE, base_difusa);
            glMaterialfv(GL_FRONT, GL_SPECULAR, base_especular);
            break;
        case 1:
            glMaterialfv(GL_FRONT, GL_DIFFUSE, il_object_difusa);
            glMaterialfv(GL_FRONT, GL_SPECULAR, il_object_especular);
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
            glMaterialfv(GL_FRONT, GL_DIFFUSE, lanterna_3);
            glMaterialfv(GL_FRONT, GL_SPECULAR, lanterna_3);
            break;
        case 5:
            glMaterialfv(GL_FRONT, GL_DIFFUSE, lanterna_4);
            glMaterialfv(GL_FRONT, GL_SPECULAR, lanterna_4);
            break;
        case 6:
            glMaterialfv(GL_FRONT, GL_DIFFUSE, lanterna_5);
            glMaterialfv(GL_FRONT, GL_SPECULAR, lanterna_5);
            break;
        case 7:
            glMaterialfv(GL_FRONT, GL_DIFFUSE, il_object_difusa);
            glMaterialfv(GL_FRONT, GL_SPECULAR, il_object_especular);
            break;
    }
}

void timer(int value)
{
    float speed = translation_speed;

    if (g_key['w'] || g_key['W'])
    {
        camera.Move(speed, fly_mode);
    }
    else if (g_key['s'] || g_key['S'])
    {
        camera.Move(-speed, fly_mode);
    }
    else if (g_key['a'] || g_key['A'])
    {
        camera.Strafe(speed);
    }
    else if (g_key['d'] || g_key['D'])
    {
        camera.Strafe(-speed);
    }

    glutTimerFunc(1, timer, 0);
}

void monta_skybox()
{
    Vertice v1(-(skybox_largura/2), -(skybox_altura/2), (skybox_profundidade/2));
    Vertice v2((skybox_largura/2), -(skybox_altura/2), (skybox_profundidade/2));
    Vertice v3((skybox_largura/2), (skybox_altura/2), (skybox_profundidade/2));
    Vertice v4(-(skybox_largura/2), (skybox_altura/2), (skybox_profundidade/2));
    Vertice v5(-(skybox_largura/2), -(skybox_altura/2), -(skybox_profundidade/2));
    Vertice v6((skybox_largura/2), -(skybox_altura/2), -(skybox_profundidade/2));
    Vertice v7((skybox_largura/2), (skybox_altura/2), -(skybox_profundidade/2));
    Vertice v8(-(skybox_largura/2), (skybox_altura/2), -(skybox_profundidade/2));
    skybox = new Skybox(v1, v2, v3, v4, v5, v6, v7, v8);
}

void iluminacao()
{
    // Define luz 1
    if (il_lightEnable[0])
    {
        // Posiciona esfera que representa a fonte de luz 2 no mundo
        glPushMatrix();
        glTranslatef(il_posicao_luz2[0], il_posicao_luz2[1], il_posicao_luz2[2]);
        glLightfv(GL_LIGHT0, GL_POSITION, il_posicao_luz2);
        // Informa que a superficie abaixo sera de emissao (fonte de luz) e defini cor
        if (il_activeLight==2)
            glMaterialfv(GL_FRONT, GL_EMISSION, il_cor_luz_ativa);
        else
            glMaterialfv(GL_FRONT, GL_EMISSION, il_cor_luz2);
        glutSolidSphere(0.2, 30, 30);
        glPopMatrix();
    }
    // Define luz 2
    if (il_lightEnable[1])
    {
        glPushMatrix();
        glTranslatef(il_posicao_luz3[0], il_posicao_luz3[1], il_posicao_luz3[2]);
        glLightfv(GL_LIGHT1, GL_POSITION, il_posicao_luz3);
        if (il_activeLight==3)
            glMaterialfv(GL_FRONT, GL_EMISSION, il_cor_luz_ativa);
        else
            glMaterialfv(GL_FRONT, GL_EMISSION, il_cor_luz3);
        glutSolidSphere(0.2, 30, 30);
        glMaterialfv(GL_FRONT, GL_EMISSION, il_sem_cor);
        glPopMatrix();
    }
    // Define os parametros da superficie a ser iluminada
    if (il_ambiente)
        glMaterialfv(GL_FRONT, GL_AMBIENT, il_object_ambient);
    else
        glMaterialfv(GL_FRONT, GL_AMBIENT, il_sem_cor);

    if (il_difusa)
        glMaterialfv(GL_FRONT, GL_DIFFUSE, il_object_difusa);
    else
        glMaterialfv(GL_FRONT, GL_DIFFUSE, il_sem_cor);

    if (il_especular)
        glMaterialfv(GL_FRONT, GL_SPECULAR, il_object_especular);
    else
        glMaterialfv(GL_FRONT, GL_SPECULAR, il_sem_cor);

    glMaterialfv(GL_FRONT, GL_EMISSION, il_object_emissao);
    glMaterialfv(GL_FRONT, GL_SHININESS, il_object_brilho);
}