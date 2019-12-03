#ifndef AUX_H
#define AUX_H

#include "vertice.h"
#include "triangulo.h"
#include "bloco.h"
#include "esfera.h"
#include "tabuleiro.h"
#include "pad.h"
#include "gameController.h"
#include "glcSound.h"
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
    void resolve_colisao(Esfera *esfera1, Tabuleiro *tabuleiro, bool leste);
    void resolve_colisao(Esfera *esfera1, Pad *rebatedor);
    //Vertice *normaLizaVetor(Vertice *vetor);
    Vertice *normal_vertice_parede(Vertice *vertices_elipse, int nSeg, double tamParedes, Esfera *esfera1, bool inverso);
    Vertice *normal_vertice_pad(Vertice *vertices_elipse, int nSeg, double tamPad, Esfera *esfera1, bool inverso);
    bool detecta_colisao_esfera_elipse(double x0, double y0, double w, double h, double x1, double y1, double r);
    bool iterate(double x, double y, double c0x, double c0y, double c2x, double c2y, double rr);
public:
    Aux();
    virtual ~Aux();
    Vertice *calcula_normal(Vertice *vertice_a, Vertice *vertice_b, Vertice *vertice_c);
    int detecta_colisao_esfera_tabuleiro(Esfera *esfera, Tabuleiro *tabuleiro, GameController *game_controller, Pad* pad, bool destroy, glcSound *som);
    int detecta_colisao_esfera_blocos(Esfera *esfera,Bloco ***matriz, GameController *game_controller, bool destroy, glcSound *som);
    int detecta_colisao_esfera_objetos_importados(Esfera *esfera, Esfera *esfera_colisao, GameController *game_controller, glcSound *som);
    int detecta_colisao_esfera_rebatedor(Esfera *esfera, Pad *pad, GameController *game_controller, glcSound *som);
};

#endif // AUX_H
