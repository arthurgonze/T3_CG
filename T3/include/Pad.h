#ifndef PAD_H
#define PAD_H

#include "Bloco.h"

class Bloco;
class Pad
{
protected:
private:
    Bloco *pad;
public:
    /// CONSTRUCTOR & DESTRUCTOR
    Pad(Vertice *verticePonto, double valorTamanhoBase, double valorTamanhoAltura, double valorTamanhoProfundidade);
    virtual ~Pad();

    /// GETTERS
    inline Bloco *getPad() { return pad; };

    /// SETTERS
    inline void setPad(Bloco *novoBloco) { pad = novoBloco; };
};

#endif // PAD_H
