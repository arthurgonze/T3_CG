#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "Esfera.h"
#include "Bloco.h"
#include "IO.h"

class Bloco;
class Pad;
class GameController
{
private:
    double anguloDisparo, anguloDisparoMaximo, taxaDeAumentoAngulo; // angulo esfera
    double velEsfera, velEsferaPausa, velMaxEsfera, taxaDeAumentoVelocidade; // velocidade esfera
    bool jogoIniciado, jogoPausado, jogoVencido;
    double velocidadePad;
    bool spawn1Fora = false, spawn2Fora = false;

    // matriz de blocos
    bool **posicoesBlocos;
    Bloco ***matrizBlocos;
    int numBlocosPorColuna;
    int numBlocosPorLinha;
    int tipoMaterial;
    double espacamentoBlocos;
    void criaBlocos(double baseTab, double alturaTab);
    void defineOrganizacaoBlocos();

    // leitura
    std::string nomeArquivo;
    int fase;
    IO *leitura;
    int numFases, numVidas, numInicialVidas;

public:
    /// CONSTRUCTOR & DESTRUCTOR
    GameController();
    virtual ~GameController();

    /// GETTERS
    inline bool getJogoIniciado() { return jogoIniciado; };
    inline bool getJogoPausado() { return jogoPausado; };
    inline double getAnguloDisparo() { return anguloDisparo; };
    inline double getAnguloDisparoMaximo() { return anguloDisparoMaximo; };
    inline double getTaxaDeAumentoAngulo() { return taxaDeAumentoAngulo; };
    inline double getVelEsfera() { return velEsfera; };
    inline double getVelMaxEsfera() { return velMaxEsfera; };
    inline double getTaxaDeAumentoVelocidade() { return taxaDeAumentoVelocidade; };
    inline double getEspacoBlocosMatriz() { return espacamentoBlocos; };
    inline double getVelocidadePad() { return velocidadePad; };
    inline double getJogoVencido() { return jogoVencido; };
    inline int getNumVidas() { return numVidas; };
    inline int getNumFases() { return numFases; };
    inline int getFase() { return fase; };
    inline int getTipoMaterial() { return tipoMaterial; };
    inline int getNumBlocosLinhaMatriz() { return numBlocosPorLinha; };
    inline int getNumBlocosColunaMatriz() { return numBlocosPorColuna; };
    inline Bloco ***getMatrizBlocos() { return matrizBlocos; };
    inline Bloco *getBlocoMatriz(int i, int j) { return matrizBlocos[i][j]; };
    inline bool getSpawn1Fora() { return spawn1Fora; }
    inline bool getSpawn2Fora() { return spawn2Fora; }

    /// SETTERS
    inline void setVelPad(double novaVel) { velocidadePad = novaVel; };
    inline void setJogoIniciado(bool toggle) { jogoIniciado = toggle; };
    inline void setJogoPausado(bool toggle) { jogoPausado = toggle; };
    inline void setAnguloDisparo(double novoAnguloDisparo) { anguloDisparo = novoAnguloDisparo; };
    inline void setAnguloDisparoMaximo(double novoAnguloDisparoMaximo) { anguloDisparoMaximo = novoAnguloDisparoMaximo; };
    inline void setTaxaDeAumentoAngulo(double novaTaxaDeAumentoAngulo) { taxaDeAumentoAngulo = novaTaxaDeAumentoAngulo; };
    inline void setVelEsfera(double novaVelEsfera) { velEsfera = novaVelEsfera; };
    inline void setVelMaxEsfera(double novaVelMaxEsfera) { velMaxEsfera = novaVelMaxEsfera; };
    inline void setTaxaDeAumentoVelocidade(double novaTaxaDeAumentoVelocidade) { taxaDeAumentoVelocidade = novaTaxaDeAumentoVelocidade; };
    inline void setNumBlocosLinhaMatriz(int novoBlocosLinha) { numBlocosPorLinha = novoBlocosLinha; };
    inline void setNumBlocosColunaMatriz(int novoBlocosColuna) { numBlocosPorColuna = novoBlocosColuna; };
    inline void setEspacoBlocosMatriz(double novoEspacamento) { espacamentoBlocos = novoEspacamento; };
    inline void setSpawn1Fora(bool toggle) { spawn1Fora = toggle; }
    inline void setSpawn2Fora(bool toggle) { spawn2Fora = toggle; }

    inline void perdeVida() { numVidas--; };
    inline void resetaFases() { fase = 1; };
    inline void resetaVidas() { numVidas = numInicialVidas; };
    void criaMatrizBlocos(double baseTab, double alturaTab);
    void restartGame(Esfera *esfera, Pad *pad);
    void switchPause();
    bool checaFinalDeJogo(Esfera *esfera, Pad *pad);
    void resetaMatriz();
    void continuaFase(Esfera *esfera, Pad *pad);
};

#endif // GAMECONTROLLER_H
