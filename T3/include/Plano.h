#ifndef PLANO_H
#define PLANO_H

#include "Triangulo.h"
class Triangulo;
class Plano
{
private:
    Triangulo *trianguloBase;
    Triangulo *trianguloTopo;
public:
    /// CONSTRUCTOR & DESTRUCTOR
    Plano(Triangulo *pTrianguloBase, Triangulo *pTrianguloTopo);
    virtual ~Plano();

    /// GETTERS
    inline Triangulo *getTrianguloBase() { return trianguloBase; };
    inline Triangulo *getTrianguloTopo() { return trianguloTopo; };

    /// SETTERS
    inline void setTrianguloBase(Triangulo *novoTrianguloBase) { trianguloBase = novoTrianguloBase; };
    inline void setTrianguloTopo(Triangulo *novoTrianguloTopo) { trianguloTopo = novoTrianguloTopo; };

};

#endif // PLANO_H
