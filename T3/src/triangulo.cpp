#include "triangulo.h"

// tipo:
// 0, 2 ou 4:  |     1, 3 ou 5:
// C              B--A
// |\              \ |
// | \      OR      \|
// A--B              C
Triangulo::Triangulo(Vertice *ponto_A, double p_tam_base, double p_tam_altura, int tipo)
{
    Aux aux;
    tam_base = p_tam_base;
    tam_altura = p_tam_altura;

    // nao permitir coordenadas negativas
    ponto_A->define_x(fabs(ponto_A->pega_x()));
    ponto_A->define_y(fabs(ponto_A->pega_y()));
    ponto_A->define_z(fabs(ponto_A->pega_z()));

    vertice_a = new Vertice(ponto_A->pega_x(), ponto_A->pega_y(), ponto_A->pega_z());
    vertice_b = new Vertice(ponto_A->pega_x(), ponto_A->pega_y(), ponto_A->pega_z());
    vertice_c = new Vertice(ponto_A->pega_x(), ponto_A->pega_y(), ponto_A->pega_z());

    switch (tipo)
    {
        case 0: // triangulo XY base normal pra cima
            vertice_b->define_x(vertice_b->pega_x() + tam_base);
            vertice_c->define_y(vertice_c->pega_y() + tam_altura);
            vetor_normal = aux.calcula_normal(vertice_a, vertice_b, vertice_c);
            break;
        case 1: // trinagulo XY topo normal pra cima
            vertice_b->define_x(vertice_b->pega_x() - tam_base);
            vertice_c->define_y(vertice_c->pega_y() - tam_altura);
            vetor_normal = aux.calcula_normal(vertice_a, vertice_b, vertice_c);
            break;
        case 2: // triangulo XZ base normal pra sul
            vertice_b->define_x(vertice_b->pega_x() + tam_base);
            vertice_c->define_z(vertice_c->pega_z() + tam_altura);
            vetor_normal = aux.calcula_normal(vertice_a, vertice_b, vertice_c);
            break;
        case 3: // triangulo XZ topo normal pra sul
            vertice_b->define_x(vertice_b->pega_x() - tam_base);
            vertice_c->define_z(vertice_c->pega_z() - tam_altura);
            vetor_normal = aux.calcula_normal(vertice_a, vertice_b, vertice_c);
            break;
        case 4: // triangulo YZ base normal pra leste
            vertice_b->define_y(vertice_b->pega_y() + tam_base);
            vertice_c->define_z(vertice_c->pega_z() + tam_altura);
            vetor_normal = aux.calcula_normal(vertice_a, vertice_b, vertice_c);
            break;
        case 5: // triangulo YZ topo normal pra leste
            vertice_b->define_y(vertice_b->pega_y() - tam_base);
            vertice_c->define_z(vertice_c->pega_z() - tam_altura);
            vetor_normal = aux.calcula_normal(vertice_a, vertice_b, vertice_c);
            break;
        default:
            cout << "Tipo de triangulo invalido" << endl;
            break;
    }

    pos_x = (vertice_a->pega_x() + vertice_b->pega_x() + vertice_c->pega_x())/3;
    pos_y = (vertice_a->pega_y() + vertice_b->pega_y() + vertice_c->pega_y())/3;
    pos_z = (vertice_a->pega_z() + vertice_b->pega_z() + vertice_c->pega_z())/3;
}

Triangulo::~Triangulo()
{
    //dtor
}
