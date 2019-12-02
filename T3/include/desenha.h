#ifndef DESENHA_H
#define DESENHA_H

#include <GL/glut.h>
#include "aux.h"
#include "tabuleiro.h"
#include "triangulo.h"
#include "vertice.h"
#include "plano.h"
#include "esfera.h"
#include "bloco.h"
#include "camera.h"
#include "glcTexture.h"
#include "skybox.h"
#include "glcWavefrontObject.h"
#include "gameController.h"
class Bloco;
class Triangulo;
class Tabuleiro;
class Desenha
{
private:
    void desenha_plano(Plano *plano, bool inv, int normal_dir);
    void desenha_triangulo(Triangulo *triangulo, bool inv, int normal_dir);
    void desenha_normal(Triangulo *triangulo, int dir);
    void desenha_parede_leste_tabuleiro(Tabuleiro *tabuleiro) const;
    void desenha_parede_oeste_tabuleiro(Tabuleiro *tabuleiro) const;

public:
    /// CONSTRUCTOR & DESTRUCTOR
    Desenha();
    virtual ~Desenha();

    void desenha_tabuleiro(Tabuleiro *tabuleiro);
    void desenha_esfera(Esfera *esfera);
    void desenha_bloco(Bloco *bloco);
    void desenha_seta_direcao(Esfera *esfera, double angulo_disparo);
    void desenha_vetor_direcao_esfera(Esfera *esfera);
    void desenha_matriz_blocos(Bloco ***matriz, int blocos_Col, int blocos_lin);
    void desenha_objetos_importados(glcWavefrontObject *gerenciador_de_objetos, Esfera *geracao_esfera_1, Esfera *geracao_esfera_2);
    void desenha_texto_nivel(GameController *controlador_de_jogo);
    void desenha_vidas(GameController *controlador_de_jogo, Vertice *ponto_inicial_vidas);
    void desenha_skybox(glcTexture *textureManager, Camera *camera, Skybox *skybox, bool rotacao_em_conjunto);
    void desenha_rebatedor(Pad *rebatedor);
};

#endif // DESENHA_H
