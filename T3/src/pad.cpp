#include "pad.h"


/// pad será um bloco indestrutivel
Pad::Pad(Vertice *vertice_ponto, double valor_tamanho_base, double valor_tamanho_altura, double valor_tamanho_profundidade)
{
    pad = new Bloco(vertice_ponto, valor_tamanho_base, valor_tamanho_altura, valor_tamanho_profundidade, false);
}

Pad::~Pad()
{
    //dtor
}
