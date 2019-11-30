#include "esfera.h"

Esfera::Esfera(Vertice *new_pos, double valor_raio)
{
    pos = new_pos;
    raio = valor_raio;
    direcao = new Vertice(0.0, 1.0, 0.0);
    slices = 30;
    stacks = 30;
}

Esfera::~Esfera()
{
    //dtor
}
