#ifndef ESFERA_H
#define ESFERA_H

#include "Vertice.h"

class Esfera
{
private:
    int slices, stacks;
    double raio;
    Vertice* pos;
    Vertice* direcao;
    int spawn; // 0->nao é, 1-> primeiro spawn, 2-> segundo spawn
    //Vertice* posReset;
public:
    /// CONSTRUCTOR & DESTRUCTOR
    Esfera(Vertice* newPos, double valorRaio);
    virtual ~Esfera();

    /// GETTERS
    inline Vertice* getPosicao(){ return pos;};
    inline Vertice* getDirecao(){return direcao;};
    inline double getRaio(){return raio;};
    inline int getSlices(){return slices;};
    inline int getStacks(){return stacks;};
    inline int getSpawn(){return spawn;}
    //inline Vertice* getPosReset();

    /// SETTERS
    inline void setDirecao(Vertice* novaDirecao){direcao = novaDirecao;};
    inline void setPosicao(Vertice* vetorPosicoes){pos = vetorPosicoes;};
    inline void setRaio(double valorRaio){raio = valorRaio;};
    inline void setSlices(int novoSlices){slices = novoSlices;};
    inline void setStacks(int novoStacks){stacks = novoStacks;};
    inline void setSpawn(int nSpawn){spawn = nSpawn;}
    //inline void setPosReset(Vertice* novaPosReset){posReset = novaPosReset;}

    //inline void resetPos(){pos = posReset;}
};

#endif // ESFERA_H
