#ifndef TRIANGULO_H
#define TRIANGULO_H

#include <iostream>
#include <cmath>
#include "Vertice.h"
#include "Aux.h"

using namespace std;
class Triangulo
{
    // tipo:
    // 0, 2 ou 4:  |     1, 3 ou 5:
    // C           |     B--A
    // |\          OR     \ |
    // | \         |       \|
    // A--B        |        C
    // tipo 0: tamBase aplicado ao eixo X e tamAltura aplicado ao eixo Y
    // tipo 1: tamBase aplicado ao eixo X e tamAltura aplicado ao eixo Y
    // tipo 2: tamBase aplicado ao eixo X e tamAltura aplicado ao eixo Z
    // tipo 3: tamBase aplicado ao eixo X e tamAltura aplicado ao eixo Z
    // tipo 4: tamBase aplicado ao eixo Y e tamAltura aplicado ao eixo Z
    // tipo 5: tamBase aplicado ao eixo Y e tamAltura aplicado ao eixo Z

private:
    Vertice* verticeA;
    Vertice* verticeB;
    Vertice* verticeC;
    Vertice* vetorNormal;
    double posX, posY, posZ;
    double tamBase; // B-A ou A-C
    double tamAltura; // A-B ou C-A
    int tipo;
public:
    /// CONSTRUCTOR & DESTRUCTOR
    Triangulo(Vertice* pontoA, double pTamBase, double pTamAltura, int tipo);
    virtual ~Triangulo();

    /// GETTERS
    inline Vertice* getVerticeA(){return verticeA;};
    inline Vertice* getVerticeB(){return verticeB;};
    inline Vertice* getVerticeC(){return verticeC;};
    inline Vertice* getVetorNormal(){return vetorNormal;};
    inline double getTamBase(){return tamBase;};
    inline double getTamAltura(){return tamAltura;};
    inline int getTipo(){return tipo;}
    inline double getX(){return posX;}
    inline double getY(){return posY;}
    inline double getZ(){return posZ;}

    /// SETTERS
    inline void setVerticeA(Vertice* pontoA){verticeA = pontoA;};
    inline void setVerticeB(Vertice* pontoB){verticeB = pontoB;};
    inline void setVerticeC(Vertice* pontoC){verticeC = pontoC;};
    inline void setVetorNormal(Vertice* novaNormal){vetorNormal = novaNormal;};
    inline void setTamBase(double novoTamBase){tamBase = novoTamBase;};
    inline void setTamAltura(double novoTamAltura){tamAltura = novoTamAltura;};
    inline void setTipo(int novoTipo){tipo = novoTipo;};
};

#endif // TRIANGULO_H
