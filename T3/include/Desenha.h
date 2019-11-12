#ifndef DESENHA_H
#define DESENHA_H

#include <GL/glut.h>
#include "Aux.h"
#include "Tabuleiro.h"
#include "Triangulo.h"
#include "Vertice.h"
#include "Plano.h"
#include "Esfera.h"
#include "Bloco.h"

class Desenha
{
private:
    void desenhaPlano(Plano* plano, bool inv, int normalDir);
    void desenhaTriangulo(Triangulo* Triangulo, bool inv, int normalDir);
    void desenhaNormal(Triangulo* triangulo, int dir);
    double xMaxBarra, xMinBarra;

public:
    /// CONSTRUCTOR & DESTRUCTOR
    Desenha();
    virtual ~Desenha();

    void desenhaTabuleiro(Tabuleiro* tabuleiro);
    void desenhaEsfera(Esfera* esfera);
    void desenhaBloco(Bloco* bloco);
    void desenhaBarraVelocidade_Background(Tabuleiro* tabuleiro, double velMax);
    void desenhaBarraVelocidade_Medidor(Tabuleiro* tabuleiro, double velocidadeDisparo, double velocidadeMax);
    void desenhaSetaDirecao(Esfera* esfera, double anguloDisparo);
    void desenhaVetorDirecaoEsfera(Esfera* esfera);
    void desenhaMatrizBlocos(Bloco*** matriz, int blocosCol, int blocosLin);
};

#endif // DESENHA_H
