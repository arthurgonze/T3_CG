#include "pad.h"

/// pad será um bloco indestrutivel
Pad::Pad(Vertice *vertice_ponto, double valor_tamanho_base, double valor_tamanho_altura, double valor_tamanho_profundidade)
{
    this->valor_tamanho_altura = valor_tamanho_altura;
    this->valor_tamanho_base = valor_tamanho_base;
    this->valor_tamanho_profundidade = valor_tamanho_profundidade;
    pad = new Bloco(vertice_ponto, valor_tamanho_base, valor_tamanho_altura, valor_tamanho_profundidade, false);

    num_segmentos = 15;
    raio_x = valor_tamanho_base/2;
    raio_y = valor_tamanho_altura/2;

    monta_elipses();
}

Pad::~Pad()
{
    //dtor
}

void Pad::monta_elipses()
{
    centro_x = (pad->pega_parede_frente()->pega_triangulo_base()->pega_vertice_a()->pega_x()+
        pad->pega_parede_frente()->pega_triangulo_base()->pega_vertice_b()->pega_x())/2;

    centro_y = pad->pega_parede_traseira()->pega_triangulo_base()->pega_vertice_a()->pega_y();


    float theta = 3.1415926/float(num_segmentos);
    float cos_aux = cosf(theta);//precalculate the sine and cosine
    float sen_aux = sinf(theta);
    float t;

    float x = 1;//we start at angle = 0
    float y = 0;

    for (int i = 0; i < num_segmentos; i++)
    {
        //apply radius and offset
        vertices_elipse[i].define_x(x*raio_x + centro_x);
        vertices_elipse[i].define_y(y*raio_y + centro_y);
        vertices_elipse[i].define_z(0);

        //apply the rotation matrix
        t = x;
        x = cos_aux*x - sen_aux*y;
        y = sen_aux*t + cos_aux*y;
    }
}
