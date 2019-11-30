#ifndef AUX_H
#define AUX_H

#include "vertice.h"
#include "triangulo.h"
#include "bloco.h"
#include "esfera.h"
#include "tabuleiro.h"
#include "pad.h"
#include "gameController.h"
#include <algorithm>//max
#include <numeric>
#include <math.h> //sqrt

class Bloco;
class Triangulo;
class Tabuleiro;
class Pad;
class GameController;
class Aux
{
private:
    void resolve_colisao(Esfera *esfera, Triangulo *triangulo);
    void resolve_colisao(Esfera *esfera1, Esfera *esfera2);
    Vertice *normaLizaVetor(Vertice *vetor);
public:
    Aux();
    virtual ~Aux();
    Vertice *calcula_normal(Vertice *vertice_a, Vertice *vertice_b, Vertice *vertice_c);
    int detecta_colisao(Esfera *esfera, Esfera *esfera_colisao, Bloco ***matriz, Tabuleiro *tabuleiro, Pad *pad, GameController *game_controller, int blocos_col, int blocos_lin, double velocidade_esfera, double fps, bool destroy);
};

#endif // AUX_H
