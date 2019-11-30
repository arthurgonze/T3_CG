#include "desenha.h"

Desenha::Desenha() {}

Desenha::~Desenha() {/*dtor*/}

/** Desenha um triangulo seguindo os vértices passados por parâmetro.
  * Será utilizada para criar as faces triangulares do prisma (desenhaPrisma),
  * da base do tabuleiro (desenhaTabuleiro) e suas paredes (desenhaParedes)
  */
void Desenha::desenha_triangulo(Triangulo *triangulo, bool inv, int normal_dir)
{
    glBegin(GL_TRIANGLES);
    {
        glNormal3f(
            (GLfloat) triangulo->pega_vetor_normal()->pega_x(),
            (GLfloat) triangulo->pega_vetor_normal()->pega_y(),
            (GLfloat) triangulo->pega_vetor_normal()->pega_z()
        );
        if (!inv)
        {
            glVertex3d(
                triangulo->pega_vertice_a()->pega_x(),
                triangulo->pega_vertice_a()->pega_y(),
                triangulo->pega_vertice_a()->pega_z()
            );

            glVertex3d(
                triangulo->pega_vertice_b()->pega_x(),
                triangulo->pega_vertice_b()->pega_y(),
                triangulo->pega_vertice_b()->pega_z()
            );

            glVertex3d(
                triangulo->pega_vertice_c()->pega_x(),
                triangulo->pega_vertice_c()->pega_y(),
                triangulo->pega_vertice_c()->pega_z()
            );
        }
        else
        {
            glVertex3d(
                triangulo->pega_vertice_c()->pega_x(),
                triangulo->pega_vertice_c()->pega_y(),
                triangulo->pega_vertice_c()->pega_z()
            );

            glVertex3d(
                triangulo->pega_vertice_b()->pega_x(),
                triangulo->pega_vertice_b()->pega_y(),
                triangulo->pega_vertice_b()->pega_z()
            );

            glVertex3d(
                triangulo->pega_vertice_a()->pega_x(),
                triangulo->pega_vertice_a()->pega_y(),
                triangulo->pega_vertice_a()->pega_z()
            );
        }
    }
    glEnd();
    //desenha_normal(triangulo, normal_dir);
}

void Desenha::desenha_normal(Triangulo *triangulo, int dir)
{
    double nX = triangulo->pega_x() + triangulo->pega_vetor_normal()->pega_x()*0.5;
    double nY = triangulo->pega_y() + triangulo->pega_vetor_normal()->pega_y()*0.5;
    double nZ = triangulo->pega_z() + triangulo->pega_vetor_normal()->pega_z()*0.5;

    switch (dir)
    {
        case 0: // inv X
            nX = nX = triangulo->pega_x() + triangulo->pega_vetor_normal()->pega_x()*-0.5;
            break;
        case 1: // inv Y
            nY = triangulo->pega_y() + triangulo->pega_vetor_normal()->pega_y()*-0.5;;
            break;
        case 2: // inv Z
            nZ = triangulo->pega_z() + triangulo->pega_vetor_normal()->pega_z()*-0.5;
            break;
        default:// do nothing
            break;
    }

    /// DESENHA NORMAL
    glBegin(GL_LINES);
    {
        glVertex3f(triangulo->pega_x(), triangulo->pega_y(), triangulo->pega_z());

        glVertex3f(nX, nY, nZ);
    }
    glEnd();
}

/** Define a base do tabuleiro
  */
void Desenha::desenha_tabuleiro(Tabuleiro *tabuleiro)
{
    //desenha_plano(tabuleiro->pega_base());
    desenha_plano(tabuleiro->pega_parede_norte(), false, 3);
    desenha_plano(tabuleiro->pega_parede_oeste(), false, 3);
    desenha_plano(tabuleiro->pega_parede_leste(), true, 0);
    //desenha_plano(tabuleiro->pega_parede_sul());
}

void Desenha::desenha_esfera(Esfera *esfera)
{
    glPushMatrix();
    {
        glPushMatrix();
        {
            glTranslated(esfera->pega_posicao()->pega_x(),
                         esfera->pega_posicao()->pega_y(),
                         esfera->pega_posicao()->pega_z());
            glutSolidSphere(esfera->pega_raio(), esfera->pega_slices(), esfera->pega_stacks());
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void Desenha::desenha_plano(Plano *plano, bool inv, int normal_dir)
{
    desenha_triangulo(plano->pega_triangulo_base(), inv, normal_dir);
    desenha_triangulo(plano->pega_triangulo_topo(), inv, normal_dir);
}

void Desenha::desenha_bloco(Bloco *bloco)
{
    desenha_plano(bloco->pega_parede_leste(), false, 0);
    desenha_plano(bloco->pega_parede_norte(), false, 3);
    desenha_plano(bloco->pega_parede_oeste(), true, 3);
    desenha_plano(bloco->pega_parede_sul(), true, 3);
    desenha_plano(bloco->pega_parede_frente(), false, 3);
    desenha_plano(bloco->pega_parede_traseira(), true, 1);
}

void Desenha::desenha_seta_direcao(Esfera *esfera, double angulo_disparo)
{
    glPushMatrix();
    {
        glTranslatef(esfera->pega_posicao()->pega_x(),
                     esfera->pega_posicao()->pega_y(),
                     esfera->pega_posicao()->pega_z()
        );

        glPushMatrix();
        {
            glRotatef(0, 0.0, 0.0, 0.0); // rotação
            glutWireSphere(0.0, 30, 30);
        }
        glRotatef((GLfloat) (angulo_disparo + 90), 0.0, 0.0, 1.0); // rotação
        glTranslatef(0.0, esfera->pega_raio(), 0.0);
        glPushMatrix();
        {
            glRotatef(270, 1.0, 0.0, 0.0); // rotação
            glutSolidCone(esfera->pega_raio(),
                          esfera->pega_raio(),
                          esfera->pega_slices(),
                          esfera->pega_stacks()
            );
        }
        glPopMatrix();
    }
    glPopMatrix(); // origem volta para o sistema de coordenadas original
}

void Desenha::desenha_vetor_direcao_esfera(Esfera *esfera)
{
    Vertice proxPos((esfera->pega_posicao()->pega_x() + esfera->pega_direcao()->pega_x()*(5/3)),
                    (esfera->pega_posicao()->pega_y() + esfera->pega_direcao()->pega_y()*(5/3)),
                    (esfera->pega_posicao()->pega_z()));

    Vertice posAtual(esfera->pega_posicao()->pega_x(), esfera->pega_posicao()->pega_y(), esfera->pega_posicao()->pega_z());

    glBegin(GL_LINES);
    {
        glVertex3f((GLfloat) posAtual.pega_x(), (GLfloat) posAtual.pega_y(), (GLfloat) posAtual.pega_z());
        glVertex3f((GLfloat) proxPos.pega_x(), (GLfloat) proxPos.pega_y(), (GLfloat) proxPos.pega_z());
    }
    glEnd();
}

void Desenha::desenha_matriz_blocos(Bloco ***matriz, int blocos_Col, int blocos_lin)
{
    // percorre a matriz de blocos
    for (int i = 0; i < blocos_Col; i++)
    {
        for (int j = 0; j < blocos_lin; j++)
        {
            // realiza a alternancia de cor entre os blocos
            // ((i+j)%2 == 0) ? setColor(0.91, 0.76, 0.65) : setColor(0.65, 0.50, 0.39);

            // desenha o bloco
            if (!matriz[i][j]->pega_destruido())
            {
                desenha_bloco(matriz[i][j]);
            }
        }
    }
}

void Desenha::desenha_skybox(Bloco *skybox)
{
//    // Store the current matrix
//    glPushMatrix();
//
//    // Reset and transform the matrix.
//    glLoadIdentity();
//    gluLookAt(
//        0,0,0,
//        camera->x(),camera->y(),camera->z(),
//        0,1,0);
//
//    // Enable/Disable features
//    glPushAttrib(GL_ENABLE_BIT);
//    glEnable(GL_TEXTURE_2D);
//    glDisable(GL_DEPTH_TEST);
//    glDisable(GL_LIGHTING);
//    glDisable(GL_BLEND);
//
//    // Just in case we set all vertices to white.
//    glColor4f(1,1,1,1);
//
//    // Render the front quad
//    glBindTexture(GL_TEXTURE_2D, _skybox[0]);
//    glBegin(GL_QUADS);
//    {
//        glTexCoord2f(0, 0);
//        glVertex3f(0.5f, -0.5f, -0.5f);
//        glTexCoord2f(1, 0);
//        glVertex3f(-0.5f, -0.5f, -0.5f);
//        glTexCoord2f(1, 1);
//        glVertex3f(-0.5f, 0.5f, -0.5f);
//        glTexCoord2f(0, 1);
//        glVertex3f(0.5f, 0.5f, -0.5f);
//    }glEnd();
//
//    // Render the left quad
//    glBindTexture(GL_TEXTURE_2D, _skybox[1]);
//    glBegin(GL_QUADS);
//    {
//        glTexCoord2f(0, 0);
//        glVertex3f(0.5f, -0.5f, 0.5f);
//        glTexCoord2f(1, 0);
//        glVertex3f(0.5f, -0.5f, -0.5f);
//        glTexCoord2f(1, 1);
//        glVertex3f(0.5f, 0.5f, -0.5f);
//        glTexCoord2f(0, 1);
//        glVertex3f(0.5f, 0.5f, 0.5f);
//    }
//    glEnd();
//
//    // Render the back quad
//    glBindTexture(GL_TEXTURE_2D, _skybox[2]);
//    glBegin(GL_QUADS);
//    {
//        glTexCoord2f(0, 0);
//        glVertex3f(-0.5f, -0.5f, 0.5f);
//        glTexCoord2f(1, 0);
//        glVertex3f(0.5f, -0.5f, 0.5f);
//        glTexCoord2f(1, 1);
//        glVertex3f(0.5f, 0.5f, 0.5f);
//        glTexCoord2f(0, 1);
//        glVertex3f(-0.5f, 0.5f, 0.5f);
//    }
//    glEnd();
//
//    // Render the right quad
//    glBindTexture(GL_TEXTURE_2D, _skybox[3]);
//    glBegin(GL_QUADS);
//    {
//        glTexCoord2f(0, 0);
//        glVertex3f(-0.5f, -0.5f, -0.5f);
//        glTexCoord2f(1, 0);
//        glVertex3f(-0.5f, -0.5f, 0.5f);
//        glTexCoord2f(1, 1);
//        glVertex3f(-0.5f, 0.5f, 0.5f);
//        glTexCoord2f(0, 1);
//        glVertex3f(-0.5f, 0.5f, -0.5f);
//    }
//    glEnd();
//
//    // Render the top quad
//    glBindTexture(GL_TEXTURE_2D, _skybox[4]);
//    glBegin(GL_QUADS);
//    {
//        glTexCoord2f(0, 1);
//        glVertex3f(-0.5f, 0.5f, -0.5f);
//        glTexCoord2f(0, 0);
//        glVertex3f(-0.5f, 0.5f, 0.5f);
//        glTexCoord2f(1, 0);
//        glVertex3f(0.5f, 0.5f, 0.5f);
//        glTexCoord2f(1, 1);
//        glVertex3f(0.5f, 0.5f, -0.5f);
//    }
//    glEnd();
//
//    // Render the bottom quad
//    glBindTexture(GL_TEXTURE_2D, _skybox[5]);
//    glBegin(GL_QUADS);
//    {
//        glTexCoord2f(0, 0);
//        glVertex3f(-0.5f, -0.5f, -0.5f);
//        glTexCoord2f(0, 1);
//        glVertex3f(-0.5f, -0.5f, 0.5f);
//        glTexCoord2f(1, 1);
//        glVertex3f(0.5f, -0.5f, 0.5f);
//        glTexCoord2f(1, 0);
//        glVertex3f(0.5f, -0.5f, -0.5f);
//    }
//    glEnd();
//
//    // Restore enable bits and matrix
//    glPopAttrib();
//    glPopMatrix();
}