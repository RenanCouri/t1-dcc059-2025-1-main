#include "Grafo.h"
#include <iostream>

Grafo::Grafo() {
    in_direcionado=false;
    in_ponderado_vertice=false;
    in_ponderado_aresta=false;
    ha_peso_negativo=false;
    transposto_valido=false;
    transposto=NULL;
    ordem=0;
}

Grafo::Grafo(bool direcionado,bool ponderadoVertice,bool ponderadoAresta){
    in_direcionado=direcionado;
    in_ponderado_vertice=ponderadoVertice;
    in_ponderado_aresta=ponderadoAresta;
    ha_peso_negativo=false;
    transposto_valido=false;
    transposto=NULL;
    ordem=0;
}

Grafo::~Grafo() {
    for (No* no : lista_adj) {
        for (Aresta* aresta : no->arestas) {
            delete aresta;
        }
        delete no;
    }
    if(transposto!=NULL)
        delete transposto;
}

bool Grafo::ehUmNovoNo(char id){
    return posicaoNoNaLista(id)==-1;
}

int Grafo::posicaoNoNaLista(char id){
    int i=0;
    for (No* no : lista_adj) {
        
        if (no->id == id)
            return i;   
        i++;    
    }
    return -1;
}   

No* Grafo::encontrarNo(char id){
    for (No* no : lista_adj) {
        if (no->id == id)
            return no;
    }
    return NULL;
}

bool Grafo::inserirNo(char id, int pesoNo){
    if(!ehUmNovoNo(id))
    {
        cout<<"Nó já presente no Grafo de id: "<<id<<endl;
        return false;
    }
    No* novoNo=new No(id);
    if(in_ponderado_vertice)
        novoNo->peso=pesoNo;   

    lista_adj.push_back(novoNo);
    ordem++;
    transposto_valido=false;
    return true;
}

bool Grafo::inserirAresta(char id_org,char id_dest,int pesoArst){
    int posOrg=posicaoNoNaLista(id_org);
    int posDest=posicaoNoNaLista(id_dest);
    
    if(posOrg==-1 || posDest==-1)
        return false;
    Aresta* novaAresta= new Aresta(id_dest);    
    if(in_ponderado_aresta){
        novaAresta->peso=pesoArst;
        if(!ha_peso_negativo && pesoArst<0)
            ha_peso_negativo=true;
    }
    lista_adj[posOrg]->arestas.push_back(novaAresta);
    cout<<"ABA"<<endl;
    if(!in_direcionado){
        Aresta* novaAresta2= new Aresta(id_org); 
        if(in_ponderado_aresta)
            novaAresta2->peso=pesoArst;
         lista_adj[posDest]->arestas.push_back(novaAresta2);       
    }   
    transposto_valido=false;
    return true; 
}


void Grafo::gerarTransposto(){
    if(!transposto_valido && in_direcionado){
        if(transposto!=NULL)
            delete transposto;
        transposto = new Grafo(this->in_direcionado,this->in_ponderado_vertice,this->in_ponderado_aresta);
        for(No* no: lista_adj){
            No* novoNo = new No(no->id);
            if(in_direcionado)
                novoNo->peso=no->peso;
            transposto->lista_adj.push_back(novoNo);    
        }
        for(No* no: lista_adj){
            for(Aresta* aresta: no->arestas){
                transposto->inserirAresta(aresta->id_no_alvo,no->id,aresta->peso);
            }
        }
    }
}


void Grafo::auxiliarFechos(char id_no,vector<char>& lista_fecho){
    No* atual = encontrarNo(id_no);
    if(atual!=NULL){
        
        if (std::find(lista_fecho.begin(), lista_fecho.end(), id_no) == lista_fecho.end()) {
            lista_fecho.push_back(id_no);
            cout<<(atual->arestas.size()!=0);
            for(Aresta* aresta : atual->arestas ){
                
                auxiliarFechos(aresta->id_no_alvo,lista_fecho);

            }    
        }
    }
}


vector<char> Grafo::fecho_transitivo_direto(char id_no) {
    
    if(!in_direcionado){
        cout<<"Grafo não direcionado, não há conceito de fecho"<<endl;
        return {};
    }
    vector<char> retorno;

    auxiliarFechos(id_no,retorno);
    if(retorno.size()==0){
        cout<<"Nó não existe!!!!"<<endl;
        return {};
    }    
    return retorno;
}

vector<char> Grafo::fecho_transitivo_indireto(char id_no) {
    if(!in_direcionado){
        cout<<"Grafo não direcionado, não há conceito de fecho"<<endl;
        return {};
    }
    
    
    vector<char> retorno;
    gerarTransposto();
    retorno=transposto->fecho_transitivo_direto(id_no);
        
    return retorno;
}

vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::caminho_minimo_floyd(char id_no, char id_no_b) {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

Grafo * Grafo::arvore_geradora_minima_prim(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

Grafo * Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

Grafo * Grafo::arvore_caminhamento_profundidade(char id_no) {
    cout<<"Metodo nao implementado"<<endl;
    return nullptr;
}

int Grafo::raio() {
    cout<<"Metodo nao implementado"<<endl;
    return 0;
}

int Grafo::diametro() {
    cout<<"Metodo nao implementado"<<endl;
    return 0;
}

vector<char> Grafo::centro() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::periferia() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}

vector<char> Grafo::vertices_de_articulacao() {
    cout<<"Metodo nao implementado"<<endl;
    return {};
}


