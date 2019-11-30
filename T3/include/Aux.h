#ifndef AUX_H
#define AUX_H

#include "Vertice.h"
#include "Triangulo.h"
#include "Bloco.h"
#include "Esfera.h"
#include "Tabuleiro.h"
#include "Pad.h"
#include "GameController.h"
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
    void handleColisao(Esfera *esfera, Triangulo *triangulo);
    //void handleColisao(Esfera* esfera1, Esfera* esfera2, Vertice v1, Vertice v2);
    void handleColisao(Esfera *esfera1, Esfera *esfera2);
    Vertice *normaLizaVetor(Vertice *vetor);
public:
    Aux();
    virtual ~Aux();
    Vertice *CalculaNormal(Vertice *verticeA, Vertice *verticeB, Vertice *verticeC);
    int detectaColisao(Esfera *esfera, Esfera *esferaColisao, Bloco ***matriz, Tabuleiro *tabuleiro, Pad *pad, GameController *gameController, int blocosCol, int blocosLin, double velocidadeEsfera, double fps, bool destroy);
};

#endif // AUX_H
