#ifndef ESFERA_H
#define ESFERA_H

#include "vertice.h"

class Esfera
{
private:
    int slices, stacks;
    double raio;
    Vertice *pos;
    Vertice *direcao;
    int spawn; // 0->nao é, 1-> primeiro spawn, 2-> segundo spawn
public:
    /// CONSTRUTOR & DESTRUTOR
    Esfera(Vertice *new_pos, double valor_raio);
    virtual ~Esfera();

    /// GETTERS
    inline Vertice *pega_posicao() { return pos; };
    inline Vertice *pega_direcao() { return direcao; };
    inline double pega_raio() { return raio; };
    inline int pega_slices() { return slices; };
    inline int pega_stacks() { return stacks; };
    inline int pega_spawn() { return spawn; }

    /// SETTERS
    inline void define_direcao(Vertice *nova_direcao) { direcao = nova_direcao; };
    inline void define_posicao(Vertice *vetor_posicoes) { pos = vetor_posicoes; };
    inline void define_raio(double valor_raio) { raio = valor_raio; };
    inline void define_slices(int novo_slices) { slices = novo_slices; };
    inline void define_stacks(int novo_stacks) { stacks = novo_stacks; };
    inline void define_spawn(int n_spawn) { spawn = n_spawn; }
};

#endif // ESFERA_H
