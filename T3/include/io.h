#ifndef IO_H
#define IO_H

#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <dirent.h>
#include <errno.h>

class IO
{
private:
    char diretorio_entradas[200];
    std::string nome_entrada;

    const char *pega_caminho_diretorio();
    void executa_carrega_fase(char *caminho_diretorio, bool **matriz, int num_linhas, int num_colunas, int *tipo_material);

public:
    IO(std::string = "");
    ~IO() {};
    char *pega_diretorio_entradas() { return diretorio_entradas; };
    std::string pega_nome_entrada() { return nome_entrada; };
    void define_nome_entrada(std::string nome_arquivo) { nome_entrada = nome_arquivo; };

    void carrega_fase(int fase, bool **matriz, int num_linhas, int num_colunas, int *tipo_material);
};

#endif // IO_H
