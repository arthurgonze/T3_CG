#ifndef PLANO_H
#define PLANO_H

#include "triangulo.h"

class Triangulo;
class Plano
{
private:
    Triangulo *triangulo_base;
    Triangulo *triangulo_topo;
public:
    /// CONSTRUCTOR & DESTRUCTOR
    Plano(Triangulo *p_triangulo_base, Triangulo *p_triangulo_topo);
    virtual ~Plano();

    /// GETTERS
    inline Triangulo *pega_triangulo_base() { return triangulo_base; };
    inline Triangulo *pega_triangulo_topo() { return triangulo_topo; };

    /// SETTERS
    inline void define_triangulo_base(Triangulo *novo_triangulo_base) { triangulo_base = novo_triangulo_base; };
    inline void define_triangulo_topo(Triangulo *novo_triangulo_topo) { triangulo_topo = novo_triangulo_topo; };

};

#endif // PLANO_H
