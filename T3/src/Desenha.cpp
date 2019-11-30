#include "Desenha.h"

Desenha::Desenha()
{
    xMaxBarra = 0;
    xMinBarra = 0;
}

Desenha::~Desenha()
{
    //dtor
}

/** Desenha um triangulo seguindo os vértices passados por parâmetro.
  * Será utilizada para criar as faces triangulares do prisma (desenhaPrisma),
  * da base do tabuleiro (desenhaTabuleiro) e suas paredes (desenhaParedes)
  */
void Desenha::desenhaTriangulo(Triangulo *triangulo, bool inv, int normalDir)
{
    glBegin(GL_TRIANGLES);
    {
        // glNormal3f((GLfloat)vetorNormal.x, (GLfloat)vetorNormal.y, (GLfloat)vetorNormal.z);
        glNormal3f(
            (GLfloat) triangulo->getVetorNormal()->getX(),
            (GLfloat) triangulo->getVetorNormal()->getY(),
            (GLfloat) triangulo->getVetorNormal()->getZ()
        );
        if (!inv)
        {
            glVertex3d(
                triangulo->getVerticeA()->getX(),
                triangulo->getVerticeA()->getY(),
                triangulo->getVerticeA()->getZ()
            );

            glVertex3d(
                triangulo->getVerticeB()->getX(),
                triangulo->getVerticeB()->getY(),
                triangulo->getVerticeB()->getZ()
            );

            glVertex3d(
                triangulo->getVerticeC()->getX(),
                triangulo->getVerticeC()->getY(),
                triangulo->getVerticeC()->getZ()
            );
        }
        else
        {
            glVertex3d(
                triangulo->getVerticeC()->getX(),
                triangulo->getVerticeC()->getY(),
                triangulo->getVerticeC()->getZ()
            );

            glVertex3d(
                triangulo->getVerticeB()->getX(),
                triangulo->getVerticeB()->getY(),
                triangulo->getVerticeB()->getZ()
            );

            glVertex3d(
                triangulo->getVerticeA()->getX(),
                triangulo->getVerticeA()->getY(),
                triangulo->getVerticeA()->getZ()
            );
        }
    }
    glEnd();
    //desenhaNormal(triangulo, normalDir);
}

void Desenha::desenhaNormal(Triangulo *triangulo, int dir)
{
    double nX = triangulo->getX() + triangulo->getVetorNormal()->getX()*0.5;
    double nY = triangulo->getY() + triangulo->getVetorNormal()->getY()*0.5;
    double nZ = triangulo->getZ() + triangulo->getVetorNormal()->getZ()*0.5;

    switch (dir)
    {
        case 0: // inv X
            nX = nX = triangulo->getX() + triangulo->getVetorNormal()->getX()*-0.5;
            break;
        case 1: // inv Y
            nY = triangulo->getY() + triangulo->getVetorNormal()->getY()*-0.5;;
            break;
        case 2: // inv Z
            nZ = triangulo->getZ() + triangulo->getVetorNormal()->getZ()*-0.5;
            break;
        default:// do nothing
            break;
    }

    /// DESENHA NORMAL
    glBegin(GL_LINES);
    {
        glVertex3f(triangulo->getX(), triangulo->getY(), triangulo->getZ());

        glVertex3f(nX, nY, nZ);
    }
    glEnd();
}

/** Define a base do tabuleiro
  */
void Desenha::desenhaTabuleiro(Tabuleiro *tabuleiro)
{
    //desenhaPlano(tabuleiro->getBase());
    desenhaPlano(tabuleiro->getParedeNorte(), false, 3);
    desenhaPlano(tabuleiro->getParedeOeste(), false, 3);
    desenhaPlano(tabuleiro->getParedeLeste(), true, 0);
    //desenhaPlano(tabuleiro->getParedeSul());
}

void Desenha::desenhaEsfera(Esfera *esfera)
{
    glPushMatrix();
    {
        glPushMatrix();
        {
            glTranslated(esfera->getPosicao()->getX(),
                         esfera->getPosicao()->getY(),
                         esfera->getPosicao()->getZ());
            glutSolidSphere(esfera->getRaio(), esfera->getSlices(), esfera->getStacks());
        }
        glPopMatrix();
    }
    glPopMatrix();
}

void Desenha::desenhaPlano(Plano *plano, bool inv, int normalDir)
{
    desenhaTriangulo(plano->getTrianguloBase(), inv, normalDir);
    desenhaTriangulo(plano->getTrianguloTopo(), inv, normalDir);
}

void Desenha::desenhaBloco(Bloco *bloco)
{
    desenhaPlano(bloco->getParedeLeste(), false, 0);
    desenhaPlano(bloco->getParedeNorte(), false, 3);
    desenhaPlano(bloco->getParedeOeste(), true, 3);
    desenhaPlano(bloco->getParedeSul(), true, 3);
    desenhaPlano(bloco->getParedeFrente(), false, 3);
    desenhaPlano(bloco->getParedeTraseira(), true, 1);
}

void Desenha::desenhaSetaDirecao(Esfera *esfera, double anguloDisparo)
{
    glPushMatrix();
    {
        glTranslatef(esfera->getPosicao()->getX(),
                     esfera->getPosicao()->getY(),
                     esfera->getPosicao()->getZ()
        );

        glPushMatrix();
        {
            glRotatef(0, 0.0, 0.0, 0.0); // rotação
            glutWireSphere(0.0, 30, 30);
        }
        glRotatef((GLfloat) (anguloDisparo + 90), 0.0, 0.0, 1.0); // rotação
        glTranslatef(0.0, esfera->getRaio(), 0.0);
        glPushMatrix();
        {
            glRotatef(270, 1.0, 0.0, 0.0); // rotação
            glutSolidCone(esfera->getRaio(),
                          esfera->getRaio(),
                          esfera->getSlices(),
                          esfera->getStacks()
            );
        }
        glPopMatrix();
    }
    glPopMatrix(); // origem volta para o sistema de coordenadas original
}

void Desenha::desenhaVetorDirecaoEsfera(Esfera *esfera)
{
    Vertice proxPos((esfera->getPosicao()->getX() + esfera->getDirecao()->getX()*(5/3)),
                    (esfera->getPosicao()->getY() + esfera->getDirecao()->getY()*(5/3)),
                    (esfera->getPosicao()->getZ()));

    Vertice posAtual(esfera->getPosicao()->getX(), esfera->getPosicao()->getY(), esfera->getPosicao()->getZ());

    glBegin(GL_LINES);
    {
        glVertex3f((GLfloat) posAtual.getX(), (GLfloat) posAtual.getY(), (GLfloat) posAtual.getZ());
        glVertex3f((GLfloat) proxPos.getX(), (GLfloat) proxPos.getY(), (GLfloat) proxPos.getZ());
    }
    glEnd();
}

void Desenha::desenhaMatrizBlocos(Bloco ***matriz, int blocosCol, int blocosLin)
{
    // percorre a matriz de blocos
    for (int i = 0; i < blocosCol; i++)
    {
        for (int j = 0; j < blocosLin; j++)
        {
            // realiza a alternancia de cor entre os blocos
            // ((i+j)%2 == 0) ? setColor(0.91, 0.76, 0.65) : setColor(0.65, 0.50, 0.39);

            // desenha o bloco
            if (!matriz[i][j]->getDestruido())
            {
                desenhaBloco(matriz[i][j]);
            }
        }
    }
}
