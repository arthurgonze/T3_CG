#ifndef BLOCO_H
#define BLOCO_H

#include "Vertice.h"
#include "Plano.h"


//       Y
//       ^     tamBase
//       | v6o------o v7
//       |  /|     /|
//       | / |    / |
//       |/  | v5/  |
//    v4 o---|--o   |tamAltura
//       |   |  |   |
//       |   |  |   |
//       |   o--|---o v3
//       |  /v2 |  /
//       | /    | / tamProfundidade
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
    double tamBase;
    double tamAltura;
    double tamProfundidade;
    bool destrutivel; // blocos e pad utilizarão o mesmo tad, entao esse booleano pode ajudar, já que o pad não pode ser destruído
    bool destruido;
    Vertice* ponto;
    Plano* paredeNorte;
    Plano* paredeLeste;
    Plano* paredeSul;
    Plano* paredeOeste;
    Plano* paredeFrente;
    Plano* paredeTraseira;

    void montaBloco();

public:
    /// CONSTRUCTOR & DESTRUCTOR
    Bloco();
    Bloco(Vertice* verticePonto, double valorTamBase, double valorTamAltura, double valorTamProfundidade, bool blocoDestrutivel);
    virtual ~Bloco();

    /// GETTERS
    inline Plano* getParedeNorte() {return paredeNorte;};
    inline Plano* getParedeLeste(){return paredeLeste;};
    inline Plano* getParedeSul(){return paredeSul;};
    inline Plano* getParedeOeste(){return paredeOeste;};
    inline Plano* getParedeFrente(){return paredeFrente;};
    inline Plano* getParedeTraseira(){return paredeTraseira;};
    inline Vertice* getVertice(){return ponto;};
    inline double getTamBase(){return tamBase;};
    inline double getTamAltura(){return tamAltura;};
    inline double getTamProfundidade(){return tamProfundidade;};
    inline bool getDestrutivel(){return destrutivel;};
    inline bool getDestruido(){return destruido;};
    Vertice getPos();

    /// SETTERS
    inline void setParedeNorte(Plano* parede){paredeNorte = parede;};
    inline void setParedeLeste(Plano* parede){paredeLeste = parede;};
    inline void setParedeSul(Plano* parede){paredeSul = parede;};
    inline void setParedeOeste(Plano* parede){paredeOeste = parede;};
    inline void setParedeFrente(Plano* parede){paredeFrente = parede;};
    inline void setParedeTraseira(Plano* parede){paredeTraseira = parede;};
    inline void setVertice(Vertice* verticePonto){ponto = verticePonto;montaBloco();};
    inline void setTamBase(double valorTamBase){tamBase = valorTamBase;};
    inline void setTamAltura(double valorTamAltura){tamAltura = valorTamAltura;};
    inline void setTamProfundidade(double valorTamProfundidade){tamProfundidade = valorTamProfundidade;};
    inline void alteraDestrutivel(){destrutivel = !destrutivel;};
    inline void setDestruido(bool toggle){destruido=toggle;};
};

#endif // bloco_H
