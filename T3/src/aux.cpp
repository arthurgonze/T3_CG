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

int Aux::detecta_colisao_esfera_tabuleiro(Esfera *esfera, Tabuleiro *tabuleiro, GameController *game_controller, Pad* pad, bool destroy, glcSound *som)
{
    Vertice proxPos((esfera->pega_posicao()->pega_x() + (game_controller->pega_vel_esfera()/game_controller->pega_fps_desejado())*esfera->pega_direcao()->pega_x()),
                    (esfera->pega_posicao()->pega_y() + (game_controller->pega_vel_esfera()/game_controller->pega_fps_desejado())*esfera->pega_direcao()->pega_y()),
                    (esfera->pega_posicao()->pega_z()));

    /// COLISAO COM AS PARADES DO TABULEIRO
    // Norte
    if (proxPos.pega_y() >= tabuleiro->pega_parede_norte()->pega_triangulo_base()->pega_vertice_a()->pega_y() - esfera->pega_raio())
    {
        resolve_colisao(esfera, tabuleiro->pega_parede_norte()->pega_triangulo_base());
    }
    // Sul
    if (proxPos.pega_y() <= tabuleiro->pega_parede_sul()->pega_triangulo_base()->pega_vertice_a()->pega_y() - (esfera->pega_raio()*4))
    {
        if (destroy)
        {
            game_controller->perde_vida();
            game_controller->continua_fase(esfera, pad);
            return 2;
        }
        else
        {
            if (esfera->pega_spawn()==1)
            {
                game_controller->define_spawn1_fora(true);
                return 1;
            }

            if (esfera->pega_spawn()==2)
            {
                game_controller->define_spawn2_fora(true);
                return 1;
            }
        }

    }

    // Leste
    if (detecta_colisao_esfera_elipse(tabuleiro->pega_Centro_x_leste(), tabuleiro->pega_Centro_y_leste(), tabuleiro->pega_Raio_y(),
        tabuleiro->pega_Raio_x(), proxPos.pega_x(), proxPos.pega_y(), esfera->pega_raio()))
    {
        resolve_colisao(esfera, tabuleiro, true);
    }


    // Oeste
    if (detecta_colisao_esfera_elipse(tabuleiro->pega_Centro_x_oeste(), tabuleiro->pega_Centro_y_oeste(), tabuleiro->pega_Raio_y(),
        tabuleiro->pega_Raio_x(), proxPos.pega_x(), proxPos.pega_y(), esfera->pega_raio()))
    {
        resolve_colisao(esfera, tabuleiro, false);
    }
    return 0;
}

int Aux::detecta_colisao_esfera_blocos(Esfera *esfera, Bloco ***matriz, GameController *game_controller, bool destroy, glcSound *som)
{
    Vertice proxPos((esfera->pega_posicao()->pega_x() + (game_controller->pega_vel_esfera()/game_controller->pega_fps_desejado())*esfera->pega_direcao()->pega_x()),
                    (esfera->pega_posicao()->pega_y() + (game_controller->pega_vel_esfera()/game_controller->pega_fps_desejado())*esfera->pega_direcao()->pega_y()),
                    (esfera->pega_posicao()->pega_z()));

    double minX, maxX, minY, maxY, minZ, maxZ, x, y, z, distance;

    /// COLISAO COM OS BLOCOS DO TOPO DO TABULEIRO
    for (int i = 0; i < game_controller->pega_num_blocos_coluna_matriz(); i++)
    {
        for (int j = 0; j < game_controller->pega_num_blocos_linha_matriz(); j++)
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
    return 0;
}

int Aux::detecta_colisao_esfera_objetos_importados(Esfera *esfera, Esfera *esfera_colisao, GameController *game_controller, glcSound *som)
{
    Vertice proxPos((esfera_colisao->pega_posicao()->pega_x() + (game_controller->pega_vel_esfera()/game_controller->pega_fps_desejado())*esfera_colisao->pega_direcao()->pega_x()),
                    (esfera_colisao->pega_posicao()->pega_y() + (game_controller->pega_vel_esfera()/game_controller->pega_fps_desejado())*esfera_colisao->pega_direcao()->pega_y()),
                    (esfera->pega_posicao()->pega_z()));

    /// COLISAO ESFERA X ESFERA(SPAWNED OBJECTS)
    if (esfera!=nullptr)
    {
        // a-> esfera_colisao, b->esfera
        // vector between the centers of each sphere
        Vertice *s = new Vertice(esfera_colisao->pega_posicao()->pega_x() - esfera->pega_posicao()->pega_x(),
                                 esfera_colisao->pega_posicao()->pega_y() - esfera->pega_posicao()->pega_y(),
                                 esfera_colisao->pega_posicao()->pega_z() - esfera->pega_posicao()->pega_z()
        );

        // relative velocity between spheres
        Vertice proxPos2((esfera->pega_posicao()->pega_x() + (game_controller->pega_vel_esfera()/game_controller->pega_fps_desejado())*esfera->pega_direcao()->pega_x()),
                         (esfera->pega_posicao()->pega_y() + (game_controller->pega_vel_esfera()/game_controller->pega_fps_desejado())*esfera->pega_direcao()->pega_y()),
                         (esfera->pega_posicao()->pega_z()));

        Vertice *v = new Vertice(proxPos.pega_x() - proxPos2.pega_x(),
                                 proxPos.pega_y() - proxPos2.pega_y(),
                                 proxPos.pega_z() - proxPos2.pega_z()
        );

        double sumRadius = esfera_colisao->pega_raio() + esfera->pega_raio();

        double c1 = (pow(v->pega_x(), 2) + pow(v->pega_y(), 2) + pow(v->pega_z(), 2)) - pow(sumRadius, 2); // if negative, they overlap
        if (c1 < esfera->pega_raio()) // if true, they already overlap
        {
            resolve_colisao(esfera_colisao, esfera);
            if (esfera->pega_spawn() == 0)
            {
                if (esfera_colisao->pega_spawn()==1)
                    game_controller->define_spawn1_fora(true);
                if (esfera_colisao->pega_spawn()==2)
                    game_controller->define_spawn2_fora(true);
            }
        }
    }

    return 0;
}

int Aux::detecta_colisao_esfera_rebatedor(Esfera *esfera, Pad *pad, GameController *game_controller, glcSound *som)
{
    Vertice proxPos((esfera->pega_posicao()->pega_x() + (game_controller->pega_vel_esfera()/game_controller->pega_fps_desejado())*esfera->pega_direcao()->pega_x()),
                    (esfera->pega_posicao()->pega_y() + (game_controller->pega_vel_esfera()/game_controller->pega_fps_desejado())*esfera->pega_direcao()->pega_y()),
                    (esfera->pega_posicao()->pega_z()));

    double minX, maxX, minY, maxY, minZ, maxZ, x, y, z, distance;

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

    if (detecta_colisao_esfera_elipse(pad->pega_Centro_x(), pad->pega_Centro_y(), pad->pega_Raio_x(),
                                      pad->pega_Raio_y(),esfera->pega_posicao()->pega_x(),
                                      esfera->pega_posicao()->pega_y(), esfera->pega_raio()))
    {
        som->PlaySound(0);
        resolve_colisao(esfera, pad);
    }

    // Testa Colisao
    if (distance < esfera->pega_raio())
    {
        // descobrir que face a esfera colidiu
        // parede Leste?
        if (proxPos.pega_x() >= maxX && (proxPos.pega_y() >= minY && proxPos.pega_y() <= maxY))
        {
            som->PlaySound(0);
            resolve_colisao(esfera, pad->pega_pad()->pega_parede_leste()->pega_triangulo_base());
        }

        // parede Oeste?
        if (proxPos.pega_x() <= minX && (proxPos.pega_y() >= minY && proxPos.pega_y() <= maxY))
        {
            som->PlaySound(0);
            resolve_colisao(esfera, pad->pega_pad()->pega_parede_oeste()->pega_triangulo_base());
        }

        // parede frente
        if (proxPos.pega_y() <= minY && (proxPos.pega_x() >= minX && proxPos.pega_x() <= maxX))
        {
            som->PlaySound(0);
            resolve_colisao(esfera, pad->pega_pad()->pega_parede_frente()->pega_triangulo_base());
        }

        // parede traseira
        if (proxPos.pega_y() >= maxY && (proxPos.pega_x() >= minX && proxPos.pega_x() <= maxX))
        {
            som->PlaySound(0);
            resolve_colisao(esfera, pad->pega_pad()->pega_parede_traseira()->pega_triangulo_base());
        }
    }
    return 0;
}

void Aux::resolve_colisao(Esfera *esfera1, Tabuleiro *tabuleiro, bool leste)
{
    if (leste)
    {
        double len1 = sqrt(pow(esfera1->pega_direcao()->pega_x(), 2) +
            pow(esfera1->pega_direcao()->pega_y(), 2) +
            pow(esfera1->pega_direcao()->pega_z(), 2));

        esfera1->define_direcao(new Vertice(
            esfera1->pega_direcao()->pega_x()/len1,
            esfera1->pega_direcao()->pega_y()/len1,
            esfera1->pega_direcao()->pega_z()/len1));

        Vertice* colisaoParedeLeste = normal_vertice_parede(
                tabuleiro->pega_Vertices_elipse_leste(),
                tabuleiro->pega_Num_segmentos(),
                tabuleiro->pega_tam_altura_paredes(),
                esfera1, true);

        // r=e−2(e⋅n)n
        double prodEsc = (colisaoParedeLeste->pega_x()*esfera1->pega_direcao()->pega_x()) +
            (colisaoParedeLeste->pega_y()*esfera1->pega_direcao()->pega_y()) +
            (colisaoParedeLeste->pega_z()*esfera1->pega_direcao()->pega_z());



        prodEsc = 2*prodEsc; // 2(e.n)
        Vertice *v = new Vertice(prodEsc*(colisaoParedeLeste->pega_x()),   //2(e.n)n         //1,0,0
                                 prodEsc*colisaoParedeLeste->pega_y(),
                                 prodEsc*colisaoParedeLeste->pega_z());

        /*printf("(%.2f, %.2f, %.2f)leste ",
               colisaoParedeLeste->pega_x(),
               colisaoParedeLeste->pega_y(),
               colisaoParedeLeste->pega_z());*/


        Vertice *r1 = new Vertice(esfera1->pega_direcao()->pega_x() - v->pega_x(),
                                  esfera1->pega_direcao()->pega_y() - v->pega_y(),
                                  esfera1->pega_direcao()->pega_z() - v->pega_z());

        esfera1->define_direcao(r1);
    }
    else
    {
        double len1 = sqrt(pow(esfera1->pega_direcao()->pega_x(), 2) +
            pow(esfera1->pega_direcao()->pega_y(), 2) +
            pow(esfera1->pega_direcao()->pega_z(), 2));

        esfera1->define_direcao(new Vertice(
            esfera1->pega_direcao()->pega_x()/len1,
            esfera1->pega_direcao()->pega_y()/len1,
            esfera1->pega_direcao()->pega_z()/len1));


        Vertice* colisaoParedeOeste = normal_vertice_parede(
                tabuleiro->pega_Vertices_elipse_oeste(),
                tabuleiro->pega_Num_segmentos(),
                tabuleiro->pega_tam_altura_paredes(),
                esfera1, true);


        // r=e−2(e⋅n)n
        double prodEsc = (colisaoParedeOeste->pega_x()*esfera1->pega_direcao()->pega_x()) +
            (colisaoParedeOeste->pega_y()*esfera1->pega_direcao()->pega_y()) +
            (colisaoParedeOeste->pega_z()*esfera1->pega_direcao()->pega_z());

        prodEsc = 2*prodEsc; // 2(e.n)
        Vertice *v = new Vertice(prodEsc*(colisaoParedeOeste->pega_x()),   //2(e.n)n         //1,0,0
                                 prodEsc*colisaoParedeOeste->pega_y(),
                                 prodEsc*colisaoParedeOeste->pega_z());

       /* printf("(%.2f, %.2f, %.2f)oeste ",
               colisaoParedeOeste->pega_x(),
               colisaoParedeOeste->pega_y(),
               colisaoParedeOeste->pega_z());*/

        Vertice *r1 = new Vertice(esfera1->pega_direcao()->pega_x() - v->pega_x(),
                                  esfera1->pega_direcao()->pega_y() - v->pega_y(),
                                  esfera1->pega_direcao()->pega_z() - v->pega_z());

        esfera1->define_direcao(r1);
    }
}
void Aux::resolve_colisao(Esfera *esfera1, Pad *rebatedor)
{
    double len1 = sqrt(pow(esfera1->pega_direcao()->pega_x(), 2) +
        pow(esfera1->pega_direcao()->pega_y(), 2) +
        pow(esfera1->pega_direcao()->pega_z(), 2));

    esfera1->define_direcao(new Vertice(
        esfera1->pega_direcao()->pega_x()/len1,
        esfera1->pega_direcao()->pega_y()/len1,
        esfera1->pega_direcao()->pega_z()/len1));

    Vertice* colisaoPad = normal_vertice_pad(
            rebatedor->pega_Vertices_elipse(),
            rebatedor->pega_Num_segmentos(),
            rebatedor->pega_Valor_tamanho_altura(),
            esfera1, false);

    // r=e−2(e⋅n)n
    double prodEsc = (colisaoPad->pega_x()*esfera1->pega_direcao()->pega_x()) +
        (colisaoPad->pega_y()*esfera1->pega_direcao()->pega_y()) +
        (colisaoPad->pega_z()*esfera1->pega_direcao()->pega_z());

    prodEsc = 2*prodEsc; // 2(e.n)
    Vertice *v = new Vertice(prodEsc*(colisaoPad->pega_x()),   //2(e.n)n         //0,1,0
                             prodEsc*colisaoPad->pega_y(),
                             prodEsc*colisaoPad->pega_z());

    /*printf("(%.2f, %.2f, %.2f) ",
           colisaoPad->pega_x(),
           colisaoPad->pega_y(),
           colisaoPad->pega_z());*/

    Vertice *r1 = new Vertice(esfera1->pega_direcao()->pega_x() - v->pega_x(),
                              esfera1->pega_direcao()->pega_y() - v->pega_y(),
                              esfera1->pega_direcao()->pega_z() - v->pega_z());

    esfera1->define_direcao(r1);
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


// Test for collision between an ellipse of horizontal radius w and vertical radius h at (x0, y0) and
// a circle of radius r at (x1, y1)
bool Aux::detecta_colisao_esfera_elipse(double x0, double y0, double w, double h, double x1, double y1, double r)
{
    double x = fabs(x1 - x0);
    double y = fabs(y1 - y0);

    if (x*x + (h - y)*(h - y) <= r*r || (w - x)*(w - x) + y*y <= r*r || x*h + y*w <= w*h
        || ((x*h + y*w - w*h)*(x*h + y*w - w*h) <= r*r*(w*w + h*h) && x*w - y*h >= -h*h && x*w - y*h <= w*w))
    {
        return true;
    }
    else
    {
        if ((x - w)*(x - w) + (y - h)*(y - h) <= r*r || (x <= w && y - r <= h) || (y <= h && x - r <= w))
        {
            return iterate(x, y, w, 0, 0, h, r*r);
        }
        return false;
    }
}
bool Aux::iterate(double x, double y, double c0x, double c0y, double c2x, double c2y, double rr)
{
    double *innerPolygonCoef;
    double *outerPolygonCoef;
    int maxIterations;

    maxIterations = 10;
    innerPolygonCoef = new double[maxIterations + 1];
    outerPolygonCoef = new double[maxIterations + 1];
    for (int t = 0; t <= maxIterations; t++)
    {
        int numNodes = 4 << t;
        innerPolygonCoef[t] = 0.5/cos(4*acos(0.0)/numNodes);
        outerPolygonCoef[t] = 0.5/(cos(2*acos(0.0)/numNodes)*cos(2*acos(0.0)/numNodes));
    }

    for (int t = 1; t <= maxIterations; t++)
    {
        double c1x = (c0x + c2x)*innerPolygonCoef[t];
        double c1y = (c0y + c2y)*innerPolygonCoef[t];
        double tx = x - c1x;
        double ty = y - c1y;
        if (tx*tx + ty*ty <= rr)
        {
            return true;
        }
        double t2x = c2x - c1x;
        double t2y = c2y - c1y;
        if (tx*t2x + ty*t2y >= 0 && tx*t2x + ty*t2y <= t2x*t2x + t2y*t2y &&
            (ty*t2x - tx*t2y >= 0 || rr*(t2x*t2x + t2y*t2y) >= (ty*t2x - tx*t2y)*(ty*t2x - tx*t2y)))
        {
            return true;
        }
        double t0x = c0x - c1x;
        double t0y = c0y - c1y;
        if (tx*t0x + ty*t0y >= 0 && tx*t0x + ty*t0y <= t0x*t0x + t0y*t0y &&
            (ty*t0x - tx*t0y <= 0 || rr*(t0x*t0x + t0y*t0y) >= (ty*t0x - tx*t0y)*(ty*t0x - tx*t0y)))
        {
            return true;
        }
        double c3x = (c0x + c1x)*outerPolygonCoef[t];
        double c3y = (c0y + c1y)*outerPolygonCoef[t];
        if ((c3x - x)*(c3x - x) + (c3y - y)*(c3y - y) < rr)
        {
            c2x = c1x;
            c2y = c1y;
            continue;
        }
        double c4x = c1x - c3x + c1x;
        double c4y = c1y - c3y + c1y;
        if ((c4x - x)*(c4x - x) + (c4y - y)*(c4y - y) < rr)
        {
            c0x = c1x;
            c0y = c1y;
            continue;
        }
        double t3x = c3x - c1x;
        double t3y = c3y - c1y;
        if (ty*t3x - tx*t3y <= 0 || rr*(t3x*t3x + t3y*t3y) > (ty*t3x - tx*t3y)*(ty*t3x - tx*t3y))
        {
            if (tx*t3x + ty*t3y > 0)
            {
                if (fabs(tx*t3x + ty*t3y) <= t3x*t3x + t3y*t3y || (x - c3x)*(c0x - c3x) + (y - c3y)*(c0y - c3y) >= 0)
                {
                    c2x = c1x;
                    c2y = c1y;
                    continue;
                }
            }
            else if (-(tx*t3x + ty*t3y) <= t3x*t3x + t3y*t3y || (x - c4x)*(c2x - c4x) + (y - c4y)*(c2y - c4y) >= 0)
            {
                c0x = c1x;
                c0y = c1y;
                continue;
            }
        }
        return false;
    }
    return false; // Out of iterations so it is unsure if there was a collision. But have to return something.
}

/**
 * recebe o vetor com os vertices da parede em formato elíptico, seu tamanho, o tamanho das paredes do tabuleiro,
 * o ponteiro para a esfera e um booleano  para que o sentido dos pontos possa ser invertido (ou não) para que a
 * normal aponte para o lado correto
 **/
Vertice* Aux::normal_vertice_parede(Vertice *vertices_elipse, int nSeg, double tamParedes, Esfera *esfera1, bool inverso)
{
    // testa qual vertice da parede será selecionado para obter a normal
    // para isso, a ideia é obter a menor diferença absoluta entre os valores de 'x'
    // da esfera e da parede onde a diferença absoluta entre os valores de y é mínima
    int menor = 0;
    double distX = abs(vertices_elipse[menor].pega_x() - esfera1->pega_posicao()->pega_x() );
    for(int z=0; z<nSeg; z++)
    {
        if( abs(vertices_elipse[z].pega_x() - esfera1->pega_posicao()->pega_x()) < distX &&
            abs(vertices_elipse[z].pega_y() - esfera1->pega_posicao()->pega_y()) < 0.1  )
        {
            distX = abs(vertices_elipse[z].pega_x() - esfera1->pega_posicao()->pega_x());
            menor = z;
        }
    }

    Vertice topoParede = vertices_elipse[menor];
    topoParede.define_z(tamParedes+0.01);
    Vertice* colisaoParede;
    colisaoParede = calcula_normal(
            &topoParede,
            &vertices_elipse[menor+1],
            &vertices_elipse[menor-1]);
    if(inverso)
    {
        colisaoParede = calcula_normal(
                &topoParede,
                &vertices_elipse[menor-1],
                &vertices_elipse[menor+1]);
    }

    double normaV = sqrt(pow(colisaoParede->pega_x(),2) + pow(colisaoParede->pega_y(),2) + pow(colisaoParede->pega_z(),2));
    colisaoParede->define_x(colisaoParede->pega_x()/normaV);
    colisaoParede->define_y(colisaoParede->pega_y()/normaV);
    colisaoParede->define_z(colisaoParede->pega_z()/normaV);

    return colisaoParede;
}


/**
 * recebe o vetor com os vertices do pad em formato elíptico, seu tamanho, a altura do pad,
 * o ponteiro para a esfera e um booleano  para que o sentido dos pontos possa ser invertido (ou não) para que a
 * normal aponte para o lado correto
 **/
Vertice* Aux::normal_vertice_pad(Vertice *vertices_elipse, int nSeg, double tamPad, Esfera *esfera1, bool inverso)
{
    // testa qual vertice do pad será selecionado para obter a normal
    // para isso, a ideia é obter a menor diferença absoluta entre os valores de 'y'
    // da esfera e da parede onde a diferença absoluta entre os valores de x é mínima
    int menor = 0;
    double distY = abs(vertices_elipse[menor].pega_y() - esfera1->pega_posicao()->pega_y() );
    for(int z=0; z<nSeg; z++)
    {
        if( abs(vertices_elipse[z].pega_y() - esfera1->pega_posicao()->pega_y()) < distY &&
            abs(vertices_elipse[z].pega_x() - esfera1->pega_posicao()->pega_x()) < 0.05  )
        {
            distY = abs(vertices_elipse[z].pega_y() - esfera1->pega_posicao()->pega_y());
            menor = z;
        }
    }

    Vertice topoParede = vertices_elipse[menor];
    topoParede.define_z(tamPad+0.01);
    Vertice* colisaoParede;
    colisaoParede = calcula_normal(
            &topoParede,
            &vertices_elipse[menor+(nSeg/10)],
            &vertices_elipse[menor-(nSeg/10)]);
    if(inverso)
    {
        colisaoParede = calcula_normal(
                &topoParede,
                &vertices_elipse[menor-(nSeg/10)],
                &vertices_elipse[menor+(nSeg/10)]);
    }

    double normaV = sqrt(pow(colisaoParede->pega_x(),2) + pow(colisaoParede->pega_y(),2) + pow(colisaoParede->pega_z(),2));
    colisaoParede->define_x(colisaoParede->pega_x()/normaV);
    colisaoParede->define_y(colisaoParede->pega_y()/normaV);
    colisaoParede->define_z(colisaoParede->pega_z()/normaV);

    return colisaoParede;
}