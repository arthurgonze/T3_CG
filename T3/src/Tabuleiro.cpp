#include "Tabuleiro.h"

/**
 * @posInicial
 */
Tabuleiro::Tabuleiro(Vertice *posicaoInicial, double pTamBase, double pTamAltura, double pTamAlturaParedes)
{
    pos = posicaoInicial;
    tamBase = pTamBase;
    tamAltura = pTamAltura;
    tamAlturaParedes = pTamAlturaParedes;
    montaTabuleiro();
}

Tabuleiro::~Tabuleiro()
{
    //dtor
}

void Tabuleiro::montaTabuleiro()
{
    // monta Base
    // vC     vATopo
    // o------o
    // |\tTopo|
    // | \    |
    // |  \   |
    // |   \  |
    // |    \ |
    // |tBASE\|
    // o------o
    // vABase vB
    Vertice vA_Base = *pos;
    Vertice vA_Topo = *pos;
    vA_Topo.setX(vA_Topo.getX() + tamBase);
    vA_Topo.setY(vA_Topo.getY() + tamAltura);

    Triangulo *trianguloBase_Base = new Triangulo(&vA_Base, tamBase, tamAltura, 0);
    Triangulo *trianguloTopo_Base = new Triangulo(&vA_Topo, tamBase, tamAltura, 1);

    base = new Plano(trianguloBase_Base, trianguloTopo_Base);

    // monta parede norte
    // vCz vATopoz
    // o---o
    // |\  |
    // | \ |
    // |  \|
    // o---o
    // vC  vATopo
    Triangulo *trianguloBase_Norte = new Triangulo(&*trianguloBase_Base->getVerticeC(), tamBase, tamAlturaParedes, 2);

    Vertice vA_TopoZ = vA_Topo;
    vA_TopoZ.setZ(vA_TopoZ.getZ() + tamAlturaParedes);
    Triangulo *trianguloTopo_Norte = new Triangulo(&vA_TopoZ, tamBase, tamAlturaParedes, 3);

    paredeNorte = new Plano(trianguloBase_Norte, trianguloTopo_Norte);
//
//    // monta parede leste
//    // vBz vATopoz
//    // o---o
//    // |\  |
//    // | \ |
//    // |  \|
//    // o---o
//    // vB vATopo
    Triangulo *trianguloBase_Leste = new Triangulo(trianguloBase_Base->getVerticeB(), tamAltura, tamAlturaParedes, 4);
    Triangulo *trianguloTopo_Leste = new Triangulo(&vA_TopoZ, tamAltura, tamAlturaParedes, 5);

//    trianguloBase_Leste->setVetorNormal(
//        new Vertice(
//        trianguloBase_Leste->getVetorNormal()->getX()*-1,
//        trianguloBase_Leste->getVetorNormal()->getY(),
//        trianguloBase_Leste->getVetorNormal()->getZ()
//        )
//    );
//
//    trianguloTopo_Leste->setVetorNormal(
//        new Vertice(
//        trianguloTopo_Leste->getVetorNormal()->getX()*-1,
//        trianguloTopo_Leste->getVetorNormal()->getY(),
//        trianguloTopo_Leste->getVetorNormal()->getZ()
//        )
//    );

    paredeLeste = new Plano(trianguloBase_Leste, trianguloTopo_Leste);

//    // monta parede sul
//    // vABasez vBz
//    // o-------o
//    // |\      |
//    // | \     |
//    // |  \    |
//    // |   \   |
//    // |    \  |
//    // |     \ |
//    // |      \|
//    // o-------o
//    // vABase vB
    Triangulo *trianguloBase_Sul = new Triangulo(trianguloBase_Base->getVerticeB(), tamBase, tamAlturaParedes, 2);

    Vertice vBz = *trianguloBase_Base->getVerticeB();
    vBz.setZ(vBz.getZ() + tamAlturaParedes);
    Triangulo *trianguloTopo_Sul = new Triangulo(&vBz, tamBase, tamAlturaParedes, 3);

    paredeSul = new Plano(trianguloBase_Sul, trianguloTopo_Sul);
//
//    // monta parede oeste
//    // vABasez vCz
//    // o-------o
//    // |\      |
//    // | \     |
//    // |  \    |
//    // |   \   |
//    // |    \  |
//    // |     \ |
//    // |      \|
//    // o-------o
//    // vABase vC
    Triangulo *trianguloBase_Oeste = new Triangulo(&vA_Base, tamAltura, tamAlturaParedes, 4);

    Vertice vCz = *trianguloBase_Base->getVerticeC();
    vCz.setZ(vCz.getZ() + tamAlturaParedes);
    Triangulo *trianguloTopo_Oeste = new Triangulo(&vCz, tamAltura, tamAlturaParedes, 5);

    paredeOeste = new Plano(trianguloBase_Oeste, trianguloTopo_Oeste);
}
