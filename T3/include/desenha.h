#ifndef DESENHA_H
#define DESENHA_H

#include <GL/glut.h>
#include "aux.h"
#include "tabuleiro.h"
#include "triangulo.h"
#include "vertice.h"
#include "plano.h"
#include "esfera.h"
#include "bloco.h"

class Bloco;
class Triangulo;
class Tabuleiro;
class Desenha
{
private:
    void desenha_plano(Plano *plano, bool inv, int normal_dir);
    void desenha_triangulo(Triangulo *triangulo, bool inv, int normal_dir);
    void desenha_normal(Triangulo *triangulo, int dir);

public:
    /// CONSTRUCTOR & DESTRUCTOR
    Desenha();
    virtual ~Desenha();

    void desenha_tabuleiro(Tabuleiro *tabuleiro);
    void desenha_esfera(Esfera *esfera);
    void desenha_bloco(Bloco *bloco);
    void desenha_seta_direcao(Esfera *esfera, double angulo_disparo);
    void desenha_vetor_direcao_esfera(Esfera *esfera);
    void desenha_matriz_blocos(Bloco ***matriz, int blocos_Col, int blocos_lin);
    void desenha_skybox(Bloco *skybox);
};

#endif // DESENHA_H
