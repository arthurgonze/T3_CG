#ifndef TABULEIRO_H
#define TABULEIRO_H

#include "Triangulo.h"
#include "Vertice.h"
#include "Triangulo.h"
#include "Plano.h"

class Plano;
class Tabuleiro
{
private:
    Plano* base;
    Plano* paredeNorte;
    Plano* paredeSul;
    Plano* paredeLeste;
    Plano* paredeOeste;
    Vertice* pos;
    double tamBase;
    double tamAltura;
    double tamAlturaParedes;

    void montaTabuleiro(); /// funcao para setar os planos do tabuleiro a partir das informacoes do construtor
public:
    /// CONSTRUCTOR & DESTRUCTOR
    Tabuleiro(Vertice* posicaoInicial, double pTamBase, double pTamAltura, double pTamAlturaParedes);
    virtual ~Tabuleiro();

    /// GETTERS
    inline Plano* getBase(){return base;};
    inline Plano* getParedeNorte(){return paredeNorte;};
    inline Plano* getParedeLeste(){return paredeLeste;};
    inline Plano* getParedeSul(){return paredeSul;};
    inline Plano* getParedeOeste(){return paredeOeste;};
    inline Vertice* getPosicaoInicial(){return pos;};
    inline double getTamBase(){return tamBase;};
    inline double getTamAltura(){return tamAltura;};
    inline double getTamAlturaParedes(){return tamAlturaParedes;};

    /// SETTERS
    inline void setBase(Plano* novaBase){ base = novaBase;};
    inline void setParedeNorte(Plano* novaParedeNorte){ paredeNorte = novaParedeNorte;};
    inline void setParedeLeste(Plano* novaParedeLeste){ paredeLeste = novaParedeLeste;};
    inline void setParedeSul(Plano* novaParedeSul){ paredeSul = novaParedeSul;};
    inline void setParedeOeste(Plano* novaParedeOeste){ paredeOeste = novaParedeOeste;};
    inline void setPosicao(Vertice* novaPosicao){pos = novaPosicao;};
    inline void setTamBase(double novoTamBase){tamBase = novoTamBase;};
    inline void setTamAltura(double novoTamAltura){tamAltura = novoTamAltura;};
    inline void setTamAlturaParedes(double novoTamAlturaParedes){tamAlturaParedes = novoTamAlturaParedes;};
};

#endif // TABULEIRO_H
