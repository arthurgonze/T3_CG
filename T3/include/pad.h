#ifndef PAD_H
#define PAD_H

#include "bloco.h"

class Bloco;
class Pad
{
private:
    Bloco *pad;
    Vertice vertices_elipse[15];
    double centro_x, centro_y, raio_x, raio_y, valor_tamanho_profundidade, valor_tamanho_altura, valor_tamanho_base;
    int num_segmentos;
    void monta_elipses();
public:
    /// CONSTRUTOR & DESTRUTOR
    Pad(Vertice *vertice_ponto, double valor_tamanho_base, double valor_tamanho_altura, double valor_tamanho_profundidade);
    virtual ~Pad();

    /// GETTERS
    inline Bloco *pega_pad() { monta_elipses(); return pad; }
    inline Vertice *pega_Vertices_elipse() { return vertices_elipse; }
    inline Bloco *pega_Pad() { return pad; }
    inline double pega_Centro_x() const { return centro_x; }
    inline double pega_Centro_y() const { return centro_y; }
    inline double pega_Raio_x() const { return raio_x; }
    inline double pega_Raio_y() const { return raio_y; }
    inline int pega_Num_segmentos() const { return num_segmentos; }
    inline double pega_Valor_tamanho_profundidade() const { return valor_tamanho_profundidade; }
    inline double pega_Valor_tamanho_altura() const { return valor_tamanho_altura; }
    inline double pega_Valor_tamanho_base() const { return valor_tamanho_base; }

    /// SETTERS
    inline void define_pad(Bloco *novo_bloco) { pad = novo_bloco; }
    inline void insere_vertice_elipse(int pos, Vertice v1) { vertices_elipse[pos] = v1; }
    inline void define_Pad(Bloco *pad) { this->pad = pad; }
    inline void define_Centro_x(double centro_x) { this->centro_x = centro_x; }
    inline void define_Centro_y(double centro_y) { this->centro_y = centro_y; }
    inline void define_Raio_x(double raio_x) { this->raio_x = raio_x; }
    inline void define_Raio_y(double raio_y) { this->raio_y = raio_y; }
    inline void define_Num_segmentos(int num_segmentos) { this->num_segmentos = num_segmentos; }
    inline void define_Valor_tamanho_profundidade(double valor_tamanho_profundidade) { this->valor_tamanho_profundidade = valor_tamanho_profundidade; }
    inline void define_Valor_tamanho_altura(double valor_tamanho_altura) { this->valor_tamanho_altura = valor_tamanho_altura; }
    inline void define_Valor_tamanho_base(double valor_tamanho_base) { this->valor_tamanho_base = valor_tamanho_base; }

};

#endif // PAD_H
