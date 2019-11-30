#include "bloco.h"


/// cubo virou bloco, pois o pad pode ser visto como um cubo que não é destrutivel (não some na colisao)
Bloco::Bloco(Vertice *vertice_ponto, double valor_tam_base, double valor_tam_altura, double valor_tam_profundidade, bool bloco_destrutivel)
{
    ponto = vertice_ponto;
    tam_base = valor_tam_base;
    tam_altura = valor_tam_altura;
    tam_profundidade = valor_tam_profundidade;
    destrutivel = bloco_destrutivel;
    destruido = false;
    monta_bloco();
}

Bloco::~Bloco()
{
    //dtor
}

void Bloco::monta_bloco()
{
    double xpto = ponto->pega_x();
    double ypto = ponto->pega_y();
    double zpto = ponto->pega_z();

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
    Vertice *v1 = new Vertice((xpto + tam_base), ypto, zpto);

    Vertice *v2 = new Vertice(xpto, (ypto + tam_altura), zpto);

    Vertice *v3 = new Vertice((xpto + tam_base), (ypto + tam_altura), zpto);

    Vertice *v4 = new Vertice(xpto, ypto, (zpto + tam_profundidade));

    Vertice *v5 = new Vertice((xpto + tam_base), ypto, (zpto + tam_profundidade));

    Vertice *v6 = new Vertice(xpto, (ypto + tam_altura), (zpto + tam_profundidade));

    Vertice *v7 = new Vertice((xpto + tam_base), (ypto + tam_altura), (zpto + tam_profundidade));


/// PLANO FRENTE
    /* Triangulo Frente Base
      v4
      o
      |\
      | \
      o--o v1
      ponto
      */
    Triangulo *tFrenteBase = new Triangulo(ponto, tam_base, tam_profundidade, 2);

    /*Triangulo Frente Topo
     v4 v5
     o--o
      \ |
       \|
        o v1
        */
    Triangulo *tFrenteTopo = new Triangulo(v5, tam_base, tam_profundidade, 3);

    parede_frente = new Plano(tFrenteBase, tFrenteTopo);

    /// PLANO TRASEIRO
    /* Triangulo Traseiro Base
      v6
      o
      |\
      | \
      o--o v3
      v2
      */
    Triangulo *tTraseiroBase = new Triangulo(v2, tam_base, tam_profundidade, 2);

    /* Triangulo Traseiro Topo
      v6 v7
      o--o
       \ |
        \|
         o v3
         */
    Triangulo *tTraseiroTopo = new Triangulo(v7, tam_base, tam_profundidade, 3);

    parede_traseira = new Plano(tTraseiroBase, tTraseiroTopo);

    /// PLANO NORTE
    /* Triangulo Norte Base
      v6
      o
      |\
      | \
      o--o v5
      v4
      */
    Triangulo *tNorteBase = new Triangulo(v4, tam_base, tam_altura, 0);

    /* Triangulo Norte Topo
      v6 v7
      o--o
       \ |
        \|
         o v5
         */
    Triangulo *tNorteTopo = new Triangulo(v7, tam_base, tam_altura, 1);

    parede_norte = new Plano(tNorteBase, tNorteTopo);

    /// PLANO SUL
     /*Triangulo Sul Base
      v2
      o
      |\
      | \
      o--o v1
      ponto
      */
    Triangulo *tSulBase = new Triangulo(ponto, tam_base, tam_altura, 0);

    /* Triangulo Sul Topo
      v2 v3
      o--o
       \ |
        \|
         o v1
         */
    Triangulo *tSulTopo = new Triangulo(v3, tam_base, tam_altura, 1);

    parede_sul = new Plano(tSulBase, tSulTopo);

    /// PLANO LESTE
     /*Triangulo Leste Base
      v5
      o
      |\
      | \
      o--o v3
      v1
      */
    Triangulo *tLesteBase = new Triangulo(v1, tam_altura, tam_profundidade, 4);

     /*Triangulo Leste Topo
      v5 v7
      o--o
       \ |
        \|
         o v3
         */
    Triangulo *tLesteTopo = new Triangulo(v7, tam_altura, tam_profundidade, 5);

    parede_leste = new Plano(tLesteBase, tLesteTopo);

    /// PLANO OESTE
     /*Triangulo Oeste Base
      v4
      o
      |\
      | \
      o--o v2
      ponto*/

    Triangulo *tOesteBase = new Triangulo(ponto, tam_altura, tam_profundidade, 4);

     /*Triangulo Oeste Topo
      v4 v6
      o--o
       \ |
        \|
         o v2
         */
    Triangulo *tOesteTopo = new Triangulo(v6, tam_altura, tam_profundidade, 5);

    parede_oeste = new Plano(tOesteBase, tOesteTopo);

}

Vertice Bloco::pega_pos()
{
    double xPos, yPos, zPos;

    xPos = ((parede_sul->pega_triangulo_base()->pega_vertice_a()->pega_x() + parede_sul->pega_triangulo_base()->pega_vertice_b()->pega_x())/2);
    yPos = ((parede_sul->pega_triangulo_base()->pega_vertice_a()->pega_y() + parede_sul->pega_triangulo_base()->pega_vertice_c()->pega_y())/2);
    zPos = ((parede_sul->pega_triangulo_base()->pega_vertice_a()->pega_z() + parede_leste->pega_triangulo_base()->pega_vertice_c()->pega_z())/2);

    return Vertice(xPos, yPos, zPos);
}
