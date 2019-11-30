#include "Plano.h"

Plano::Plano(Triangulo *pTrianguloBase, Triangulo *pTrianguloTopo)
{
    trianguloBase = pTrianguloBase;
    trianguloTopo = pTrianguloTopo;
}

Plano::~Plano()
{
    //dtor
}
