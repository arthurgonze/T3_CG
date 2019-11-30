#ifndef PAD_H
#define PAD_H

#include "bloco.h"

class Bloco;
class Pad
{
protected:
private:
    Bloco *pad;
public:
    /// CONSTRUTOR & DESTRUTOR
    Pad(Vertice *vertice_ponto, double valor_tamanho_base, double valor_tamanho_altura, double valor_tamanho_profundidade);
    virtual ~Pad();

    /// GETTERS
    inline Bloco *pega_pad() { return pad; };

    /// SETTERS
    inline void define_pad(Bloco *novo_bloco) { pad = novo_bloco; };
};

#endif // PAD_H
