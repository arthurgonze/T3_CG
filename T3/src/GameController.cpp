#include "GameController.h"
#include "Esfera.h"
#include "IO.h"
#include <math.h>
#include <stdio.h>
#include <string>

GameController::GameController()
{
    anguloDisparo = -90.0;
    taxaDeAumentoAngulo = 5.0;
    anguloDisparoMaximo = 60.0;

    velEsfera = 1.5;
    velEsferaPausa;
    taxaDeAumentoVelocidade = 0.5;
    velMaxEsfera = 5.0;

    jogoIniciado = false;
    jogoPausado = false;
    jogoVencido = false;

    velocidadePad = 1.5;

    // matriz de blocos
    tipoMaterial = 1;
    numBlocosPorColuna = 5;
    numBlocosPorLinha = 5;
    espacamentoBlocos = 0.05;
    matrizBlocos = new Bloco**[numBlocosPorColuna];
    posicoesBlocos = new bool*[numBlocosPorColuna];

    //primeiro arquivo será a primeira fase
    nomeArquivo = "1.txt";
    leitura = new IO(nomeArquivo);

    // variaveis iniciais do controle de fases e vidas
    fase = 1;
    numFases = 2;
    numInicialVidas = 5;
    numVidas = numInicialVidas;
}

GameController::~GameController()
{
    //dtor
}

void GameController::defineOrganizacaoBlocos()
{
    // cria matriz binaria para decidir onde será desenhado um bloco ou nao
    // como essa informação vem de arquivo externo, inicializa vazia
    for(int i=0; i<numBlocosPorColuna; i++)
    {
        posicoesBlocos[i] = new bool[numBlocosPorColuna];
        for(int j=0; j<numBlocosPorLinha; j++)
        {
            posicoesBlocos[i][j] = false;
        }
    }

    // lê arquivo e escreve na matriz de posições
    leitura->carregaFase(fase, posicoesBlocos, numBlocosPorLinha, numBlocosPorColuna, &tipoMaterial);
}

void GameController::criaMatrizBlocos(double baseTab, double alturaTab)
{
    defineOrganizacaoBlocos();

    // cria a matriz de Blocos
    criaBlocos(baseTab, alturaTab);
}

/// Cria uma matriz de blocos recebendo quantos blocos são desejados por linha e por coluna,
/// o espaçamento desejado entre eles e as dimensões do tabuleiro para que os limites sejam tratados.
void GameController::criaBlocos(double baseTab, double alturaTab)
{
    /// O tipo Bloco*** se deve ao fato de ser uma matriz (**) de objetos do tipo Bloco*.
    //matrizBlocos = new Bloco**[numBlocosPorColuna];

    // tamanho do bloco é calculado em função do número de blocos desenhado por linha.
    // espaçamento multiplicado por 1.1 é adicionado para evitar que encoste na parede do tabuleiro a direita.
    double tamBloco = (baseTab/numBlocosPorLinha) - 1.1*espacamentoBlocos;
    double altBloco = tamBloco/2.5;

    // posicao X do vértice do primeiro bloco.
    // espaçamento é adicionado para evitar que encoste na parede do tabuleiro a esquerda.
    double xBloco = espacamentoBlocos;

    // posicao Y do vértice do primeiro bloco.
    // como começa no Y máximo do tabuleiro, é subtraído do tamanho para que não seja criado fora dele.
    // espaçamento é adicionado para evitar que encoste na parede do tabuleiro em cima.
    double yBloco = alturaTab - altBloco*5 - espacamentoBlocos;

    for(int i=0; i<numBlocosPorColuna; i++)
    {
        matrizBlocos[i] = new Bloco*[numBlocosPorColuna];
        for(int j=0; j<numBlocosPorLinha; j++)
        {
            {
                // cria o vertice do bloco (0.01 em z para não coincidir com a base do tabuleiro).
                Vertice *v = new Vertice(xBloco, yBloco, 0.01);

                // cria e adiciona à matriz o bloco, utilizando o vertice, tamX, tamY e tamZ do bloco, alem de torná-lo destrutível.
                Bloco* b = new Bloco(v,tamBloco, altBloco, 0.1, true);
                b->setDestruido(posicoesBlocos[i][j]);
                matrizBlocos[i][j] = b;

                // posicao X do vértice do proximo bloco, já considerando o espaçamento.
                xBloco = xBloco + tamBloco + espacamentoBlocos;
            }
        }
        // quando faz uma linha completa, ajusta os valores de xBloco e yBloco.
        // posicao X do vértice do proximo bloco volta a ser a mesma do primeiro bloco.
        xBloco = espacamentoBlocos;
        // posicao Y do vértice do proximo bloco desce uma linha, já considerando o espaçamento.
        yBloco -= altBloco + espacamentoBlocos;
    }
}

void GameController::restartGame(Esfera* esfera, Pad* pad)
{
    // se não tem mais vidas, pula as seguintes para evento de fim de jogo
    if(numVidas == 0)
    {
        fase = 0;
    }

    setJogoIniciado(false);
    anguloDisparo = -90.0;
    esfera->setPosicao(new Vertice((0.5+0.9)/2, 0.3, 0.15));
    esfera->setDirecao(new Vertice(0.0, 1.0, 0.0));
    pad->getPad()->setVertice(new Vertice(0.5, 0.1, 0.01));
    spawn1Fora = false;
    spawn2Fora = false;
    resetaMatriz();
}

// similar ao restartGame, mas não reseta a matriz caso ainda tenha vidas
void GameController::continuaFase(Esfera* esfera, Pad* pad)
{
    // se não tem mais vidas, pula as seguintes para evento de fim de jogo
    if(numVidas == 0)
    {
        fase = 0;
        resetaMatriz();
    }

    setJogoIniciado(false);
    anguloDisparo = -90.0;
    esfera->setPosicao(new Vertice((0.5+0.9)/2, 0.3, 0.15));
    esfera->setDirecao(new Vertice(0.0, 1.0, 0.0));
    pad->getPad()->setVertice(new Vertice(0.5, 0.1, 0.01));
}

void GameController::switchPause()
{
    if(jogoIniciado && !jogoPausado)
    {
        velEsferaPausa = velEsfera;
        velEsfera = 0;
        jogoPausado = true;
    }
    else if(jogoIniciado && jogoPausado)
    {
        velEsfera = velEsferaPausa;
        jogoPausado = false;
    }
}

bool GameController::checaFinalDeJogo(Esfera* esfera, Pad* pad)
{
    bool fimFase = true;

    for(int i=0; i<numBlocosPorColuna; i++)
    {
        for(int j=0; j<numBlocosPorLinha; j++)
        {

            if(!matrizBlocos[i][j]->getDestruido())
            {
                //ainda ha blocos para serem destruidos -- fase não acabou
                fimFase = false;
                return false;
            }
        }
    }

    // jogador venceu todas as fases -- ganhou o jogo
    if(fimFase && fase == numFases)
    {
        // pula para fase extra vazia no final de jogo
        fase = 0;
        // identifica que o jogo foi realmente vencido para exibir a mensagem
        jogoVencido = true;
        return true;
    }
    // jogador venceu uma fase, mas ainda há mais fases para serem vencidas
    else if(fimFase && fase >= 1 && fase < numFases)
    {
        // avança para proxima fase e a carrega
        fase++;
        leitura->carregaFase(fase, posicoesBlocos, numBlocosPorLinha, numBlocosPorColuna, &tipoMaterial);
        restartGame(esfera, pad);
        return false;
    }
    //fim de jogo
    else if(fase == 0)
    {
        leitura->carregaFase(fase, posicoesBlocos, numBlocosPorLinha, numBlocosPorColuna, &tipoMaterial);
        restartGame(esfera, pad);
        return true;
    }
}

void GameController::resetaMatriz()
{
    defineOrganizacaoBlocos();

    for(int i=0; i<numBlocosPorColuna; i++)
    {
        for(int j=0; j<numBlocosPorLinha; j++)
        {
            matrizBlocos[i][j]->setDestruido(posicoesBlocos[i][j]);
        }
    }
}
