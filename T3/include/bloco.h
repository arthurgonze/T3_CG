#ifndef BLOCO_H
#define BLOCO_H

#include "vertice.h"
#include "plano.h"
#include "triangulo.h"
#include "desenha.h"


//       Y
//       ^     tam_base
//       | v6o------o v7
//       |  /|     /|
//       | / |    / |
//       |/  | v5/  |
//    v4 o---|--o   |tam_altura
//       |   |  |   |
//       |   |  |   |
//       |   o--|---o v3
//       |  /v2 |  /
//       | /    | / tam_profundidade
//       |/     |/
// ponto o------o----------> X
//      /       v1
//     /
//    /
//   V
//   Z
class Plano;

class Bloco
{
private:
    double tam_base;
    double tam_altura;
    double tam_profundidade;
    bool destrutivel; // blocos e pad utilizarão o mesmo tad, entao esse booleano pode ajudar, já que o pad não pode ser destruído
    bool destruido;
    Vertice* ponto;
    Plano* parede_norte;
    Plano* parede_leste;
    Plano* parede_sul;
    Plano* parede_oeste;
    Plano* parede_frente;
    Plano* parede_traseira;

    void monta_bloco();

public:
    /// CONSTRUTOR & DESTRUTOR
    Bloco();
    Bloco(Vertice *vertice_ponto, double valor_tam_base, double valor_tam_altura, double valor_tam_profundidade, bool bloco_destrutivel);
    virtual ~Bloco();

    /// GETTERS
    inline Plano* pega_parede_norte() {return parede_norte;};
    inline Plano* pega_parede_leste(){return parede_leste;};
    inline Plano* pega_parede_sul(){return parede_sul;};
    inline Plano* pega_parede_oeste(){return parede_oeste;};
    inline Plano* pega_parede_frente(){return parede_frente;};
    inline Plano* pega_parede_traseira(){return parede_traseira;};
    inline Vertice* pega_vertice(){return ponto;};
    inline double pega_tam_base(){return tam_base;};
    inline double pega_tam_altura(){return tam_altura;};
    inline double pega_tam_profundidade(){return tam_profundidade;};
    inline bool pega_destrutivel(){return destrutivel;};
    inline bool pega_destruido(){return destruido;};
    Vertice pega_pos();

    /// SETTERS
    inline void define_parede_norte(Plano *parede){parede_norte = parede;}
    inline void define_parede_leste(Plano *parede){parede_leste = parede;}
    inline void define_parede_sul(Plano *parede){parede_sul = parede;}
    inline void define_parede_oeste(Plano *parede){parede_oeste = parede;}
    inline void define_parede_frente(Plano *parede){parede_frente = parede;}
    inline void define_parede_traseira(Plano *parede){parede_traseira = parede;}
    inline void define_vertice(Vertice *vertice_ponto){ponto = vertice_ponto;
        monta_bloco();}
    inline void define_tam_base(double valor_tam_base){tam_base = valor_tam_base;}
    inline void define_tam_altura(double valor_tam_altura){tam_altura = valor_tam_altura;}
    inline void define_tam_profundidade(double valor_tam_profundidade){tam_profundidade = valor_tam_profundidade;}
    inline void define_destruido(bool alternancia){destruido=alternancia;}
    inline void altera_destrutivel(){destrutivel = !destrutivel;}
};

#endif // bloco_H
