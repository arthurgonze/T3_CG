#include "Aux.h"
#include "math.h"

Aux::Aux()
{

}

Aux::~Aux()
{
    //dtor
}

/// Exemplo de cálculo de vetor normal que são definidos a partir dos vértices do triângulo;
/// v_2
///  ^
///  |\
///  | \
///  |  \       'vn' é o vetor normal resultante
///  |   \
///  +----> v_1
///  v_0
Vertice *Aux::CalculaNormal(Vertice *verticeA, Vertice *verticeB, Vertice *verticeC)
{
    Vertice *v_0 = verticeA;
    Vertice *v_1 = verticeB;
    Vertice *v_2 = verticeC;
    double len;

    // Set Vector U to (Triangle.p2 minus Triangle.p1)
    /// Encontra vetor v1
    Vertice *v1 = new Vertice(
        (v_1->getX() - v_0->getX()),
        (v_1->getY() - v_0->getY()),
        (v_1->getZ() - v_0->getZ())
    );

    // Set Vector V to (Triangle.p3 minus Triangle.p1)
    /// Encontra vetor v2
    Vertice *v2 = new Vertice(
        (v_2->getX() - v_0->getX()),
        (v_2->getY() - v_0->getY()),
        (v_2->getZ() - v_0->getZ())
    );

    /// Calculo do produto vetorial de v1 e v2
    Vertice *vn = new Vertice(
        //	Set Normal.x to (multiply U.y by V.z) minus (multiply U.z by V.y)
        (v1->getY()*v2->getZ()) - (v1->getZ()*v2->getY()),
        //	Set Normal.y to (multiply U.z by V.x) minus (multiply U.x by V.z)
        (v1->getZ()*v2->getX()) - (v1->getX()*v2->getZ()),
        //	Set Normal.z to (multiply U.x by V.y) minus (multiply U.y by V.x)
        (v1->getX()*v2->getY()) - (v1->getY()*v2->getX())
    );

    /// normalizacao de n
    len = sqrt(pow(vn->getX(), 2) + pow(vn->getY(), 2) + pow(vn->getZ(), 2));

    vn->setX(vn->getX()/len);
    vn->setY(vn->getY()/len);
    vn->setZ(vn->getZ()/len);

    // Returning Normalized Normal
    return vn;
}

int Aux::detectaColisao(Esfera *esfera, Esfera *esferaColisao, Bloco ***matriz, Tabuleiro *tabuleiro, Pad *pad, GameController *gameController,
                        int blocosCol, int blocosLin, double velocidadeEsfera, double fps, bool destroy)
{
    Vertice proxPos((esfera->getPosicao()->getX() + (velocidadeEsfera/fps)*esfera->getDirecao()->getX()),
                    (esfera->getPosicao()->getY() + (velocidadeEsfera/fps)*esfera->getDirecao()->getY()),
                    (esfera->getPosicao()->getZ()));

    double minX, maxX, minY, maxY, minZ, maxZ, x, y, z, distance;
    /// COLISAO COM OS BLOCOS DO TOPO DO TABULEIRO
    for (int i = 0; i < blocosCol; i++)
    {
        for (int j = 0; j < blocosLin; j++)
        {
            minX = matriz[i][j]->getParedeSul()->getTrianguloBase()->getVerticeA()->getX();
            maxX = matriz[i][j]->getParedeSul()->getTrianguloBase()->getVerticeB()->getX();

            minY = matriz[i][j]->getParedeSul()->getTrianguloBase()->getVerticeA()->getY();
            maxY = matriz[i][j]->getParedeSul()->getTrianguloBase()->getVerticeC()->getY();

            minZ = matriz[i][j]->getParedeLeste()->getTrianguloBase()->getVerticeA()->getZ();
            maxZ = matriz[i][j]->getParedeLeste()->getTrianguloBase()->getVerticeC()->getZ();

            // get box closest point to sphere center by clamping
            x = max(minX, min(proxPos.getX(), maxX));
            y = max(minY, min(proxPos.getY(), maxY));
            z = max(minZ, min(proxPos.getZ(), maxZ));

            distance = sqrt(pow((x - proxPos.getX()), 2) +
                pow((y - proxPos.getY()), 2) +
                pow((z - proxPos.getZ()), 2)
            );

            // Testa Colisao
            if (distance < esfera->getRaio() && !matriz[i][j]->getDestruido())
            {
                // descobrir que face a esfera colidiu
                // parede Leste?
                if (proxPos.getX() >= maxX && (proxPos.getY() >= minY && proxPos.getY() <= maxY))
                {
                    handleColisao(esfera, matriz[i][j]->getParedeLeste()->getTrianguloBase());
                }

                // parede Oeste?
                if (proxPos.getX() <= minX && (proxPos.getY() >= minY && proxPos.getY() <= maxY))
                {
                    handleColisao(esfera, matriz[i][j]->getParedeOeste()->getTrianguloBase());
                }

                // parede frente
                if (proxPos.getY() <= minY && (proxPos.getX() >= minX && proxPos.getX() <= maxX))
                {
                    handleColisao(esfera, matriz[i][j]->getParedeFrente()->getTrianguloBase());
                }

                // parede traseira
                if (proxPos.getY() >= maxY && (proxPos.getX() >= minX && proxPos.getX() <= maxX))
                {
                    handleColisao(esfera, matriz[i][j]->getParedeTraseira()->getTrianguloBase());
                }

                if (destroy)
                {
                    matriz[i][j]->setDestruido(true);
                }
            }
        }
    }

    /// COLISAO COM AS PARADES DO TABULEIRO
    // Norte
    if (proxPos.getY() >= tabuleiro->getParedeNorte()->getTrianguloBase()->getVerticeA()->getY() - esfera->getRaio())
    {
        handleColisao(esfera, tabuleiro->getParedeNorte()->getTrianguloBase());
    }
    // Sul
    if (proxPos.getY() <= tabuleiro->getParedeSul()->getTrianguloBase()->getVerticeA()->getY() - (esfera->getRaio()*4))
    {
//        handleColisao(esfera, tabuleiro->getParedeSul()->getTrianguloBase());
        if (destroy)
        {
            gameController->perdeVida();
            gameController->continuaFase(esfera, pad);
            return 2;
        }
        else if (esferaColisao!=nullptr)
        {
            if (esferaColisao->getSpawn()==1)
            {
                gameController->setSpawn1Fora(true);
                return 1;
            };

            if (esferaColisao->getSpawn()==2)
            {
                gameController->setSpawn2Fora(true);
                return 1;
            };
        }
    }
    // Leste
    if (proxPos.getX() >= tabuleiro->getParedeLeste()->getTrianguloBase()->getVerticeA()->getX() - esfera->getRaio())
    {
        handleColisao(esfera, tabuleiro->getParedeLeste()->getTrianguloBase());
    }
    // Oeste
    if (proxPos.getX() <= tabuleiro->getParedeOeste()->getTrianguloBase()->getVerticeA()->getX() + esfera->getRaio())
    {
        handleColisao(esfera, tabuleiro->getParedeOeste()->getTrianguloBase());
    }


    /// COLISAO COM O PAD DO JOGADOR
    minX = pad->getPad()->getParedeSul()->getTrianguloBase()->getVerticeA()->getX();
    maxX = pad->getPad()->getParedeSul()->getTrianguloBase()->getVerticeB()->getX();

    minY = pad->getPad()->getParedeSul()->getTrianguloBase()->getVerticeA()->getY();
    maxY = pad->getPad()->getParedeSul()->getTrianguloBase()->getVerticeC()->getY();

    minZ = pad->getPad()->getParedeLeste()->getTrianguloBase()->getVerticeA()->getZ();
    maxZ = pad->getPad()->getParedeLeste()->getTrianguloBase()->getVerticeC()->getZ();

    // get box closest point to sphere center by clamping
    x = max(minX, min(proxPos.getX(), maxX));
    y = max(minY, min(proxPos.getY(), maxY));
    z = max(minZ, min(proxPos.getZ(), maxZ));

    distance = sqrt(pow((x - proxPos.getX()), 2) +
        pow((y - proxPos.getY()), 2) +
        pow((z - proxPos.getZ()), 2)
    );

    // Testa Colisao
    if (distance < esfera->getRaio())
    {
        // descobrir que face a esfera colidiu
        // parede Leste?
        if (proxPos.getX() >= maxX && (proxPos.getY() >= minY && proxPos.getY() <= maxY))
        {
            handleColisao(esfera, pad->getPad()->getParedeLeste()->getTrianguloBase());
        }

        // parede Oeste?
        if (proxPos.getX() <= minX && (proxPos.getY() >= minY && proxPos.getY() <= maxY))
        {
            handleColisao(esfera, pad->getPad()->getParedeOeste()->getTrianguloBase());
        }

        // parede frente
        if (proxPos.getY() <= minY && (proxPos.getX() >= minX && proxPos.getX() <= maxX))
        {
            handleColisao(esfera, pad->getPad()->getParedeFrente()->getTrianguloBase());
        }

        // parede traseira
        if (proxPos.getY() >= maxY && (proxPos.getX() >= minX && proxPos.getX() <= maxX))
        {
            handleColisao(esfera, pad->getPad()->getParedeTraseira()->getTrianguloBase());
        }
    }

    /// COLISAO ESFERA X ESFERA(SPAWNED OBJECTS)
    if (esferaColisao!=nullptr)
    {
        // a-> esfera, b->esferaColisao
        // vector between the centers of each sphere
        Vertice *s = new Vertice(esfera->getPosicao()->getX() - esferaColisao->getPosicao()->getX(),
                                 esfera->getPosicao()->getY() - esferaColisao->getPosicao()->getY(),
                                 esfera->getPosicao()->getZ() - esferaColisao->getPosicao()->getZ()
        );

        // relative velocity between spheres
        Vertice proxPos2((esferaColisao->getPosicao()->getX() + (velocidadeEsfera/fps)*esferaColisao->getDirecao()->getX()),
                         (esferaColisao->getPosicao()->getY() + (velocidadeEsfera/fps)*esferaColisao->getDirecao()->getY()),
                         (esferaColisao->getPosicao()->getZ()));

        Vertice *v = new Vertice(proxPos.getX() - proxPos2.getX(),
                                 proxPos.getY() - proxPos2.getY(),
                                 proxPos.getZ() - proxPos2.getZ()
        );

        double sumRadius = esfera->getRaio() + esferaColisao->getRaio();

        double c1 = (pow(v->getX(), 2) + pow(v->getY(), 2) + pow(v->getZ(), 2)) - pow(sumRadius, 2); // if negative, they overlap
        if (c1 < esferaColisao->getRaio()) // if true, they already overlap
        {
            handleColisao(esfera, esferaColisao);
            if (esferaColisao->getSpawn()==0)
            {
                if (esfera->getSpawn()==1)
                    gameController->setSpawn1Fora(true);
                if (esfera->getSpawn()==2)
                    gameController->setSpawn2Fora(true);
            }
        }
    }

    return 0;
}
void Aux::handleColisao(Esfera *esfera1, Esfera *esfera2)
{
    double len1 = sqrt(pow(esfera1->getDirecao()->getX(), 2) +
        pow(esfera1->getDirecao()->getY(), 2) +
        pow(esfera1->getDirecao()->getZ(), 2));

    double len2 = sqrt(pow(esfera2->getDirecao()->getX(), 2) +
        pow(esfera2->getDirecao()->getY(), 2) +
        pow(esfera2->getDirecao()->getZ(), 2));

    esfera1->setDirecao(new Vertice(
        esfera1->getDirecao()->getX()/len1,
        esfera1->getDirecao()->getY()/len1,
        esfera1->getDirecao()->getZ()/len1));

    esfera2->setDirecao(new Vertice(
        esfera2->getDirecao()->getX()/len2,
        esfera2->getDirecao()->getY()/len2,
        esfera2->getDirecao()->getZ()/len2));

    // r=e−2(e⋅n)n
    double prodEsc = (esfera2->getDirecao()->getX()*esfera1->getDirecao()->getX()) +
        (esfera2->getDirecao()->getY()*esfera1->getDirecao()->getY()) +
        (esfera2->getDirecao()->getZ()*esfera1->getDirecao()->getZ());

    prodEsc = 2*prodEsc; // 2(e.n)
    Vertice *v = new Vertice(prodEsc*esfera2->getDirecao()->getX(),   //2(e.n)n
                             prodEsc*esfera2->getDirecao()->getY(),
                             prodEsc*esfera2->getDirecao()->getZ());

    Vertice *r1 = new Vertice(esfera1->getDirecao()->getX() - v->getX(),
                              esfera1->getDirecao()->getY() - v->getY(),
                              esfera1->getDirecao()->getZ() - v->getZ());

    Vertice *r2 = new Vertice(-r1->getX(),
                              -r1->getX(),
                              -r1->getX());

    esfera1->setDirecao(r1);
    esfera2->setDirecao(r2);

}

void Aux::handleColisao(Esfera *esfera, Triangulo *triangulo)
{
    // r=e−2(e⋅n)n
    double prodEsc = (triangulo->getVetorNormal()->getX()*esfera->getDirecao()->getX()) +
        (triangulo->getVetorNormal()->getY()*esfera->getDirecao()->getY()) +
        (triangulo->getVetorNormal()->getZ()*esfera->getDirecao()->getZ());

    prodEsc = 2*prodEsc; // 2(e.n)
    Vertice *v = new Vertice(prodEsc*triangulo->getVetorNormal()->getX(),   //2(e.n)n
                             prodEsc*triangulo->getVetorNormal()->getY(),
                             prodEsc*triangulo->getVetorNormal()->getZ());

    Vertice *r = new Vertice(esfera->getDirecao()->getX() - v->getX(),
                             esfera->getDirecao()->getY() - v->getY(),
                             esfera->getDirecao()->getZ() - v->getZ());
    esfera->setDirecao(r);
}
