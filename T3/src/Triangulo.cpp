#include "Triangulo.h"


// tipo:
// 0, 2 ou 4:  |     1, 3 ou 5:
// C              B--A
// |\              \ |
// | \      OR      \|
// A--B              C
Triangulo::Triangulo(Vertice* pontoA, double pTamBase, double pTamAltura, int tipo)
{
    Aux aux;
    tamBase = pTamBase;
    tamAltura = pTamAltura;

    // nao permitir coordenadas negativas
    pontoA->setX(fabs(pontoA->getX()));
    pontoA->setY(fabs(pontoA->getY()));
    pontoA->setZ(fabs(pontoA->getZ()));

    verticeA = new Vertice(pontoA->getX(), pontoA->getY(), pontoA->getZ());
    verticeB = new Vertice(pontoA->getX(), pontoA->getY(), pontoA->getZ());
    verticeC = new Vertice(pontoA->getX(), pontoA->getY(), pontoA->getZ());

    switch(tipo)
    {
    case 0: // triangulo XY base normal pra cima
        verticeB->setX(verticeB->getX()+tamBase);
        verticeC->setY(verticeC->getY()+tamAltura);
        vetorNormal = aux.CalculaNormal(verticeA, verticeB, verticeC);
        break;
    case 1: // trinagulo XY topo normal pra cima
        verticeB->setX(verticeB->getX()-tamBase);
        verticeC->setY(verticeC->getY()-tamAltura);
        vetorNormal = aux.CalculaNormal(verticeA, verticeB, verticeC);
        break;
    case 2: // triangulo XZ base normal pra sul
        verticeB->setX(verticeB->getX()+tamBase);
        verticeC->setZ(verticeC->getZ()+tamAltura);
        vetorNormal = aux.CalculaNormal(verticeA, verticeB, verticeC);
        break;
    case 3: // triangulo XZ topo normal pra sul
        verticeB->setX(verticeB->getX()-tamBase);
        verticeC->setZ(verticeC->getZ()-tamAltura);
        vetorNormal = aux.CalculaNormal(verticeA, verticeB, verticeC);
        break;
    case 4: // triangulo YZ base normal pra leste
        verticeB->setY(verticeB->getY()+tamBase);
        verticeC->setZ(verticeC->getZ()+tamAltura);
        vetorNormal = aux.CalculaNormal(verticeA, verticeB, verticeC);
        break;
    case 5: // triangulo YZ topo normal pra leste
        verticeB->setY(verticeB->getY()-tamBase);
        verticeC->setZ(verticeC->getZ()-tamAltura);
        vetorNormal = aux.CalculaNormal(verticeA, verticeB, verticeC);
        break;
    default:
        cout << "Tipo de triangulo invalido" << endl;
        break;
    }

    posX = (verticeA->getX()+verticeB->getX()+verticeC->getX())/3;
    posY = (verticeA->getY()+verticeB->getY()+verticeC->getY())/3;
    posZ = (verticeA->getZ()+verticeB->getZ()+verticeC->getZ())/3;
}

Triangulo::~Triangulo()
{
    //dtor
}
