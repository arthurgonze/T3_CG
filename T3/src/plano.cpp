#include "plano.h"

Plano::Plano(Triangulo *p_triangulo_base, Triangulo *p_triangulo_topo)
{
    triangulo_base = p_triangulo_base;
    triangulo_topo = p_triangulo_topo;
}

Plano::~Plano()
{
    //dtor
}
