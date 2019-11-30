#ifndef TABULEIRO_H
#define TABULEIRO_H

#include "triangulo.h"
#include "vertice.h"
#include "triangulo.h"
#include "plano.h"

class Plano;
class Tabuleiro
{
private:
    Plano *base;
    Plano *parede_norte;
    Plano *parede_sul;
    Plano *parede_leste;
    Plano *parede_oeste;
    Vertice *pos;
    double tam_base;
    double tam_altura;
    double tam_altura_paredes;

    void monta_Tabuleiro(); /// funcao para setar os planos do tabuleiro a partir das informacoes do construtor
public:
    /// CONSTRUTOR & DESTRUTOR
    Tabuleiro(Vertice *posicao_inicial, double p_tam_base, double p_tam_altura, double p_tam_altura_paredes);
    virtual ~Tabuleiro();

    /// GETTERS
    inline Plano *pega_base() { return base; };
    inline Plano *pega_parede_norte() { return parede_norte; };
    inline Plano *pega_parede_leste() { return parede_leste; };
    inline Plano *pega_parede_sul() { return parede_sul; };
    inline Plano *pega_parede_oeste() { return parede_oeste; };
    inline Vertice *pega_posicao_inicial() { return pos; };
    inline double pega_tam_base() { return tam_base; };
    inline double pega_tam_altura() { return tam_altura; };
    inline double pega_tam_altura_paredes() { return tam_altura_paredes; };

    /// SETTERS
    inline void define_base(Plano *nova_base) { base = nova_base; };
    inline void define_parede_norte(Plano *nova_parede_norte) { parede_norte = nova_parede_norte; };
    inline void define_parede_leste(Plano *nova_parede_leste) { parede_leste = nova_parede_leste; };
    inline void define_parede_sul(Plano *nova_parede_sul) { parede_sul = nova_parede_sul; };
    inline void define_parede_oeste(Plano *nova_parede_oeste) { parede_oeste = nova_parede_oeste; };
    inline void define_posicao(Vertice *nova_posicao) { pos = nova_posicao; };
    inline void define_tam_base(double novo_tam_base) { tam_base = novo_tam_base; };
    inline void define_tam_altura(double novo_tam_altura) { tam_altura = novo_tam_altura; };
    inline void define_tam_altura_paredes(double novo_tam_altura_paredes) { tam_altura_paredes = novo_tam_altura_paredes; };
};

#endif // TABULEIRO_H
