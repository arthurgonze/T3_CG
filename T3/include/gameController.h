#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "esfera.h"
#include "bloco.h"
#include "io.h"
#include <math.h>
#include <stdio.h>
#include <string>

class Bloco;
class Pad;
class GameController
{
private:
    double angulo_disparo, angulo_disparo_maximo, taxa_aumento_angulo; // angulo esfera
    double vel_esfera, vel_esfera_pausa, vel_max_esfera, taxa_aumento_velocidade; // velocidade esfera
    bool jogo_iniciado, jogo_pausado, jogo_vencido;
    double velocidade_pad;
    double fps_desejado;
    bool spawn1_fora = false, spawn2_fora = false;

    // matriz de blocos
    bool **posicoes_blocos;
    Bloco ***matriz_blocos;
    int num_blocos_por_coluna;
    int num_blocos_por_linha;
    int tipo_material;
    double espacamento_blocos;
    void cria_blocos(double base_tab, double altura_tab);
    void define_organizacao_blocos();

    // leitura
    std::string nome_arquivo;
    int fase;
    IO *leitura;
    int num_fases, num_vidas, num_inicial_vidas;

public:
    /// CONSTRUTOR & DESTRUTOR
    GameController();
    virtual ~GameController();

    /// GETTERS
    inline bool pega_jogo_iniciado() { return jogo_iniciado; }
    inline bool pega_jogo_pausado() { return jogo_pausado; }
    inline double pega_angulo_disparo() { return angulo_disparo; }
    inline double pega_angulo_disparo_maximo() { return angulo_disparo_maximo; }
    inline double pega_taxa_de_aumento_angulo() { return taxa_aumento_angulo; }
    inline double pega_vel_esfera() { return vel_esfera; }
    inline double pega_vel_max_esfera() { return vel_max_esfera; }
    inline double pega_taxa_de_aumento_velocidade() { return taxa_aumento_velocidade; }
    inline double pega_espaco_blocos_matriz() { return espacamento_blocos; }
    inline double pega_velocidade_pad() { return velocidade_pad; }
    inline double pega_jogo_vencido() { return jogo_vencido; }
    inline double pega_fps_desejado() { return fps_desejado; }
    inline int pega_num_vidas() { return num_vidas; }
    inline int pega_num_fases() { return num_fases; }
    inline int pega_fase() { return fase; }
    inline int pega_tipo_material() { return tipo_material; }
    inline int pega_num_blocos_linha_matriz() { return num_blocos_por_linha; }
    inline int pega_num_blocos_coluna_matriz() { return num_blocos_por_coluna; }
    inline Bloco ***pega_matriz_blocos() { return matriz_blocos; }
    inline Bloco *pega_bloco_matriz(int i, int j) { return matriz_blocos[i][j]; }
    inline bool pega_spawn1_fora() { return spawn1_fora; }
    inline bool pega_spawn2_fora() { return spawn2_fora; }

    /// SETTERS
    inline void define_vel_pad(double nova_vel) { velocidade_pad = nova_vel; }
    inline void define_fps_desejado(double fps_desejado) { this->fps_desejado = fps_desejado; }
    inline void define_jogo_iniciado(bool toggle) { jogo_iniciado = toggle; }
    inline void define_jogo_pausado(bool toggle) { jogo_pausado = toggle; }
    inline void define_angulo_disparo(double novo_angulo_disparo) { angulo_disparo = novo_angulo_disparo; }
    inline void define_Angulo_disparo_maximo(double novo_angulo_disparo_maximo) { angulo_disparo_maximo = novo_angulo_disparo_maximo; }
    inline void define_taxa_de_aumento_angulo(double nova_taxa_de_aumento_angulo) { taxa_aumento_angulo = nova_taxa_de_aumento_angulo; }
    inline void define_vel_esfera(double nova_vel_esfera) { vel_esfera = nova_vel_esfera; }
    inline void define_vel_max_esfera(double nova_vel_max_esfera) { vel_max_esfera = nova_vel_max_esfera; }
    inline void define_taxa_de_aumento_velocidade(double nova_taxa_de_aumento_velocidade) { taxa_aumento_velocidade = nova_taxa_de_aumento_velocidade; }
    inline void define_num_blocos_linha_matriz(int novo_blocos_linha) { num_blocos_por_linha = novo_blocos_linha; }
    inline void define_num_blocos_coluna_matriz(int novo_blocos_coluna) { num_blocos_por_coluna = novo_blocos_coluna; }
    inline void define_espaco_blocos_matriz(double novo_espacamento) { espacamento_blocos = novo_espacamento; }
    inline void define_spawn1_fora(bool alternancia) { spawn1_fora = alternancia; }
    inline void define_spawn2_fora(bool alternancia) { spawn2_fora = alternancia; }

    inline void perde_vida() { num_vidas--; }
    inline void reseta_fases() { fase = 1; }
    inline void reseta_vidas() { num_vidas = num_inicial_vidas; }
    void cria_matriz_blocos(double base_tab, double altura_tab);
    void restart_game(Esfera *esfera, Pad *pad);
    void switch_pause();
    bool checa_final_de_jogo(Esfera *esfera, Pad *pad);
    void reseta_matriz();
    void continua_fase(Esfera *esfera, Pad *pad);
};

#endif // GAMECONTROLLER_H
