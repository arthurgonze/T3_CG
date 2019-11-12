#include "Esfera.h"

Esfera::Esfera(Vertice* newPos, double valorRaio)
{
    pos = newPos;
    raio = valorRaio;
    direcao = new Vertice(0.0,1.0,0.0);
    slices = 30;
    stacks = 30;
}

Esfera::~Esfera()
{
    //dtor
}
