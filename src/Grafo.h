//
// Created by Rafael on 28/05/2025.
//

#ifndef GRAFO_H
#define GRAFO_H

#include "No.h"
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct ArestaCompleta {
    char origem;
    char destino;
    int peso;
};

class Grafo {
public:
    Grafo();
    ~Grafo();
    //Criei este construtor:
    Grafo(bool direcionado,bool ponderadoVertice,bool ponderadoAresta);

    vector<char> fecho_transitivo_direto(char id_no); // a
    vector<char> fecho_transitivo_indireto(char id_no); // b
    vector<char> caminho_minimo_dijkstra(char id_no_a, char id_no_b); // c
    vector<char> caminho_minimo_floyd(char id_no, char id_no_b); // d
    Grafo* arvore_geradora_minima_prim(vector<char> ids_nos); // e
    Grafo* arvore_geradora_minima_kruskal(vector<char> ids_nos); // f
    Grafo* arvore_caminhamento_profundidade(char id_no); // g
    int raio(); // h 1
    int diametro(); // h 2
    vector<char> centro(); // h 3
    vector<char> periferia(); // h 4
    vector<char> vertices_de_articulacao(); // i

    // Criei estas:
    bool ehUmNovoNo(char id);
    No* encontrarNo(char id);
    bool inserirNo(char id, int pesoNo);
    bool inserirAresta(char id_org,char id_dest,int pesoArst);
    int posicaoNoNaLista(char id);
    void gerarTransposto();
    bool inserirArestaRetorno(char id_org,char id_dest,int pesoArst);
    Grafo* subGrafoVerticeInduzido(vector<char> ids_nos);

    int ordem;
    bool in_direcionado;
    bool in_ponderado_aresta;
    bool in_ponderado_vertice;
    vector<No*> lista_adj;
    vector<No*> lista_adj_retorno;

    //Criei Estes:
    bool ha_peso_negativo;
    
    bool transposto_valido=false;
    Grafo* transposto;


private:
    

    void auxiliarFechos(char id_no,vector<char>& lista_fecho); 
    static vector<ArestaCompleta> listaArestasOrdenadas(Grafo* grafo);
    void buscaProfundidadeNo(Grafo* prof,No* no,bool*visitado,char id_pai);

};



#endif //GRAFO_H
