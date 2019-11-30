#include "aux.h"
#include "math.h"

Aux::Aux()
{

}

Aux::~Aux()
{
    //dtor
}

/* Exemplo de cálculo de vetor normal que são definidos a partir dos vértices do triângulo;
     v_2
      ^
      |\
      | \
      |  \       'vn' é o vetor normal resultante
      |   \
      +----> v_1
    v_0 */
Vertice *Aux::calcula_normal(Vertice *vertice_a, Vertice *vertice_b, Vertice *vertice_c)
{
    Vertice *v_0 = vertice_a;
    Vertice *v_1 = vertice_b;
    Vertice *v_2 = vertice_c;
    double len;

    // Set Vector U to (Triangle.p2 minus Triangle.p1)
    /// Encontra vetor v1
    Vertice *v1 = new Vertice(
        (v_1->pega_x() - v_0->pega_x()),
        (v_1->pega_y() - v_0->pega_y()),
        (v_1->pega_z() - v_0->pega_z())
    );

    // Set Vector V to (Triangle.p3 minus Triangle.p1)
    /// Encontra vetor v2
    Vertice *v2 = new Vertice(
        (v_2->pega_x() - v_0->pega_x()),
        (v_2->pega_y() - v_0->pega_y()),
        (v_2->pega_z() - v_0->pega_z())
    );

    /// Calculo do produto vetorial de v1 e v2
    Vertice *vn = new Vertice(
        //	Set Normal.x to (multiply U.y by V.z) minus (multiply U.z by V.y)
        (v1->pega_y()*v2->pega_z()) - (v1->pega_z()*v2->pega_y()),
        //	Set Normal.y to (multiply U.z by V.x) minus (multiply U.x by V.z)
        (v1->pega_z()*v2->pega_x()) - (v1->pega_x()*v2->pega_z()),
        //	Set Normal.z to (multiply U.x by V.y) minus (multiply U.y by V.x)
        (v1->pega_x()*v2->pega_y()) - (v1->pega_y()*v2->pega_x())
    );

    /// normalizacao de n
    len = sqrt(pow(vn->pega_x(), 2) + pow(vn->pega_y(), 2) + pow(vn->pega_z(), 2));

    vn->define_x(vn->pega_x()/len);
    vn->define_y(vn->pega_y()/len);
    vn->define_z(vn->pega_z()/len);

    // Returning Normalized Normal
    return vn;
}

int Aux::detecta_colisao(Esfera *esfera, Esfera *esfera_colisao, Bloco ***matriz, Tabuleiro *tabuleiro, Pad *pad, GameController *game_controller,
                         int blocos_col, int blocos_lin, double velocidade_esfera, double fps, bool destroy)
{
    Vertice proxPos((esfera->pega_posicao()->pega_x() + (velocidade_esfera/fps)*esfera->pega_direcao()->pega_x()),
                    (esfera->pega_posicao()->pega_y() + (velocidade_esfera/fps)*esfera->pega_direcao()->pega_y()),
                    (esfera->pega_posicao()->pega_z()));

    double minX, maxX, minY, maxY, minZ, maxZ, x, y, z, distance;
    /// COLISAO COM OS BLOCOS DO TOPO DO TABULEIRO
    for (int i = 0; i < blocos_col; i++)
    {
        for (int j = 0; j < blocos_lin; j++)
        {
            minX = matriz[i][j]->pega_parede_sul()->pega_triangulo_base()->pega_vertice_a()->pega_x();
            maxX = matriz[i][j]->pega_parede_sul()->pega_triangulo_base()->pega_vertice_b()->pega_x();

            minY = matriz[i][j]->pega_parede_sul()->pega_triangulo_base()->pega_vertice_a()->pega_y();
            maxY = matriz[i][j]->pega_parede_sul()->pega_triangulo_base()->pega_vertice_c()->pega_y();

            minZ = matriz[i][j]->pega_parede_leste()->pega_triangulo_base()->pega_vertice_a()->pega_z();
            maxZ = matriz[i][j]->pega_parede_leste()->pega_triangulo_base()->pega_vertice_c()->pega_z();

            // get box closest point to sphere center by clamping
            x = max(minX, min(proxPos.pega_x(), maxX));
            y = max(minY, min(proxPos.pega_y(), maxY));
            z = max(minZ, min(proxPos.pega_z(), maxZ));

            distance = sqrt(pow((x - proxPos.pega_x()), 2) +
                pow((y - proxPos.pega_y()), 2) +
                pow((z - proxPos.pega_z()), 2)
            );

            // Testa Colisao
            if (distance < esfera->pega_raio() && !matriz[i][j]->pega_destruido())
            {
                // descobrir que face a esfera colidiu
                // parede Leste?
                if (proxPos.pega_x() >= maxX && (proxPos.pega_y() >= minY && proxPos.pega_y() <= maxY))
                {
                    resolve_colisao(esfera, matriz[i][j]->pega_parede_leste()->pega_triangulo_base());
                }

                // parede Oeste?
                if (proxPos.pega_x() <= minX && (proxPos.pega_y() >= minY && proxPos.pega_y() <= maxY))
                {
                    resolve_colisao(esfera, matriz[i][j]->pega_parede_oeste()->pega_triangulo_base());
                }

                // parede frente
                if (proxPos.pega_y() <= minY && (proxPos.pega_x() >= minX && proxPos.pega_x() <= maxX))
                {
                    resolve_colisao(esfera, matriz[i][j]->pega_parede_frente()->pega_triangulo_base());
                }

                // parede traseira
                if (proxPos.pega_y() >= maxY && (proxPos.pega_x() >= minX && proxPos.pega_x() <= maxX))
                {
                    resolve_colisao(esfera, matriz[i][j]->pega_parede_traseira()->pega_triangulo_base());
                }

                if (destroy)
                {
                    matriz[i][j]->define_destruido(true);
                }
            }
        }
    }

    /// COLISAO COM AS PARADES DO TABULEIRO
    // Norte
    if (proxPos.pega_y() >= tabuleiro->pega_parede_norte()->pega_triangulo_base()->pega_vertice_a()->pega_y() - esfera->pega_raio())
    {
        resolve_colisao(esfera, tabuleiro->pega_parede_norte()->pega_triangulo_base());
    }
    // Sul
    if (proxPos.pega_y() <= tabuleiro->pega_parede_sul()->pega_triangulo_base()->pega_vertice_a()->pega_y() - (esfera->pega_raio()*4))
    {
//        resolve_colisao(esfera, tabuleiro->pega_parede_sul()->pega_triangulo_base());
        if (destroy)
        {
            game_controller->perde_vida();
            game_controller->continua_fase(esfera, pad);
            return 2;
        }
        else if (esfera_colisao!=nullptr)
        {
            if (esfera_colisao->pega_spawn()==1)
            {
                game_controller->define_spawn1_fora(true);
                return 1;
            };

            if (esfera_colisao->pega_spawn()==2)
            {
                game_controller->define_spawn2_fora(true);
                return 1;
            };
        }
    }
    // Leste
    if (proxPos.pega_x() >= tabuleiro->pega_parede_leste()->pega_triangulo_base()->pega_vertice_a()->pega_x() - esfera->pega_raio())
    {
        resolve_colisao(esfera, tabuleiro->pega_parede_leste()->pega_triangulo_base());
    }
    // Oeste
    if (proxPos.pega_x() <= tabuleiro->pega_parede_oeste()->pega_triangulo_base()->pega_vertice_a()->pega_x() + esfera->pega_raio())
    {
        resolve_colisao(esfera, tabuleiro->pega_parede_oeste()->pega_triangulo_base());
    }


    /// COLISAO COM O PAD DO JOGADOR
    minX = pad->pega_pad()->pega_parede_sul()->pega_triangulo_base()->pega_vertice_a()->pega_x();
    maxX = pad->pega_pad()->pega_parede_sul()->pega_triangulo_base()->pega_vertice_b()->pega_x();

    minY = pad->pega_pad()->pega_parede_sul()->pega_triangulo_base()->pega_vertice_a()->pega_y();
    maxY = pad->pega_pad()->pega_parede_sul()->pega_triangulo_base()->pega_vertice_c()->pega_y();

    minZ = pad->pega_pad()->pega_parede_leste()->pega_triangulo_base()->pega_vertice_a()->pega_z();
    maxZ = pad->pega_pad()->pega_parede_leste()->pega_triangulo_base()->pega_vertice_c()->pega_z();

    // get box closest point to sphere center by clamping
    x = max(minX, min(proxPos.pega_x(), maxX));
    y = max(minY, min(proxPos.pega_y(), maxY));
    z = max(minZ, min(proxPos.pega_z(), maxZ));

    distance = sqrt(pow((x - proxPos.pega_x()), 2) +
        pow((y - proxPos.pega_y()), 2) +
        pow((z - proxPos.pega_z()), 2)
    );

    // Testa Colisao
    if (distance < esfera->pega_raio())
    {
        // descobrir que face a esfera colidiu
        // parede Leste?
        if (proxPos.pega_x() >= maxX && (proxPos.pega_y() >= minY && proxPos.pega_y() <= maxY))
        {
            resolve_colisao(esfera, pad->pega_pad()->pega_parede_leste()->pega_triangulo_base());
        }

        // parede Oeste?
        if (proxPos.pega_x() <= minX && (proxPos.pega_y() >= minY && proxPos.pega_y() <= maxY))
        {
            resolve_colisao(esfera, pad->pega_pad()->pega_parede_oeste()->pega_triangulo_base());
        }

        // parede frente
        if (proxPos.pega_y() <= minY && (proxPos.pega_x() >= minX && proxPos.pega_x() <= maxX))
        {
            resolve_colisao(esfera, pad->pega_pad()->pega_parede_frente()->pega_triangulo_base());
        }

        // parede traseira
        if (proxPos.pega_y() >= maxY && (proxPos.pega_x() >= minX && proxPos.pega_x() <= maxX))
        {
            resolve_colisao(esfera, pad->pega_pad()->pega_parede_traseira()->pega_triangulo_base());
        }
    }

    /// COLISAO ESFERA X ESFERA(SPAWNED OBJECTS)
    if (esfera_colisao!=nullptr)
    {
        // a-> esfera, b->esfera_colisao
        // vector between the centers of each sphere
        Vertice *s = new Vertice(esfera->pega_posicao()->pega_x() - esfera_colisao->pega_posicao()->pega_x(),
                                 esfera->pega_posicao()->pega_y() - esfera_colisao->pega_posicao()->pega_y(),
                                 esfera->pega_posicao()->pega_z() - esfera_colisao->pega_posicao()->pega_z()
        );

        // relative velocity between spheres
        Vertice proxPos2((esfera_colisao->pega_posicao()->pega_x() + (velocidade_esfera/fps)*esfera_colisao->pega_direcao()->pega_x()),
                         (esfera_colisao->pega_posicao()->pega_y() + (velocidade_esfera/fps)*esfera_colisao->pega_direcao()->pega_y()),
                         (esfera_colisao->pega_posicao()->pega_z()));

        Vertice *v = new Vertice(proxPos.pega_x() - proxPos2.pega_x(),
                                 proxPos.pega_y() - proxPos2.pega_y(),
                                 proxPos.pega_z() - proxPos2.pega_z()
        );

        double sumRadius = esfera->pega_raio() + esfera_colisao->pega_raio();

        double c1 = (pow(v->pega_x(), 2) + pow(v->pega_y(), 2) + pow(v->pega_z(), 2)) - pow(sumRadius, 2); // if negative, they overlap
        if (c1 < esfera_colisao->pega_raio()) // if true, they already overlap
        {
            resolve_colisao(esfera, esfera_colisao);
            if (esfera_colisao->pega_spawn()==0)
            {
                if (esfera->pega_spawn()==1)
                    game_controller->define_spawn1_fora(true);
                if (esfera->pega_spawn()==2)
                    game_controller->define_spawn2_fora(true);
            }
        }
    }

    return 0;
}
void Aux::resolve_colisao(Esfera *esfera1, Esfera *esfera2)
{
    double len1 = sqrt(pow(esfera1->pega_direcao()->pega_x(), 2) +
        pow(esfera1->pega_direcao()->pega_y(), 2) +
        pow(esfera1->pega_direcao()->pega_z(), 2));

    double len2 = sqrt(pow(esfera2->pega_direcao()->pega_x(), 2) +
        pow(esfera2->pega_direcao()->pega_y(), 2) +
        pow(esfera2->pega_direcao()->pega_z(), 2));

    esfera1->define_direcao(new Vertice(
        esfera1->pega_direcao()->pega_x()/len1,
        esfera1->pega_direcao()->pega_y()/len1,
        esfera1->pega_direcao()->pega_z()/len1));

    esfera2->define_direcao(new Vertice(
        esfera2->pega_direcao()->pega_x()/len2,
        esfera2->pega_direcao()->pega_y()/len2,
        esfera2->pega_direcao()->pega_z()/len2));

    // r=e−2(e⋅n)n
    double prodEsc = (esfera2->pega_direcao()->pega_x()*esfera1->pega_direcao()->pega_x()) +
        (esfera2->pega_direcao()->pega_y()*esfera1->pega_direcao()->pega_y()) +
        (esfera2->pega_direcao()->pega_z()*esfera1->pega_direcao()->pega_z());

    prodEsc = 2*prodEsc; // 2(e.n)
    Vertice *v = new Vertice(prodEsc*esfera2->pega_direcao()->pega_x(),   //2(e.n)n
                             prodEsc*esfera2->pega_direcao()->pega_y(),
                             prodEsc*esfera2->pega_direcao()->pega_z());

    Vertice *r1 = new Vertice(esfera1->pega_direcao()->pega_x() - v->pega_x(),
                              esfera1->pega_direcao()->pega_y() - v->pega_y(),
                              esfera1->pega_direcao()->pega_z() - v->pega_z());

    Vertice *r2 = new Vertice(-r1->pega_x(),
                              -r1->pega_x(),
                              -r1->pega_x());

    esfera1->define_direcao(r1);
    esfera2->define_direcao(r2);

}

void Aux::resolve_colisao(Esfera *esfera, Triangulo *triangulo)
{
    // r=e−2(e⋅n)n
    double prodEsc = (triangulo->pega_vetor_normal()->pega_x()*esfera->pega_direcao()->pega_x()) +
        (triangulo->pega_vetor_normal()->pega_y()*esfera->pega_direcao()->pega_y()) +
        (triangulo->pega_vetor_normal()->pega_z()*esfera->pega_direcao()->pega_z());

    prodEsc = 2*prodEsc; // 2(e.n)
    Vertice *v = new Vertice(prodEsc*triangulo->pega_vetor_normal()->pega_x(),   //2(e.n)n
                             prodEsc*triangulo->pega_vetor_normal()->pega_y(),
                             prodEsc*triangulo->pega_vetor_normal()->pega_z());

    Vertice *r = new Vertice(esfera->pega_direcao()->pega_x() - v->pega_x(),
                             esfera->pega_direcao()->pega_y() - v->pega_y(),
                             esfera->pega_direcao()->pega_z() - v->pega_z());
    esfera->define_direcao(r);
}
