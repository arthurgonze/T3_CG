#ifndef TRIANGULO_H
#define TRIANGULO_H

#include <iostream>
#include <cmath>
#include "vertice.h"
#include "aux.h"

using namespace std;
class Triangulo
{
    // tipo:
    // 0, 2 ou 4:  |     1, 3 ou 5:
    // C           |     B--A
    // |\          OR     \ |
    // | \         |       \|
    // A--B        |        C
    // tipo 0: tam_base aplicado ao eixo X e tam_altura aplicado ao eixo Y
    // tipo 1: tam_base aplicado ao eixo X e tam_altura aplicado ao eixo Y
    // tipo 2: tam_base aplicado ao eixo X e tam_altura aplicado ao eixo Z
    // tipo 3: tam_base aplicado ao eixo X e tam_altura aplicado ao eixo Z
    // tipo 4: tam_base aplicado ao eixo Y e tam_altura aplicado ao eixo Z
    // tipo 5: tam_base aplicado ao eixo Y e tam_altura aplicado ao eixo Z

private:
    Vertice *vertice_a;
    Vertice *vertice_b;
    Vertice *vertice_c;
    Vertice *vetor_normal;
    double pos_x, pos_y, pos_z;
    double tam_base; // B-A ou A-C
    double tam_altura; // A-B ou C-A
    int tipo;
public:
    /// CONSTRUTOR & DESTRUTOR
    Triangulo(Vertice *ponto_a, double p_tam_base, double p_tam_altura, int tipo);
    virtual ~Triangulo();

    /// GETTERS
    inline Vertice *pega_vertice_a() { return vertice_a; };
    inline Vertice *pega_vertice_b() { return vertice_b; };
    inline Vertice *pega_vertice_c() { return vertice_c; };
    inline Vertice *pega_vetor_normal() { return vetor_normal; };
    inline double pega_tam_base() { return tam_base; };
    inline double pega_tam_altura() { return tam_altura; };
    inline int pega_tipo() { return tipo; }
    inline double pega_x() { return pos_x; }
    inline double pega_y() { return pos_y; }
    inline double pega_z() { return pos_z; }

    /// SETTERS
    inline void define_vertice_a(Vertice *ponto_a) { vertice_a = ponto_a; };
    inline void define_vertice_b(Vertice *ponto_b) { vertice_b = ponto_b; };
    inline void define_vertice_c(Vertice *ponto_c) { vertice_c = ponto_c; };
    inline void define_vetor_normal(Vertice *nova_normal) { vetor_normal = nova_normal; };
    inline void define_tam_base(double novo_tam_base) { tam_base = novo_tam_base; };
    inline void define_tam_altura(double novo_tam_altura) { tam_altura = novo_tam_altura; };
    inline void define_tipo(int novo_tipo) { tipo = novo_tipo; };
};

#endif // TRIANGULO_H
