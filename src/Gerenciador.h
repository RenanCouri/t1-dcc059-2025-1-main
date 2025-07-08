#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include <iostream>
#include "Grafo.h"
#include <algorithm>

using namespace std;
class Gerenciador {
public:
    static void comandos(Grafo* grafo,string nomeArquivo);
    static char get_id_entrada();
    static vector<char> get_conjunto_ids(Grafo* grafo, int tam);
    static bool pergunta_imprimir_arquivo(string nome_arquivo);
    static void imprimirGrafo(Grafo* grafo);
private:
    // Criei esta para imprimir a lista de fechos. Ainda falta colocar uma para imprimir no formato de grafo, seja baseando-se num grafo todo ou num grafo e numa lista dessas:
    static void imprimirConjuntoChar(vector<char> conjunto);  
    static void imprimirGrafoPorIndices(Grafo* grafo,vector<char>& conjunto);  
        static void imprimeArquivo_a_ate_d(vector<char> resultado, string nome,string nome_arquivo_grafo);
    static void imprimeArquivo_e_ate_g(Grafo* grafo, string nome,string nome_arquivo_grafo);
    static void imprimeArquivo_h(int raio, int diametro, vector<char> centro, vector<char> periferia, string nome,string nome_arquivo_grafo);
};


#endif //GERENCIADOR_H
