#include "tabuleiro.h"

/**
 * @posInicial
 */
Tabuleiro::Tabuleiro(Vertice *posicao_inicial, double p_tam_base, double p_tam_altura, double p_tam_altura_paredes)
{
    pos = posicao_inicial;
    tam_base = p_tam_base;
    tam_altura = p_tam_altura;
    tam_altura_paredes = p_tam_altura_paredes;
    monta_Tabuleiro();
}

Tabuleiro::~Tabuleiro()
{
    //dtor
}

void Tabuleiro::monta_Tabuleiro()
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
    vA_Topo.define_x(vA_Topo.pega_x() + tam_base);
    vA_Topo.define_y(vA_Topo.pega_y() + tam_altura);

    Triangulo *trianguloBase_Base = new Triangulo(&vA_Base, tam_base, tam_altura, 0);
    Triangulo *trianguloTopo_Base = new Triangulo(&vA_Topo, tam_base, tam_altura, 1);

    base = new Plano(trianguloBase_Base, trianguloTopo_Base);

    // monta parede norte
    // vCz vATopoz
    // o---o
    // |\  |
    // | \ |
    // |  \|
    // o---o
    // vC  vATopo
    Triangulo *trianguloBase_Norte = new Triangulo(&*trianguloBase_Base->pega_vertice_c(), tam_base, tam_altura_paredes, 2);

    Vertice vA_TopoZ = vA_Topo;
    vA_TopoZ.define_z(vA_TopoZ.pega_z() + tam_altura_paredes);
    Triangulo *trianguloTopo_Norte = new Triangulo(&vA_TopoZ, tam_base, tam_altura_paredes, 3);

    parede_norte = new Plano(trianguloBase_Norte, trianguloTopo_Norte);
//
//    // monta parede leste
//    // vBz vATopoz
//    // o---o
//    // |\  |
//    // | \ |
//    // |  \|
//    // o---o
//    // vB vATopo
    Triangulo *trianguloBase_Leste = new Triangulo(trianguloBase_Base->pega_vertice_b(), tam_altura, tam_altura_paredes, 4);
    Triangulo *trianguloTopo_Leste = new Triangulo(&vA_TopoZ, tam_altura, tam_altura_paredes, 5);

//    trianguloBase_Leste->define_vetor_normal(
//        new Vertice(
//        trianguloBase_Leste->pega_vetor_normal()->pega_x()*-1,
//        trianguloBase_Leste->pega_vetor_normal()->pega_y(),
//        trianguloBase_Leste->pega_vetor_normal()->pega_z()
//        )
//    );
//
//    trianguloTopo_Leste->define_vetor_normal(
//        new Vertice(
//        trianguloTopo_Leste->pega_vetor_normal()->pega_x()*-1,
//        trianguloTopo_Leste->pega_vetor_normal()->pega_y(),
//        trianguloTopo_Leste->pega_vetor_normal()->pega_z()
//        )
//    );

    parede_leste = new Plano(trianguloBase_Leste, trianguloTopo_Leste);

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
    Triangulo *trianguloBase_Sul = new Triangulo(trianguloBase_Base->pega_vertice_b(), tam_base, tam_altura_paredes, 2);

    Vertice vBz = *trianguloBase_Base->pega_vertice_b();
    vBz.define_z(vBz.pega_z() + tam_altura_paredes);
    Triangulo *trianguloTopo_Sul = new Triangulo(&vBz, tam_base, tam_altura_paredes, 3);

    parede_sul = new Plano(trianguloBase_Sul, trianguloTopo_Sul);
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
    Triangulo *trianguloBase_Oeste = new Triangulo(&vA_Base, tam_altura, tam_altura_paredes, 4);

    Vertice vCz = *trianguloBase_Base->pega_vertice_c();
    vCz.define_z(vCz.pega_z() + tam_altura_paredes);
    Triangulo *trianguloTopo_Oeste = new Triangulo(&vCz, tam_altura, tam_altura_paredes, 5);

    parede_oeste = new Plano(trianguloBase_Oeste, trianguloTopo_Oeste);
}
