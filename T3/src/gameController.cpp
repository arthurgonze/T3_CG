#include "gameController.h"


GameController::GameController()
{
    angulo_disparo = -90.0;
    taxa_aumento_angulo = 5.0;
    angulo_disparo_maximo = 60.0;

    vel_esfera = 1.5;
    vel_esfera_pausa;
    taxa_aumento_velocidade = 0.5;
    vel_max_esfera = 5.0;

    jogo_iniciado = false;
    jogo_pausado = false;
    jogo_vencido = false;

    velocidade_pad = 1.5;

    // matriz de blocos
    tipo_material = 1;
    num_blocos_por_coluna = 5;
    num_blocos_por_linha = 5;
    espacamento_blocos = 0.05;
    matriz_blocos = new Bloco **[num_blocos_por_coluna];
    posicoes_blocos = new bool *[num_blocos_por_coluna];

    //primeiro arquivo será a primeira fase
    nome_arquivo = "1.txt";
    leitura = new IO(nome_arquivo);

    // variaveis iniciais do controle de fases e vidas
    fase = 1;
    num_fases = 2;
    num_inicial_vidas = 5;
    num_vidas = num_inicial_vidas;
}

GameController::~GameController()
{
    //dtor
}

void GameController::define_organizacao_blocos()
{
    // cria matriz binaria para decidir onde será desenhado um bloco ou nao
    // como essa informação vem de arquivo externo, inicializa vazia
    for (int i = 0; i < num_blocos_por_coluna; i++)
    {
        posicoes_blocos[i] = new bool[num_blocos_por_coluna];
        for (int j = 0; j < num_blocos_por_linha; j++)
        {
            posicoes_blocos[i][j] = false;
        }
    }

    // lê arquivo e escreve na matriz de posições
    leitura->carrega_fase(fase, posicoes_blocos, num_blocos_por_linha, num_blocos_por_coluna, &tipo_material);
}

void GameController::cria_matriz_blocos(double base_tab, double altura_tab)
{
    define_organizacao_blocos();

    // cria a matriz de Blocos
    cria_blocos(base_tab, altura_tab);
}

/// Cria uma matriz de blocos recebendo quantos blocos são desejados por linha e por coluna,
/// o espaçamento desejado entre eles e as dimensões do tabuleiro para que os limites sejam tratados.
void GameController::cria_blocos(double base_tab, double altura_tab)
{
    /// O tipo Bloco*** se deve ao fato de ser uma matriz (**) de objetos do tipo Bloco*.
    //matriz_blocos = new Bloco**[num_blocos_por_coluna];

    // tamanho do bloco é calculado em função do número de blocos desenhado por linha.
    // espaçamento multiplicado por 1.1 é adicionado para evitar que encoste na parede do tabuleiro a direita.
    double tamBloco = (base_tab/num_blocos_por_linha) - 1.1*espacamento_blocos;
    double altBloco = tamBloco/2.5;

    // posicao X do vértice do primeiro bloco.
    // espaçamento é adicionado para evitar que encoste na parede do tabuleiro a esquerda.
    double xBloco = espacamento_blocos+1;

    // posicao Y do vértice do primeiro bloco.
    // como começa no Y máximo do tabuleiro, é subtraído do tamanho para que não seja criado fora dele.
    // espaçamento é adicionado para evitar que encoste na parede do tabuleiro em cima.
    double yBloco = altura_tab - altBloco*5 - espacamento_blocos;

    for (int i = 0; i < num_blocos_por_coluna; i++)
    {
        matriz_blocos[i] = new Bloco *[num_blocos_por_coluna];
        for (int j = 0; j < num_blocos_por_linha; j++)
        {
            {
                // cria o vertice do bloco (0.01 em z para não coincidir com a base do tabuleiro).
                Vertice *v = new Vertice(xBloco, yBloco, 0.01);

                // cria e adiciona à matriz o bloco, utilizando o vertice, tamX, tamY e tamZ do bloco, alem de torná-lo destrutível.
                Bloco *b = new Bloco(v, tamBloco, altBloco, 0.1, true);
                b->define_destruido(posicoes_blocos[i][j]);
                matriz_blocos[i][j] = b;

                // posicao X do vértice do proximo bloco, já considerando o espaçamento.
                xBloco = xBloco + tamBloco + espacamento_blocos;
            }
        }
        // quando faz uma linha completa, ajusta os valores de xBloco e yBloco.
        // posicao X do vértice do proximo bloco volta a ser a mesma do primeiro bloco.
        xBloco = espacamento_blocos+1;
        // posicao Y do vértice do proximo bloco desce uma linha, já considerando o espaçamento.
        yBloco -= altBloco + espacamento_blocos;
    }
}

void GameController::restart_game(Esfera *esfera, Pad *pad)
{
    // se não tem mais vidas, pula as seguintes para evento de fim de jogo
    if (num_vidas==0)
    {
        fase = 0;
    }

    define_jogo_iniciado(false);
    angulo_disparo = -90.0;
    esfera->define_posicao(new Vertice((0.5 + 0.9)/2, 0.3, 0.15));
    esfera->define_direcao(new Vertice(0.0, 1.0, 0.0));
    pad->pega_pad()->define_vertice(new Vertice(0.5, 0.1, 0.01));
    spawn1_fora = false;
    spawn2_fora = false;
    reseta_matriz();
}

// similar ao restart_game, mas não reseta a matriz caso ainda tenha vidas
void GameController::continua_fase(Esfera *esfera, Pad *pad)
{
    // se não tem mais vidas, pula as seguintes para evento de fim de jogo
    if (num_vidas==0)
    {
        fase = 0;
        reseta_matriz();
    }

    define_jogo_iniciado(false);
    angulo_disparo = -90.0;
    esfera->define_posicao(new Vertice((0.5 + 0.9)/2, 0.3, 0.15));
    esfera->define_direcao(new Vertice(0.0, 1.0, 0.0));
    pad->pega_pad()->define_vertice(new Vertice(0.5, 0.1, 0.01));
}

void GameController::switch_pause()
{
    if (jogo_iniciado && !jogo_pausado)
    {
        vel_esfera_pausa = vel_esfera;
        vel_esfera = 0;
        jogo_pausado = true;
    }
    else if (jogo_iniciado && jogo_pausado)
    {
        vel_esfera = vel_esfera_pausa;
        jogo_pausado = false;
    }
}

bool GameController::checa_final_de_jogo(Esfera *esfera, Pad *pad)
{
    bool fimFase = true;

    for (int i = 0; i < num_blocos_por_coluna; i++)
    {
        for (int j = 0; j < num_blocos_por_linha; j++)
        {

            if (!matriz_blocos[i][j]->pega_destruido())
            {
                //ainda ha blocos para serem destruidos -- fase não acabou
                fimFase = false;
                return false;
            }
        }
    }

    // jogador venceu todas as fases -- ganhou o jogo
    if (fimFase && fase==num_fases)
    {
        // pula para fase extra vazia no final de jogo
        fase = 0;
        // identifica que o jogo foi realmente vencido para exibir a mensagem
        jogo_vencido = true;
        return true;
    }
        // jogador venceu uma fase, mas ainda há mais fases para serem vencidas
    else if (fimFase && fase >= 1 && fase < num_fases)
    {
        // avança para proxima fase e a carrega
        fase++;
        leitura->carrega_fase(fase, posicoes_blocos, num_blocos_por_linha, num_blocos_por_coluna, &tipo_material);
        restart_game(esfera, pad);
        return false;
    }
        //fim de jogo
    else if (fase==0)
    {
        leitura->carrega_fase(fase, posicoes_blocos, num_blocos_por_linha, num_blocos_por_coluna, &tipo_material);
        restart_game(esfera, pad);
        return true;
    }
}

void GameController::reseta_matriz()
{
    define_organizacao_blocos();

    for (int i = 0; i < num_blocos_por_coluna; i++)
    {
        for (int j = 0; j < num_blocos_por_linha; j++)
        {
            matriz_blocos[i][j]->define_destruido(posicoes_blocos[i][j]);
        }
    }
}
