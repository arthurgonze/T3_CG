#include "Bloco.h"
#include "Vertice.h"
#include "Triangulo.h"
#include "Desenha.h"

/// cubo virou bloco, pois o pad pode ser visto como um cubo que não é destrutivel (não some na colisao)
Bloco::Bloco(Vertice* verticePonto, double valorTamBase, double valorTamAltura, double valorTamProfundidade, bool blocoDestrutivel)
{
    ponto = verticePonto;
    tamBase = valorTamBase;
    tamAltura = valorTamAltura;
    tamProfundidade = valorTamProfundidade;
    destrutivel = blocoDestrutivel;
    destruido = false;
    montaBloco();
}

Bloco::~Bloco()
{
    //dtor
}

void Bloco::montaBloco()
{
    double xpto = ponto->getX();
    double ypto = ponto->getY();
    double zpto = ponto->getZ();

///       Y
///       ^     tamBase
///       | v6o------o v7
///       |  /|     /|
///       | / |    / |
///       |/  | v5/  |
///    v4 o---|--o   |tamAltura
///       |   |  |   |
///       |   |  |   |
///       |   o--|---o v3
///       |  /v2 |  /
///       | /    | / tamProfundidade
///       |/     |/
/// ponto o------o----------> X
///      /       v1
///     /
///    /
///   V
///   Z
    ///Calcular vertices bloco a partir do ponto de valores de tamanho
    Vertice* v1 = new Vertice( (xpto+tamBase), ypto, zpto );

    Vertice* v2 = new Vertice( xpto, (ypto+tamAltura), zpto );

    Vertice* v3 = new Vertice( (xpto+tamBase), (ypto+tamAltura), zpto );

    Vertice* v4 = new Vertice( xpto, ypto, (zpto+tamProfundidade) );

    Vertice* v5 = new Vertice( (xpto+tamBase), ypto, (zpto+tamProfundidade) );

    Vertice* v6 = new Vertice( xpto, (ypto+tamAltura), (zpto+tamProfundidade) );

    Vertice* v7 = new Vertice( (xpto+tamBase), (ypto+tamAltura), (zpto+tamProfundidade) );


/// PLANO FRENTE
    // Triangulo Frente Base
    //  v4
    //  o
    //  |\
    //  | \
    //  o--o v1
    //  ponto
    Triangulo* tFrenteBase = new Triangulo(ponto , tamBase, tamProfundidade, 2);

    // Triangulo Frente Topo
    //  v4 v5
    //  o--o
    //   \ |
    //    \|
    //     o v1
    Triangulo* tFrenteTopo = new Triangulo(v5 , tamBase, tamProfundidade, 3);

    paredeFrente = new Plano(tFrenteBase, tFrenteTopo);

    /// PLANO TRASEIRO
    // Triangulo Traseiro Base
    //  v6
    //  o
    //  |\
    //  | \
    //  o--o v3
    //  v2
    Triangulo* tTraseiroBase = new Triangulo(v2 , tamBase, tamProfundidade, 2);

    // Triangulo Traseiro Topo
    //  v6 v7
    //  o--o
    //   \ |
    //    \|
    //     o v3
    Triangulo* tTraseiroTopo = new Triangulo(v7 , tamBase, tamProfundidade, 3);

    paredeTraseira = new Plano(tTraseiroBase, tTraseiroTopo);

    /// PLANO NORTE
    // Triangulo Norte Base
    //  v6
    //  o
    //  |\
    //  | \
    //  o--o v5
    //  v4
    Triangulo* tNorteBase = new Triangulo(v4 , tamBase, tamAltura, 0);

    // Triangulo Norte Topo
    //  v6 v7
    //  o--o
    //   \ |
    //    \|
    //     o v5
    Triangulo* tNorteTopo = new Triangulo(v7 , tamBase, tamAltura, 1);

    paredeNorte = new Plano(tNorteBase, tNorteTopo);

    /// PLANO SUL
    // Triangulo Sul Base
    //  v2
    //  o
    //  |\
    //  | \
    //  o--o v1
    //  ponto
    Triangulo* tSulBase = new Triangulo(ponto, tamBase, tamAltura, 0);

    // Triangulo Sul Topo
    //  v2 v3
    //  o--o
    //   \ |
    //    \|
    //     o v1
    Triangulo* tSulTopo = new Triangulo(v3, tamBase, tamAltura, 1);

    paredeSul = new Plano(tSulBase, tSulTopo);

    /// PLANO LESTE
    // Triangulo Leste Base
    //  v5
    //  o
    //  |\
    //  | \
    //  o--o v3
    //  v1
    Triangulo* tLesteBase = new Triangulo(v1 , tamAltura, tamProfundidade, 4);

    // Triangulo Leste Topo
    //  v5 v7
    //  o--o
    //   \ |
    //    \|
    //     o v3
    Triangulo* tLesteTopo = new Triangulo(v7 , tamAltura, tamProfundidade, 5);

    paredeLeste = new Plano(tLesteBase, tLesteTopo);

    /// PLANO OESTE
    // Triangulo Oeste Base
    //  v4
    //  o
    //  |\
    //  | \
    //  o--o v2
    //  ponto
    Triangulo* tOesteBase = new Triangulo(ponto, tamAltura, tamProfundidade, 4);

    // Triangulo Oeste Topo
    //  v4 v6
    //  o--o
    //   \ |
    //    \|
    //     o v2
    Triangulo* tOesteTopo = new Triangulo(v6 , tamAltura, tamProfundidade, 5);

    paredeOeste = new Plano(tOesteBase, tOesteTopo);

}

Vertice Bloco::getPos()
{
    double xPos, yPos, zPos;

    xPos = ((paredeSul->getTrianguloBase()->getVerticeA()->getX()+paredeSul->getTrianguloBase()->getVerticeB()->getX())/2);
    yPos = ((paredeSul->getTrianguloBase()->getVerticeA()->getY()+paredeSul->getTrianguloBase()->getVerticeC()->getY())/2);
    zPos = ((paredeSul->getTrianguloBase()->getVerticeA()->getZ()+paredeLeste->getTrianguloBase()->getVerticeC()->getZ())/2);

    return Vertice(xPos, yPos, zPos);
}
