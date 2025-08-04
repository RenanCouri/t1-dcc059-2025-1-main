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

struct NoLA2 {
    char id;
    int posLista;
    int peso;
    vector<Aresta*> arestasPrim;
    vector<Aresta*> arestasSec; 
    int cobertosTotal=1;
    int cobertosPrim=1;
    int cobertosSec=0;
    bool cobre_a_si=true;
    bool na_solucao=false;

    void alteraCobertPrim(int alt){
        cobertosPrim+=alt;
        cobertosTotal+=alt;
    }
    void alteraCobertSec(int alt){
        cobertosSec+=alt;
        cobertosTotal+=alt;
    }
    void alteraCobertTotal(int alt){
        alteraCobertPrim(alt);
    }
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
    vector<char> dijkstra_padrao_impl(char id_no_a, char id_no_b);
    vector<char> dijkstra_negativo_impl(char id_no_a, char id_no_b);
    bool ehUmNovoNo(char id);
    No* encontrarNo(char id);
    bool inserirNo(char id, int pesoNo);
    bool inserirAresta(char id_org,char id_dest,int pesoArst);
    int posicaoNoNaLista(char id);
    void gerarTransposto();
    bool inserirArestaRetorno(char id_org,char id_dest,int pesoArst);
    Grafo* subGrafoVerticeInduzido(vector<char> ids_nos);
    Grafo* subGrafoVerticeInduzidoMelhorado(vector<char> ids_nos);

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

    //T2
    vector<char> dominanteMin2_guloso();
    vector<char> dominanteMin2_gulosoAdaptRand(double alphas, int repeticoes);
    vector<char> dominanteMin2_gulosoAdaptRandReat(vector<double> alphas,int repeticoes,int reps_para_recal,double exp_sens);

    vector<char> auxiliar_domM_dist2_adaptRand(double alpha,int repeticoes);
    vector<char> auxiliar_domM_dist2_adaptRandReat(vector<double> alphas,vector<double> probabilidades,vector<double> medias,int repeticoes, int reps_para_recal,double exp_sens);


private:
    

    void auxiliarFechos(int pos_no,vector<char>& lista_fecho); 
    static vector<ArestaCompleta> listaArestasOrdenadas(Grafo* grafo);
    void buscaProfundidadeNo(Grafo* prof,No* no,bool* visitado,char id_pai);
    void buscaProfundidadeVistados( No* no,bool* visitado,char id_pai);
    void auxiliar_matrizes_floyd(vector<vector<int>>& matriz_distancias,vector<vector<int>>& matriz_precedentes );
    int excentricidade(vector<int> v,int posicao_ignorar);
    void buscaProfundidadeVistadosExcludente( No* no,bool* visitado,char id_pai,char id_excluido);
    Grafo* criar_versao_nao_direcionada();

    void buscaProfundidadeNivelK(No *no, bool *visitado,char id_pai,int nivel_k, vector<Aresta*> &k_arestas);

    //T2

    void inicializacao_reativo(vector<NoLA2*> &listaAdj2,const vector<int> &primOrig, const vector<int> &secOrig,
        vector<double> &alphas,vector<double> &probabilidades,vector<double> &medias,
        int repeticoes_ini,double exp_sens,vector<char> &solucao_otima,vector<long long> &somas_soluces_alphas,vector<int> &execucoes_alphas);

    int sorteia_indice_alpha(vector<double> &probabilidades);
    void recalcula_probabilidades(vector<double> &probabilidades,int f_sol_otm, vector<double> &medias,double exp_sens);


    void inicializaListaAlgGul(vector<NoLA2*> &listaAdj2, vector<int> &primOrg, vector<int> &secOrg);
    int execucao_alg_guloso_rand_adap(float alpha,vector<NoLA2*> &listaAdj2,const vector<int> &primOrig, const vector<int> &secOrig, vector<char> &solucao_otima, bool primeira_iter);

    
};



#endif //GRAFO_H
