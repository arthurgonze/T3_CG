#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <dirent.h>
#include "IO.h"
#include <errno.h>

using namespace std;

// por algum motivo o CodeBlocks só acha a ppasta com caminho completo
const char* IO::getCaminhoDiretorio()
{
    string f = "";
    string g = "";
    f += __FILE__;
    unsigned int i = f.size();
    //elimina o nome do arquivo
    while(f[i]!= '/' && i>0)
        i--;
    for(unsigned int j=0; j<i; j++)
        g = g + f[j];

    //trata a string para que fique interpretável
    char* h = new char[g.size() + 1];
    copy(g.begin(), g.end(), h);
    h[g.size()] = '\0';

    return h;
}

/**
 * Construtor inicializa com o nome de um arquivo
 **/
IO::IO(string nomeArquivo)
{
    const char* diretorio = getCaminhoDiretorio();
    nomeEntrada = nomeArquivo;

    //guarda o caminho do diretório que contém as instâncias
    strcpy(diretorioEntradas, diretorio);

    strcat(diretorioEntradas, "/fases/");
}

/**
 * Executa o procedimento de leitura de arquivo
 **/
void IO::carregaFase(int fase, bool** matriz, int numLinhas, int numColunas, int* tipoMaterial)
{
    // fase é um int, entao converte para string
    stringstream ss;
    ss<<fase;
    string strFase;
    ss>>strFase;

    //cout << strFase << endl;

    setNomeEntrada(strFase+".txt");
    //passa o objeto e o caminho onde estão as instâncias
    executaCarregaFase(diretorioEntradas, matriz, numLinhas, numColunas, tipoMaterial);
}

/**
 * Lê uma instância armazenada no diretório passado no parâmetro
 **/
void IO::executaCarregaFase(char* caminhoDiretorio, bool** matriz, int numLinhas, int numColunas, int* tipoMaterial)
{
    string info;
    string nomeArquivo =getNomeEntrada();
    char * nomeArquivoChar;
    int n;
    bool flagArq = true;
    ifstream arqFase;
    char caminhoArquivo[200];
    strcpy(caminhoArquivo, caminhoDiretorio); //copia caminho do diretorio para o caminho do arquivo
    DIR *d;
    struct dirent *dir;

    d = opendir(caminhoDiretorio); //abre o diretorio
    if(d)
    {
        while ((dir = readdir(d)) != NULL && flagArq)
        {                                   //percorre os arquivos do diretorio, lendo apenas os válidos
            if (dir->d_type == DT_REG)
            {
                if (nomeArquivo=="") {
                    nomeArquivoChar= dir->d_name;
                }else{
                    nomeArquivoChar = new char[nomeArquivo.size()+1];
                    copy(nomeArquivo.begin(), nomeArquivo.end(), nomeArquivoChar);
                    nomeArquivoChar[nomeArquivo.size()] = '\0';
                }
                //cout << "Indexando matriz: " << nomeArquivo << endl;
                strcat(caminhoArquivo, nomeArquivoChar); //concatena o nome do arquivo à string para o caminho do arquivo, que até
                                                     //então tem somente o diretório

                arqFase.open(caminhoArquivo, ifstream::binary); //tenta abrir o arquivo e verifica se ocorrreu qualquer
                                                                      //problema no processo
                if(!arqFase.is_open())
                {
                    cout << "Erro - leitura Matriz. ID: " << strerror(errno);
                    arqFase.clear();

                    exit(1);
                }
                flagArq = false;

                arqFase >> info;
                stringstream convert(info);
                convert>>n;
                *tipoMaterial = n;

                //percorre a matriz para inserir os dados
                for (int i = 0; i < numLinhas; i++)
                {
                    for (int j = 0; j < numColunas; j++)
                    {
                        //percorre ate o fim do arquivo e para
                        if(!arqFase.eof())
                        {
                            //le a informação e converte
                            arqFase >> info;
                            stringstream convert(info);
                            convert>>n;

                            //insere na matriz
                            matriz[i][j] = !((bool)n); //0 ou 1
                        }
                        else break;
                    }
                }

                arqFase.close(); //fecha o arquivo
                arqFase.clear();
                strcpy(caminhoArquivo, caminhoDiretorio); //faz com que o caminho do arquivo volte a ser apenas o diretorio
            }
        }
        closedir(d); //fecha o diretorio
    }
    else if(d == NULL)
        cout << "Não encontrou o diretório" << endl;
}
