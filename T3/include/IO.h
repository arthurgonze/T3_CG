#ifndef IO_H
#define IO_H

class IO
{
private:
    char diretorioEntradas[200];
    std::string nomeEntrada;

    const char *getCaminhoDiretorio();
    void executaCarregaFase(char *caminhoDiretorio, bool **matriz, int numLinhas, int numColunas, int *tipoMaterial);

public:
    IO(std::string = "");
    ~IO() {};
    char *getDiretorioEntradas() { return diretorioEntradas; };
    std::string getNomeEntrada() { return nomeEntrada; };
    void setNomeEntrada(std::string nomeArquivo) { nomeEntrada = nomeArquivo; };

    void carregaFase(int fase, bool **matriz, int numLinhas, int numColunas, int *tipoMaterial);
};

#endif // IO_H
