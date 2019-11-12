#include "Pad.h"
#include "Bloco.h"

/// pad será um bloco indestrutivel
Pad::Pad(Vertice* verticePonto, double valorTamanhoBase, double valorTamanhoAltura, double valorTamanhoProfundidade)
{
    pad = new Bloco(verticePonto, valorTamanhoBase, valorTamanhoAltura, valorTamanhoProfundidade, false);
}

Pad::~Pad()
{
    //dtor
}
