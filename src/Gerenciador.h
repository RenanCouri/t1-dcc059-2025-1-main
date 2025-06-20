#ifndef GERENCIADOR_H
#define GERENCIADOR_H

#include <iostream>
#include "Grafo.h"
#include <algorithm>

using namespace std;
class Gerenciador {
public:
    static void comandos(Grafo* grafo);
    static char get_id_entrada();
    static vector<char> get_conjunto_ids(Grafo* grafo, int tam);
    static bool pergunta_imprimir_arquivo(string nome_arquivo);
    static void imprimirGrafo(Grafo* grafo);
private:
    // Criei esta para imprimir a lista de fechos. Ainda falta colocar uma para imprimir no formato de grafo, seja baseando-se num grafo todo ou num grafo e numa lista dessas:
    static void imprimirConjuntoChar(vector<char> conjunto);  
    static void imprimirGrafoPorIndices(Grafo* grafo,vector<char>& conjunto);  
};


#endif //GERENCIADOR_H
