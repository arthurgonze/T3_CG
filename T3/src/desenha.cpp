#include <include/glcWavefrontObject.h>
#include <include/vertice.h>
#include <include/esfera.h>
#include <string>
#include <include/glcText.h>
#include "desenha.h"

void desenha_objetos_importados();
void desenha_vidas();
void desenha_texto_nivel();
void desenha_skybox();
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
        if (!inv)
        {
            glNormal3f(
                (GLfloat) triangulo->pega_vetor_normal()->pega_x(),
                (GLfloat) triangulo->pega_vetor_normal()->pega_y(),
                (GLfloat) triangulo->pega_vetor_normal()->pega_z()
            );
            glVertex3d(
                triangulo->pega_vertice_a()->pega_x(),
                triangulo->pega_vertice_a()->pega_y(),
                triangulo->pega_vertice_a()->pega_z()
            );

            glNormal3f(
                (GLfloat) triangulo->pega_vetor_normal()->pega_x(),
                (GLfloat) triangulo->pega_vetor_normal()->pega_y(),
                (GLfloat) triangulo->pega_vetor_normal()->pega_z()
            );
            glVertex3d(
                triangulo->pega_vertice_b()->pega_x(),
                triangulo->pega_vertice_b()->pega_y(),
                triangulo->pega_vertice_b()->pega_z()
            );

            glNormal3f(
                (GLfloat) triangulo->pega_vetor_normal()->pega_x(),
                (GLfloat) triangulo->pega_vetor_normal()->pega_y(),
                (GLfloat) triangulo->pega_vetor_normal()->pega_z()
            );
            glVertex3d(
                triangulo->pega_vertice_c()->pega_x(),
                triangulo->pega_vertice_c()->pega_y(),
                triangulo->pega_vertice_c()->pega_z()
            );
        }
        else
        {
            glNormal3f(
                (GLfloat) triangulo->pega_vetor_normal()->pega_x(),
                (GLfloat) triangulo->pega_vetor_normal()->pega_y(),
                (GLfloat) triangulo->pega_vetor_normal()->pega_z()
            );
            glVertex3d(
                triangulo->pega_vertice_c()->pega_x(),
                triangulo->pega_vertice_c()->pega_y(),
                triangulo->pega_vertice_c()->pega_z()
            );

            glNormal3f(
                (GLfloat) triangulo->pega_vetor_normal()->pega_x(),
                (GLfloat) triangulo->pega_vetor_normal()->pega_y(),
                (GLfloat) triangulo->pega_vetor_normal()->pega_z()
            );
            glVertex3d(
                triangulo->pega_vertice_b()->pega_x(),
                triangulo->pega_vertice_b()->pega_y(),
                triangulo->pega_vertice_b()->pega_z()
            );

            glNormal3f(
                (GLfloat) triangulo->pega_vetor_normal()->pega_x(),
                (GLfloat) triangulo->pega_vetor_normal()->pega_y(),
                (GLfloat) triangulo->pega_vetor_normal()->pega_z()
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
    //desenha_plano(tabuleiro->pega_parede_oeste(), false, 3);
    //desenha_plano(tabuleiro->pega_parede_leste(), true, 0);
    //desenha_plano(tabuleiro->pega_parede_sul());
    desenha_parede_leste_tabuleiro(tabuleiro);
    desenha_parede_oeste_tabuleiro(tabuleiro);
}
void Desenha::desenha_parede_oeste_tabuleiro(Tabuleiro *tabuleiro) const
{
//    Vertice *normal;
//    Aux *aux = new Aux();
    glBegin(GL_TRIANGLE_FAN);
    {
        for (int i = 0; i < tabuleiro->pega_Num_segmentos(); i++)
        {
//            normal = aux->calcula_normal(&tabuleiro->pega_Vertices_elipse_oeste()[i],
//                                         &tabuleiro->pega_Vertices_elipse_oeste()[i + 1],
//                                         &tabuleiro->pega_Vertices_elipse_oeste()[i + 2]);
            //apply radius and offset
//            glNormal3f((GLfloat) normal->pega_x(), (GLfloat) normal->pega_y(), (GLfloat) normal->pega_z());
            glVertex3f(tabuleiro->pega_Vertices_elipse_oeste()[i].pega_x(), tabuleiro->pega_Vertices_elipse_oeste()[i].pega_y(),
                       0.01);//output vertex
        }
        for (int i = 0; i < tabuleiro->pega_Num_segmentos(); i++)
        {
//            normal = aux->calcula_normal(&tabuleiro->pega_Vertices_elipse_oeste()[i],
//                                         &tabuleiro->pega_Vertices_elipse_oeste()[i + 1],
//                                         &tabuleiro->pega_Vertices_elipse_oeste()[i + 2]);
            //apply radius and offset
//            glNormal3f((GLfloat) normal->pega_x(), (GLfloat) normal->pega_y(), (GLfloat) normal->pega_z());
            //apply radius and offset
            glVertex3f(tabuleiro->pega_Vertices_elipse_oeste()[i].pega_x(), tabuleiro->pega_Vertices_elipse_oeste()[i].pega_y(),
                       tabuleiro->pega_tam_altura_paredes() + 0.01);//output vertex
        }
    }
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
    {
        for (int i = 0; i < tabuleiro->pega_Num_segmentos() - 1; i++)
        {
            //apply radius and offset
//            Vertice *vertice_aux = &tabuleiro->pega_Vertices_elipse_oeste()[i];
//            vertice_aux->define_z(tabuleiro->pega_tam_altura_paredes() + 0.01);
//            normal = aux->calcula_normal(&tabuleiro->pega_Vertices_elipse_oeste()[i],
//                                         vertice_aux,
//                                         &tabuleiro->pega_Vertices_elipse_oeste()[i + 1]);
            //apply radius and offset
//            glNormal3f((GLfloat) normal->pega_x(), (GLfloat) normal->pega_y(), (GLfloat) normal->pega_z());
            glVertex3f(tabuleiro->pega_Vertices_elipse_oeste()[i].pega_x(), tabuleiro->pega_Vertices_elipse_oeste()[i].pega_y(),
                       0.01);//output vertex

//            glNormal3f((GLfloat) normal->pega_x(), (GLfloat) normal->pega_y(), (GLfloat) normal->pega_z());
            glVertex3f(tabuleiro->pega_Vertices_elipse_oeste()[i].pega_x(), tabuleiro->pega_Vertices_elipse_oeste()[i].pega_y(),
                       tabuleiro->pega_tam_altura_paredes() + 0.01);//output vertex

//            glNormal3f((GLfloat) normal->pega_x(), (GLfloat) normal->pega_y(), (GLfloat) normal->pega_z());
            glVertex3f(tabuleiro->pega_Vertices_elipse_oeste()[i + 1].pega_x(), tabuleiro->pega_Vertices_elipse_oeste()[i + 1].pega_y(),
                       0.01);//output vertex
        }
        for (int i = 0; i < tabuleiro->pega_Num_segmentos() - 1; i++)
        {
//            Vertice *vertice_aux = &tabuleiro->pega_Vertices_elipse_oeste()[i+1];
//            vertice_aux->define_z(tabuleiro->pega_tam_altura_paredes() + 0.01);
//            normal = aux->calcula_normal(&tabuleiro->pega_Vertices_elipse_oeste()[i],
//                                         vertice_aux,
//                                         &tabuleiro->pega_Vertices_elipse_oeste()[i + 1]);
//            //apply radius and offset
//            glNormal3f((GLfloat) normal->pega_x(), (GLfloat) normal->pega_y(), (GLfloat) normal->pega_z());
            glVertex3f(tabuleiro->pega_Vertices_elipse_oeste()[i].pega_x(), tabuleiro->pega_Vertices_elipse_oeste()[i].pega_y(),
                       tabuleiro->pega_tam_altura_paredes() + 0.01);//output vertex

//            glNormal3f((GLfloat) normal->pega_x(), (GLfloat) normal->pega_y(), (GLfloat) normal->pega_z());
            glVertex3f(tabuleiro->pega_Vertices_elipse_oeste()[i + 1].pega_x(), tabuleiro->pega_Vertices_elipse_oeste()[i].pega_y(),
                       tabuleiro->pega_tam_altura_paredes() + 0.01);//output vertex

//            glNormal3f((GLfloat) normal->pega_x(), (GLfloat) normal->pega_y(), (GLfloat) normal->pega_z());
            glVertex3f(tabuleiro->pega_Vertices_elipse_oeste()[i + 1].pega_x(), tabuleiro->pega_Vertices_elipse_oeste()[i + 1].pega_y(),
                       0.01);//output vertex
        }
    }
    glEnd();
}
void Desenha::desenha_parede_leste_tabuleiro(Tabuleiro *tabuleiro) const
{
    glBegin(GL_TRIANGLE_FAN);
    {
        for (int i = 0; i < tabuleiro->pega_Num_segmentos(); i++)
        {
//            glNormal3f(0, 0, -1);
            //apply radius and offset
            glVertex3f(tabuleiro->pega_Vertices_elipse_leste()[i].pega_x(), tabuleiro->pega_Vertices_elipse_leste()[i].pega_y(),
                       0.01);//output vertex
        }
        for (int i = 0; i < tabuleiro->pega_Num_segmentos(); i++)
        {
//            glNormal3f(0, 0, 1);
            //apply radius and offset
            glVertex3f(tabuleiro->pega_Vertices_elipse_leste()[i].pega_x(), tabuleiro->pega_Vertices_elipse_leste()[i].pega_y(),
                       tabuleiro->pega_tam_altura_paredes() + 0.01);//output vertex
        }
    }
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
    {
        for (int i = 0; i < tabuleiro->pega_Num_segmentos() - 1; i++)
        {
//            glNormal3f(-1, 0, 0);
            //apply radius and offset
            glVertex3f(tabuleiro->pega_Vertices_elipse_leste()[i].pega_x(), tabuleiro->pega_Vertices_elipse_leste()[i].pega_y(),
                       0.01);//output vertex

//            glNormal3f(-1, 0, 0);
            glVertex3f(tabuleiro->pega_Vertices_elipse_leste()[i].pega_x(), tabuleiro->pega_Vertices_elipse_leste()[i].pega_y(),
                       tabuleiro->pega_tam_altura_paredes() + 0.01);//output vertex

//            glNormal3f(-1, 0, 0);
            glVertex3f(tabuleiro->pega_Vertices_elipse_leste()[i + 1].pega_x(), tabuleiro->pega_Vertices_elipse_leste()[i + 1].pega_y(),
                       0.01);//output vertex
        }
        for (int i = 0; i < tabuleiro->pega_Num_segmentos() - 1; i++)
        {
//            glNormal3f(-1, 0, 0);
            //apply radius and offset
            glVertex3f(tabuleiro->pega_Vertices_elipse_leste()[i].pega_x(), tabuleiro->pega_Vertices_elipse_leste()[i].pega_y(),
                       tabuleiro->pega_tam_altura_paredes() + 0.01);//output vertex

//            glNormal3f(-1, 0, 0);
            glVertex3f(tabuleiro->pega_Vertices_elipse_leste()[i + 1].pega_x(), tabuleiro->pega_Vertices_elipse_leste()[i].pega_y(),
                       tabuleiro->pega_tam_altura_paredes() + 0.01);//output vertex

//            glNormal3f(-1, 0, 0);
            glVertex3f(tabuleiro->pega_Vertices_elipse_leste()[i + 1].pega_x(), tabuleiro->pega_Vertices_elipse_leste()[i + 1].pega_y(),
                       0.01);//output vertex
        }
    }
    glEnd();
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

void Desenha::desenha_objetos_importados(glcWavefrontObject *gerenciador_de_objetos, Esfera *geracao_esfera_1, Esfera *geracao_esfera_2)
{
    glPushMatrix();
    {
        glTranslatef(geracao_esfera_1->pega_posicao()->pega_x(), geracao_esfera_1->pega_posicao()->pega_y(),
                     geracao_esfera_1->pega_posicao()->pega_z());

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
        glTranslatef(geracao_esfera_2->pega_posicao()->pega_x(), geracao_esfera_2->pega_posicao()->pega_y(),
                     geracao_esfera_2->pega_posicao()->pega_z());

        gerenciador_de_objetos->SelectObject(1);
        gerenciador_de_objetos->SetShadingMode(SMOOTH_SHADING); // Possible values: FLAT_SHADING e SMOOTH_SHADING
        gerenciador_de_objetos->SetRenderMode(USE_MATERIAL); // Possible values: USE_COLOR, USE_MATERIAL
        gerenciador_de_objetos->Unitize();
        gerenciador_de_objetos->Scale(0.3);
        gerenciador_de_objetos->Draw();
    }
    glPopMatrix();
}

void Desenha::desenha_vidas(GameController *controlador_de_jogo, Vertice *ponto_inicial_vidas)
{
    for (int ne = 0; ne < controlador_de_jogo->pega_num_vidas(); ne++)
    {
        Vertice *pv = new Vertice(ponto_inicial_vidas->pega_x() - ((double) ne)/10, ponto_inicial_vidas->pega_y(),
                                  ponto_inicial_vidas->pega_z());

        Esfera *ev = new Esfera(pv, 0.03);
        desenha_esfera(ev);
    }
}

void Desenha::desenha_texto_nivel(GameController *controlador_de_jogo)
{
    string str = "Fase ";
    str += to_string(controlador_de_jogo->pega_fase());
    char faseAtual[str.size() + 1];
    str.copy(faseAtual, str.size() + 1);
    faseAtual[str.size()] = '\0';
    glcText *text2 = new glcText();
    if (controlador_de_jogo->pega_fase()==0)
    {
        if (controlador_de_jogo->pega_jogo_vencido())
        {
            text2->setString("Voce venceu!");
        }
        else if (controlador_de_jogo->pega_fase()==0 && controlador_de_jogo->pega_num_vidas() <= 0)
        {
            text2->setString("Voce perdeu!");
        }
    }
    else
    {
        text2->setString(faseAtual);
    }
    text2->setColor(1.0, 1.0, 1.0);
    text2->setPos(0, 3.15);
    text2->setType(0);
    text2->render();
}

void Desenha::desenha_skybox(glcTexture *textureManager, Camera *camera, Skybox *skybox, bool rotacao_em_conjunto)
{
    textureManager->Bind(0); // textura 0 = skybox
    glDisable(GL_DEPTH_TEST);
    glPushMatrix();
    {
        textureManager->SetWrappingMode(GL_CLAMP);
        textureManager->SetMinFilterMode(GL_LINEAR);
        textureManager->SetMagFilterMode(GL_LINEAR);
        textureManager->SetColorMode(GL_MODULATE);

        if (rotacao_em_conjunto)
        {
            glRotatef(camera->pega_yaw(), 0.0, 1.0, 0.0);
            glRotatef(camera->pega_pitch(), 1.0, 0.0, 0.0);
        }

        // FRENTE
        glBegin(GL_QUADS);
        {
            glNormal3f(0.0, 0.0, 1.0);

            glTexCoord2f(0.25, 0.35);
            glVertex3d(skybox->pega_v5().pega_x(),
                       skybox->pega_v5().pega_y(),
                       skybox->pega_v5().pega_z());//v1

            glTexCoord2f(0.5, 0.35);
            glVertex3d(skybox->pega_v6().pega_x(),
                       skybox->pega_v6().pega_y(),
                       skybox->pega_v6().pega_z());//v2

            glTexCoord2f(0.5, 0.65);
            glVertex3d(skybox->pega_v7().pega_x(),
                       skybox->pega_v7().pega_y(),
                       skybox->pega_v7().pega_z());//v3

            glTexCoord2f(0.25, 0.65);
            glVertex3d(skybox->pega_v8().pega_x(),
                       skybox->pega_v8().pega_y(),
                       skybox->pega_v8().pega_z());//v4
        }
        glEnd();

        // TRASEIRA
        glBegin(GL_QUADS);
        {
            glNormal3f(0.0, 0.0, 1.0);

            glTexCoord2f(0.75, 0.35);
            glVertex3d(skybox->pega_v2().pega_x(),
                       skybox->pega_v2().pega_y(),
                       skybox->pega_v2().pega_z());//v1

            glTexCoord2f(1, 0.35);
            glVertex3d(skybox->pega_v1().pega_x(),
                       skybox->pega_v1().pega_y(),
                       skybox->pega_v1().pega_z());//v2

            glTexCoord2f(1, 0.65);
            glVertex3d(skybox->pega_v4().pega_x(),
                       skybox->pega_v4().pega_y(),
                       skybox->pega_v4().pega_z());//v3

            glTexCoord2f(0.75, 0.65);
            glVertex3d(skybox->pega_v3().pega_x(),
                       skybox->pega_v3().pega_y(),
                       skybox->pega_v3().pega_z());//v4
        }
        glEnd();

        // TOPO
        glBegin(GL_QUADS);
        {
            glNormal3f(0.0, 0.0, 1.0);

            glTexCoord2f(0.25, 0.65);
            glVertex3d(skybox->pega_v8().pega_x(),
                       skybox->pega_v8().pega_y(),
                       skybox->pega_v8().pega_z());//v4

            glTexCoord2f(0.5, 0.65);
            glVertex3d(skybox->pega_v7().pega_x(),
                       skybox->pega_v7().pega_y(),
                       skybox->pega_v7().pega_z());//v3

            glTexCoord2f(0.5, 1);
            glVertex3d(skybox->pega_v3().pega_x(),
                       skybox->pega_v3().pega_y(),
                       skybox->pega_v3().pega_z());//v2

            glTexCoord2f(0.25, 1);
            glVertex3d(skybox->pega_v4().pega_x(),
                       skybox->pega_v4().pega_y(),
                       skybox->pega_v4().pega_z());//v1

        }
        glEnd();

        // CHAO
        glBegin(GL_QUADS);
        {
            glNormal3f(0.0, 0.0, 1.0);

            glTexCoord2f(0.25, 0);
            glVertex3f(skybox->pega_v1().pega_x(),
                       skybox->pega_v1().pega_y(),
                       skybox->pega_v1().pega_z());//v1

            glTexCoord2f(0.5, 0);
            glVertex3f(skybox->pega_v2().pega_x(),
                       skybox->pega_v2().pega_y(),
                       skybox->pega_v2().pega_z());//v2

            glTexCoord2f(0.5, 0.35);
            glVertex3f(skybox->pega_v6().pega_x(),
                       skybox->pega_v6().pega_y(),
                       skybox->pega_v6().pega_z());//v3

            glTexCoord2f(0.25, 0.35);
            glVertex3f(skybox->pega_v5().pega_x(),
                       skybox->pega_v5().pega_y(),
                       skybox->pega_v5().pega_z());//v4
        }
        glEnd();

        // OESTE
        glBegin(GL_QUADS);
        {
            glNormal3f(0.0, 0.0, 1.0);

            glTexCoord2f(0.0, 0.35);
            glVertex3f(skybox->pega_v1().pega_x(),
                       skybox->pega_v1().pega_y(),
                       skybox->pega_v1().pega_z());//v1

            glTexCoord2f(0.25, 0.35);
            glVertex3f(skybox->pega_v5().pega_x(),
                       skybox->pega_v5().pega_y(),
                       skybox->pega_v5().pega_z());//v2

            glTexCoord2f(0.25, 0.65);
            glVertex3f(skybox->pega_v8().pega_x(),
                       skybox->pega_v8().pega_y(),
                       skybox->pega_v8().pega_z());//v3

            glTexCoord2f(0, 0.65);
            glVertex3f(skybox->pega_v4().pega_x(),
                       skybox->pega_v4().pega_y(),
                       skybox->pega_v4().pega_z());//v4
        }
        glEnd();

        // LESTE
        glBegin(GL_QUADS);
        {
            glNormal3f(0.0, 0.0, 1.0);

            glTexCoord2f(0.5, 0.35);
            glVertex3f(skybox->pega_v6().pega_x(),
                       skybox->pega_v6().pega_y(),
                       skybox->pega_v6().pega_z());//v1

            glTexCoord2f(0.75, 0.35);
            glVertex3f(skybox->pega_v2().pega_x(),
                       skybox->pega_v2().pega_y(),
                       skybox->pega_v2().pega_z());//v2

            glTexCoord2f(0.75, 0.65);
            glVertex3f(skybox->pega_v3().pega_x(),
                       skybox->pega_v3().pega_y(),
                       skybox->pega_v3().pega_z());//v3

            glTexCoord2f(0.5, 0.65);
            glVertex3f(skybox->pega_v7().pega_x(),
                       skybox->pega_v7().pega_y(),
                       skybox->pega_v7().pega_z());//v4
        }
        glEnd();
    }
    glPopMatrix();
    // Desabilita o uso de texturas
    textureManager->Disable();
    glEnable(GL_DEPTH_TEST);
}

void Desenha::desenha_rebatedor(Pad *rebatedor)
{
    desenha_bloco(rebatedor->pega_pad());

    glBegin(GL_TRIANGLE_FAN);
    {
        for (int i = 0; i < rebatedor->pega_Num_segmentos(); i++)
        {
            //apply radius and offset
            glNormal3f(0, 0, -1);
            glVertex3f(rebatedor->pega_Vertices_elipse()[i].pega_x(), rebatedor->pega_Vertices_elipse()[i].pega_y(),
                       0.01);//output vertex
        }
        for (int i = 0; i < rebatedor->pega_Num_segmentos(); i++)
        {
            glNormal3f(0, 0, 1);
            //apply radius and offset
            glVertex3f(rebatedor->pega_Vertices_elipse()[i].pega_x(), rebatedor->pega_Vertices_elipse()[i].pega_y(),
                       rebatedor->pega_Valor_tamanho_profundidade() + 0.01);//output vertex
        }
    }
    glEnd();

    glBegin(GL_TRIANGLE_STRIP);
    {
        for (int i = 0; i < rebatedor->pega_Num_segmentos() - 1; i++)
        {
            glNormal3f(0, 1, 0);
            //apply radius and offset
            glVertex3f(rebatedor->pega_Vertices_elipse()[i].pega_x(), rebatedor->pega_Vertices_elipse()[i].pega_y(),
                       0.01);//output vertex

            glNormal3f(0, 1, 0);
            glVertex3f(rebatedor->pega_Vertices_elipse()[i].pega_x(), rebatedor->pega_Vertices_elipse()[i].pega_y(),
                       rebatedor->pega_Valor_tamanho_profundidade() + 0.01);//output vertex

            glNormal3f(0, 1, 0);
            glVertex3f(rebatedor->pega_Vertices_elipse()[i + 1].pega_x(), rebatedor->pega_Vertices_elipse()[i + 1].pega_y(),
                       0.01);//output vertex
        }
        for (int i = 0; i < rebatedor->pega_Num_segmentos() - 1; i++)
        {
            glNormal3f(0, 1, 0);
            //apply radius and offset
            glVertex3f(rebatedor->pega_Vertices_elipse()[i].pega_x(), rebatedor->pega_Vertices_elipse()[i].pega_y(),
                       rebatedor->pega_Valor_tamanho_profundidade() + 0.01);//output vertex

            glNormal3f(0, 1, 0);
            glVertex3f(rebatedor->pega_Vertices_elipse()[i + 1].pega_x(), rebatedor->pega_Vertices_elipse()[i].pega_y(),
                       rebatedor->pega_Valor_tamanho_profundidade() + 0.01);//output vertex

            glNormal3f(0, 1, 0);
            glVertex3f(rebatedor->pega_Vertices_elipse()[i + 1].pega_x(), rebatedor->pega_Vertices_elipse()[i + 1].pega_y(),
                       0.01);//output vertex
        }
    }
    glEnd();

}
