#include "Grafo.h"
#include <iostream>
#include <climits>
#include <queue>
#include <set>
#include <algorithm>
#include <map>  // Para as distâncias e predecessores no Dijkstra

const int INF = INT_MAX;

#define infinito INT_MAX/2 

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


// Criados:
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
    novaAresta->posicao_alvo_lista_adj=posDest;
    lista_adj[posOrg]->arestas.push_back(novaAresta);
    
    if(!in_direcionado){
        Aresta* novaAresta2= new Aresta(id_org); 
        if(in_ponderado_aresta)
            novaAresta2->peso=pesoArst;
        novaAresta2->posicao_alvo_lista_adj=posOrg;    
         lista_adj[posDest]->arestas.push_back(novaAresta2);       
    }   
    transposto_valido=false;
    return true; 
}

    bool Grafo::inserirArestaRetorno(char id_org,char id_dest,int pesoArst){
        return false;
    }

void Grafo::gerarTransposto(){ // Gera grafo transposto para fazer fecho transitivo indireto
    if(!transposto_valido && in_direcionado){
        transposto_valido = true;
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


void Grafo::auxiliarFechos(int pos_no,vector<char>& lista_fecho){
    No* atual = lista_adj[pos_no];
    char id_no=atual->id;
    if (std::find(lista_fecho.begin(), lista_fecho.end(), id_no) == lista_fecho.end()) {
        lista_fecho.push_back(id_no);
        cout<<(atual->arestas.size()!=0);
        for(Aresta* aresta : atual->arestas ){
            auxiliarFechos(aresta->posicao_alvo_lista_adj,lista_fecho);
        }    
    }
    
}

// Fim das que criei, a partir daqui são as funções que já exisitiam. Implementei as de fecho transitivo direto e indireto:
vector<char> Grafo::fecho_transitivo_direto(char id_no) {
    
    if(!in_direcionado){
        cout<<"Grafo não direcionado, não há conceito de fecho"<<endl;
        return {};
    }
    
    int pos_no_inicial=posicaoNoNaLista(id_no);
    if(pos_no_inicial<0){
        cout<<"Nó inicial não existe!!!!"<<endl;
        return {};
    } 
    vector<char> retorno;
    auxiliarFechos(pos_no_inicial,retorno);
       
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




// A partir daqui, ainda devemos implementar:
vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b) {
    if (encontrarNo(id_no_a) == NULL || encontrarNo(id_no_b) == NULL) {
        cout << "Erro: Um dos nos (" << id_no_a << " ou " << id_no_b << ") nao existe no grafo." << endl;
        return {};
    }
    if(!this->in_direcionado){
        cout<<" O grafo não é direcionado, algortimo de caminhamento não se aplica!"<<endl;
        return {};
    }
    if (this->ha_peso_negativo) {
        return this->dijkstra_negativo_impl(id_no_a, id_no_b);
    } else {
        return this->dijkstra_padrao_impl(id_no_a, id_no_b);
    }
}

vector<char> Grafo::dijkstra_padrao_impl(char id_no_a, char id_no_b) {
    map<char, int> distancias;
    map<char, char> predecessores;
    priority_queue<pair<int, char>, vector<pair<int, char>>, greater<pair<int, char>>> pq;
    set<char> visitados; 

    for (No* no : lista_adj) {
        distancias[no->id] = infinito;
        predecessores[no->id] = '\0';
    }

    distancias[id_no_a] = 0;
    pq.push({0, id_no_a});

    while (!pq.empty()) {
        char no_atual_id = pq.top().second;
        pq.pop();

        if (visitados.count(no_atual_id)) continue;
        
        visitados.insert(no_atual_id);
        if (no_atual_id == id_no_b) break;

        No* no_atual = encontrarNo(no_atual_id);
        if (!no_atual) continue;

        for (Aresta* aresta : no_atual->arestas) {
            char vizinho_id = aresta->id_no_alvo;
            int peso_aresta = in_ponderado_aresta ? aresta->peso : 1;
            
            if (distancias[no_atual_id] + peso_aresta < distancias[vizinho_id]) {
                distancias[vizinho_id] = distancias[no_atual_id] + peso_aresta;
                predecessores[vizinho_id] = no_atual_id;
                pq.push({distancias[vizinho_id], vizinho_id});
            }
        }
    }
    
    vector<char> caminho;
    
    char passo_atual = id_no_b;
    while (passo_atual != '\0') {
        caminho.push_back(passo_atual);
        passo_atual = predecessores[passo_atual];
    }
    reverse(caminho.begin(), caminho.end());

    if (caminho.empty() || caminho[0] != id_no_a) {
         cout << "Nao ha caminho entre " << id_no_a << " e " << id_no_b << "." << endl;
         return {};
    }
    return caminho;
}


vector<char> Grafo::dijkstra_negativo_impl(char id_no_a, char id_no_b) {
    map<char, int> distancias;
    map<char, char> predecessores;
    queue<char> fila_para_processar;
    set<char> na_fila;
    
    for (No* no : lista_adj) {
        distancias[no->id] = infinito;
        predecessores[no->id] = '\0';
    }

    distancias[id_no_a] = 0;
    fila_para_processar.push(id_no_a);
    na_fila.insert(id_no_a);

    while (!fila_para_processar.empty()) {
        char no_atual_id = fila_para_processar.front();
        fila_para_processar.pop();
        na_fila.erase(no_atual_id);

        No* no_atual = encontrarNo(no_atual_id);
        if (!no_atual) continue;

        for (Aresta* aresta : no_atual->arestas) {
            char vizinho_id = aresta->id_no_alvo;
            int peso_aresta = in_ponderado_aresta ? aresta->peso : 1;

            if (distancias[no_atual_id] + peso_aresta < distancias[vizinho_id]) {
                distancias[vizinho_id] = distancias[no_atual_id] + peso_aresta;
                predecessores[vizinho_id] = no_atual_id;
                
                if (na_fila.find(vizinho_id) == na_fila.end()) {
                    fila_para_processar.push(vizinho_id);
                    na_fila.insert(vizinho_id);
                }
            }
        }
    }
    
    vector<char> caminho;    
    char passo_atual = id_no_b;
    while (passo_atual != '\0') {
        caminho.push_back(passo_atual);
        passo_atual = predecessores[passo_atual];
    }
    reverse(caminho.begin(), caminho.end());
    
    if (caminho.empty() || caminho[0] != id_no_a) {
         cout << "Nao ha caminho entre " << id_no_a << " e " << id_no_b << "." << endl;
         return {};
    }
    return caminho;
}

void Grafo::auxiliar_matrizes_floyd(vector<vector<int>>& matriz_distancias,vector<vector<int>>& matriz_precedentes ){
    for(int i=0;i<ordem;i++){
        for(int j=0;j<ordem;j++){
            if(i==j)
                matriz_distancias[i][j]=0;
            else
                matriz_distancias[i][j]=INF;
                   
        }
    }


            
    for(int i=0;i<ordem;i++){
        std::vector<Aresta*> arestas_no=lista_adj[i]->arestas;
        for(Aresta* aresta: arestas_no){
            if(aresta->peso<matriz_distancias[i][aresta->posicao_alvo_lista_adj])
                matriz_distancias[i][aresta->posicao_alvo_lista_adj]=aresta->peso;
        }
    }

    for (int i = 0; i < ordem; i++) {
        for (int j = 0; j < ordem; j++) {
            if (i != j && matriz_distancias[i][j] != INF)
                matriz_precedentes[i][j] = i;
            else
                matriz_precedentes[i][j] = -1;
        }
    }
    

    int caminho_alt1=0,caminho_alt2=0;
    for(int k=0;k<ordem;k++){
        for(int i=0;i<ordem;i++){
            
                for(int j=0;j<ordem;j++){
                    
                        caminho_alt1=matriz_distancias[i][k];
                        caminho_alt2=matriz_distancias[k][j];
                        if(caminho_alt1!=INF && caminho_alt2!=INF &&(
                            matriz_distancias[i][j]>(caminho_alt1+caminho_alt2)
                        ))
                        {
                            matriz_distancias[i][j]=caminho_alt1+caminho_alt2;
                            matriz_precedentes[i][j]=matriz_precedentes[k][j];;
                        }
                    
                }
               
        }
    }
}


vector<char> Grafo::caminho_minimo_floyd(char id_no, char id_no_b) {
    if(!this->in_direcionado ){
        cout<<"ERRO!!!!! Grafo não direcionado"<<endl;
        return {};
    }
    if(!this->in_ponderado_aresta){
        cout<<"AVISO: grafo não ponderado nas arestas. Considera-se peso de cada uma como 1"<<endl;
    }
    int posA=posicaoNoNaLista(id_no);
    int posB=posicaoNoNaLista(id_no_b);

    if(posA==-1 || posB==-1){
        cout<<"ERRO!!!!! Um dos ids não está no grafo!"<<endl;
        return{};
    }
    std::vector<std::vector<int>> matriz_distancias(this->ordem, std::vector<int>(this->ordem));
    std::vector<std::vector<int>> matriz_precedentes(this->ordem, std::vector<int>(this->ordem));
   
    auxiliar_matrizes_floyd(matriz_distancias,matriz_precedentes);
    for(int i=0;i<ordem;i++){
        cout<<lista_adj[i]->id;
        for(int j=0;j<ordem;j++){
            if(matriz_precedentes[i][j]!=-1){
                cout<<"  "<<lista_adj[j]->id<<"Dist : ";
                cout<<matriz_distancias[i][j]<<" Precedente: ";
                cout<<matriz_precedentes[i][j];
            }
            
        }
        cout<<endl;
    }
    cout<<endl;
    vector<char> caminho_minimo;
    if(matriz_precedentes[posA][posB]==-1){
        cout<<"Não há conexão entre os nós"<<endl;
        return {};
    }

    caminho_minimo.push_back(id_no_b);
    int atual=matriz_precedentes[posA][posB];
    while(atual!=posA){
        caminho_minimo.push_back(lista_adj[atual]->id);
        cout<<matriz_precedentes[posA][atual]<<endl;
        atual=matriz_precedentes[posA][atual];
    }
    caminho_minimo.push_back(id_no);
    std::reverse(caminho_minimo.begin(), caminho_minimo.end());

    return caminho_minimo;
}


Grafo* Grafo::subGrafoVerticeInduzido(vector<char> ids_nos){
    int ordemSubGrafo=ids_nos.size();
    vector<char> utilizados(ordemSubGrafo);
    Grafo* subGrafo = new Grafo(this->in_direcionado,this->in_ponderado_vertice,this->in_ponderado_aresta);
    for(No* no : lista_adj){
        auto iterador=std::find(ids_nos.begin(), ids_nos.end(), no->id);
        if(iterador != ids_nos.end()){
            No* novoNo = new No(no->id);
            novoNo->peso=no->peso;
            if (iterador != ids_nos.end() - 1)
                std::iter_swap(iterador, ids_nos.end() - 1);
            ids_nos.pop_back();
            utilizados.push_back(no->id);
            for(Aresta* aresta : no->arestas){
                iterador=std::find(ids_nos.begin(), ids_nos.end(), aresta->id_no_alvo);
                if(iterador!=ids_nos.end()){
                    Aresta* novaAresta= new Aresta(aresta->id_no_alvo);
                    novaAresta->peso=aresta->peso;
                    novoNo->arestas.push_back(novaAresta);
                    
                }
                else if(!this->in_direcionado && std::find(utilizados.begin(), utilizados.end(), aresta->id_no_alvo)!=utilizados.end()){
                    Aresta* novaAresta= new Aresta(aresta->id_no_alvo);
                    novaAresta->peso=aresta->peso;
                    
                    novoNo->arestas.push_back(novaAresta);
                }
            }
            
            subGrafo->lista_adj.push_back(novoNo);
            
        }
        
    }
    subGrafo->ordem=ordemSubGrafo;
    return subGrafo;
}


Grafo* Grafo::subGrafoVerticeInduzidoMelhorado(vector<char> ids_nos) {
    Grafo* subGrafo = new Grafo(this->in_direcionado, this->in_ponderado_vertice, this->in_ponderado_aresta);
    subGrafo->ordem = ids_nos.size();
    // Armazena os IDs válidos encontrados no grafo original
    vector<char> ids_encontrados;
    vector<int> posicoes_correspondetes_grafo_no_subGrafo(ordem);
    vector<int> posicoes_correspondetes_subGrafo_no_grafo(subGrafo->ordem);
    int contagem=0;
    int contagem_subgrafo=0;
    for (No* no : lista_adj) {
        auto iterador=std::find(ids_nos.begin(), ids_nos.end(), no->id);
        if (iterador != ids_nos.end()) {
            No* novoNo = new No(no->id);
            novoNo->peso = no->peso;
            if (iterador != ids_nos.end() - 1)
                std::iter_swap(iterador, ids_nos.end() - 1);
            ids_nos.pop_back();


            subGrafo->lista_adj.push_back(novoNo);
            
            posicoes_correspondetes_grafo_no_subGrafo[contagem]=contagem_subgrafo;
            posicoes_correspondetes_subGrafo_no_grafo[contagem_subgrafo]=contagem;
            contagem_subgrafo++;
        }
        else{
            posicoes_correspondetes_grafo_no_subGrafo[contagem]=-1;
        }
        contagem++;
    }
    
    for( int i=0;i<contagem_subgrafo;i++ ){
        vector<Aresta*> arestas_grafo = this->lista_adj[posicoes_correspondetes_subGrafo_no_grafo[i]]->arestas;
        for(Aresta* aresta: arestas_grafo){
            if(posicoes_correspondetes_grafo_no_subGrafo[aresta->posicao_alvo_lista_adj]!=-1){
                Aresta *novaAresta = new Aresta(aresta->id_no_alvo);
                novaAresta->peso=aresta->peso;
                novaAresta->posicao_alvo_lista_adj=posicoes_correspondetes_grafo_no_subGrafo[aresta->posicao_alvo_lista_adj];
                subGrafo->lista_adj[i]->arestas.push_back(novaAresta);
            }

        }
    }

    subGrafo->ordem = subGrafo->lista_adj.size();
    return subGrafo;
}


std::vector<ArestaCompleta> Grafo::listaArestasOrdenadas(Grafo* grafo) {
    std::vector<ArestaCompleta> arestasOrdenadas;

    for (No* no : grafo->lista_adj) {
        char idOrigem = no->id;

        for (Aresta* aresta : no->arestas) {
            char idDestino = aresta->id_no_alvo;

            // Se for grafo direcionado, inclui direto;
            // Se for não-direcionado, inclui apenas uma vez (origem <= destino evita duplicatas)
            if (grafo->in_direcionado || idOrigem <= idDestino) {
                ArestaCompleta nova;
                nova.origem = idOrigem;
                nova.destino = idDestino;
                nova.peso = aresta->peso;
                arestasOrdenadas.push_back(nova);
            }
        }
    }

    std::sort(arestasOrdenadas.begin(), arestasOrdenadas.end(), [](ArestaCompleta a, ArestaCompleta b) {
        return a.peso > b.peso;  // ordena em ordem decrescente
    });

    return arestasOrdenadas;
}

Grafo *Grafo::arvore_geradora_minima_prim(vector<char> ids_nos)
{
    Grafo *agm = new Grafo(false, in_ponderado_vertice, in_ponderado_aresta);
    if (ids_nos.empty())
        return agm;

    priority_queue<pair<int, pair<char, char>>, vector<pair<int, pair<char, char>>>, greater<>> pq;
    set<char> incluidos;

    // Inicia com o primeiro nó
    char primeiro = ids_nos[0];
    incluidos.insert(primeiro);
    agm->inserirNo(primeiro, encontrarNo(primeiro)->peso);

    // Adiciona arestas do primeiro nó
    for (Aresta *aresta : encontrarNo(primeiro)->arestas)
        if (find(ids_nos.begin(), ids_nos.end(), aresta->id_no_alvo) != ids_nos.end())
            pq.push({aresta->peso, {primeiro, aresta->id_no_alvo}});

    while (!pq.empty())
    {
        auto [peso, aresta] = pq.top();
        pq.pop();
        char u = aresta.first, v = aresta.second;

        if (incluidos.count(v))
            continue;

        incluidos.insert(v);
        agm->inserirNo(v, encontrarNo(v)->peso);
        agm->inserirAresta(u, v, peso);

        // Adiciona arestas do novo nó
        for (Aresta *a : encontrarNo(v)->arestas)
            if (find(ids_nos.begin(), ids_nos.end(), a->id_no_alvo) != ids_nos.end())
                pq.push({a->peso, {v, a->id_no_alvo}});
    }
    return agm;
}  

Grafo * Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos) {
    
    
    Grafo * subGrafoVertInd= this->subGrafoVerticeInduzidoMelhorado(ids_nos);
    if(this->in_direcionado){
        cout<<"ERRO!!!! Grafo é direcionado, arvores geradoras são para grafos não direcionados!"<<endl;
        return nullptr;
    }
    if(subGrafoVertInd->ordem==0){
        cout<<"ERRO!!!!!! Não foi capaz de gerar subGrafo com estes ids"<<endl;
        return nullptr;
    }
    
    bool *visitado= new bool[subGrafoVertInd->ordem];
    visitado[0]=true;
    for(int i=1;i<subGrafoVertInd->ordem;i++){
        visitado[i]=false;
    }
    
    cout<<"AA"<<endl;
    subGrafoVertInd->buscaProfundidadeVistados(subGrafoVertInd->lista_adj[0],visitado,'\0');
    cout<<"BB"<<endl;
    for(int i=1;i<subGrafoVertInd->ordem;i++){
        if(!visitado[i]){
            cout<<"Subgrafo vétice induzido gerado é desconexo!!!!!! Não é possível, portanto, gerar arvore geradora mínima!"<<endl;
            delete [] visitado;
            return  nullptr;
        }
    }
    delete [] visitado;
     
    Grafo * arvoreGeradoraMin= new Grafo(this->in_direcionado,this->in_ponderado_vertice,this->in_ponderado_aresta);
    vector<ArestaCompleta> listaArestasOrdenad=listaArestasOrdenadas(subGrafoVertInd);
    vector<vector<char>> nosJuntos;


    for(No* no:subGrafoVertInd->lista_adj){
        No* novoNo= new No(no->id);
        novoNo->peso=no->peso;
        arvoreGeradoraMin->lista_adj.push_back(novoNo);
    }
    arvoreGeradoraMin->ordem=subGrafoVertInd->ordem;
    int contador=0;
    while(contador<subGrafoVertInd->ordem-1 && !listaArestasOrdenad.empty()){
        
        ArestaCompleta ar= listaArestasOrdenad.back();
        listaArestasOrdenad.pop_back();
        bool achou1=false;
        bool achou2NaMesma=false;
        bool terminar=false;
        int pos1=-1;
        int pos2=-1;
        int contadorLocalNos=0;
        char naoAchado='\0';
        for(vector<char> nos : nosJuntos){
            for(char id : nos){
                if(id==ar.origem || id==ar.destino){
                    if(achou1){
                        achou2NaMesma=true;
                        terminar=true;
                        break;
                    }
                    achou1=true;    
                    if(pos1==-1){
                        pos1=contadorLocalNos;
                        if(ar.origem==id)
                            naoAchado=ar.destino;
                        else
                            naoAchado=ar.origem;    
                    }    
                    else{
                        pos2=contadorLocalNos;  
                        terminar=true;
                        break;
                    }      
                }

            }
            contadorLocalNos++;
            
            if(terminar)
                break;
            achou1=false;    
                
        }
        if(!achou2NaMesma){
            arvoreGeradoraMin->inserirAresta(ar.origem,ar.destino,ar.peso);
            contador++;
            if(pos1==-1){
                vector<char> novoConj;
                novoConj.push_back(ar.origem);
                novoConj.push_back(ar.destino);
                nosJuntos.push_back(novoConj);
            }
            else{
                if(pos2==-1)
                    (nosJuntos[pos1]).push_back(naoAchado);
                else{
                    vector<char> *juncaoExcluidaParaFusao=&nosJuntos[pos2]; 
                    vector<char> *fusaoNosJuntos= &nosJuntos[pos1];
                    for(char no: *juncaoExcluidaParaFusao)
                        fusaoNosJuntos->push_back(no);
                    nosJuntos.erase(nosJuntos.begin()+pos2);   
                }    
            }
        }
        
    }
    delete subGrafoVertInd;
    return arvoreGeradoraMin;
}

void Grafo::buscaProfundidadeNo(Grafo* prof, No* no,bool* visitado,char id_pai){
    No* novoNo= new No(no->id);
    novoNo->peso=no->peso;
    No* novoNoRet= new No(no->id);
    novoNoRet->peso=no->peso;
    prof->lista_adj.push_back(novoNo);
    prof->ordem++;
    prof->lista_adj_retorno.push_back(novoNoRet);
    int pos_lista=0;
    for(Aresta* aresta : no->arestas){
        pos_lista=aresta->posicao_alvo_lista_adj;
        Aresta* ar= new Aresta(aresta->id_no_alvo);
        ar->peso=aresta->peso;
        ar->posicao_alvo_lista_adj=pos_lista;
        if(!visitado[pos_lista]){
            visitado[pos_lista]=true;
            novoNo->arestas.push_back(ar);
            buscaProfundidadeNo(prof,lista_adj[pos_lista],visitado,no->id);
        }
        else{
            if(!this->in_direcionado && id_pai==aresta->id_no_alvo){
                novoNo->arestas.push_back(ar);
            }
            else{
                novoNoRet->arestas.push_back(ar);
            }
        }
    }
}


void Grafo::buscaProfundidadeVistados( No* no,bool* visitado,char id_pai){
    No* novoNo= new No(no->id);
    novoNo->peso=no->peso;
    int pos_lista=0;
    for(Aresta* aresta : no->arestas){
        pos_lista=aresta->posicao_alvo_lista_adj;
        Aresta* ar= new Aresta(aresta->id_no_alvo);
        ar->peso=aresta->peso;
        ar->posicao_alvo_lista_adj=pos_lista;
        if(!visitado[pos_lista]){
            visitado[pos_lista]=true;
            novoNo->arestas.push_back(ar);
            buscaProfundidadeVistados(lista_adj[pos_lista],visitado,no->id);
        }
        else{
            if(!this->in_direcionado && id_pai==aresta->id_no_alvo){
                novoNo->arestas.push_back(ar);
            }
        }
    }
    delete novoNo;
}

Grafo * Grafo::arvore_caminhamento_profundidade(char id_no) {
    int pos_lis=this->posicaoNoNaLista(id_no);
     
    if(pos_lis==-1){
        cout<<"Id inválido passado!!!"<<endl;
         return nullptr;
    }
    Grafo* profundidade= new Grafo(this->in_direcionado,this->in_ponderado_vertice,this->in_ponderado_aresta);
    bool *visitado= new bool[ordem];
    for(int i=0;i<ordem;i++)
        visitado[i]=false;
    visitado[pos_lis]=true;
    No* no = lista_adj[pos_lis];
    buscaProfundidadeNo(profundidade,no,visitado,'\0');    
    delete [] visitado;
    return profundidade;
    
}

int Grafo::excentricidade(vector<int> v){
    if(v.size()==0)
        return 0;
    int maior=INT_MIN;
    for(int valor : v){
        if( valor>maior)
            maior=valor;
    }
    if(maior==INT_MIN)
        maior=INF;
    return maior;

}



int Grafo::raio() {
    std::vector<std::vector<int>> matriz_distancias(this->ordem, std::vector<int>(this->ordem));
    std::vector<std::vector<int>> matriz_precedentes(this->ordem, std::vector<int>(this->ordem));
    auxiliar_matrizes_floyd(matriz_distancias,matriz_precedentes);
    int raio=INF;
    int exc_atual=0;
    for(vector<int> distancias : matriz_distancias){
        exc_atual=excentricidade(distancias);
        if(exc_atual<raio)
            raio=exc_atual;
    }
    if(raio==INF)
        cout<<"Gráfo é desconexo, logo raio não se aplica, é infinito!"<<endl;
    return raio;
}

int Grafo::diametro() {
    std::vector<std::vector<int>> matriz_distancias(this->ordem, std::vector<int>(this->ordem));
    std::vector<std::vector<int>> matriz_precedentes(this->ordem, std::vector<int>(this->ordem));
    auxiliar_matrizes_floyd(matriz_distancias,matriz_precedentes);
    int diametro=INT_MIN;
    int exc_atual=0;
    for(vector<int> distancias : matriz_distancias){
        exc_atual=excentricidade(distancias);
        if(exc_atual>diametro && exc_atual!=INF)
            diametro=exc_atual;
    }
    if(diametro==INT_MIN){
       cout<<"Gráfo é desconexo, logo diâmetro não se aplica, é infinito!"<<endl;
       diametro=INF;
    }
        
    return diametro;
}

vector<char> Grafo::centro() {
    std::vector<std::vector<int>> matriz_distancias(this->ordem, std::vector<int>(this->ordem));
    std::vector<std::vector<int>> matriz_precedentes(this->ordem, std::vector<int>(this->ordem));
    auxiliar_matrizes_floyd(matriz_distancias,matriz_precedentes);
    int raio=INF;
    vector<int> vetor_excentricidades(ordem);
    int i=0;
    for(vector<int> distancias : matriz_distancias){
        
        vetor_excentricidades[i]=excentricidade(distancias);
        if(vetor_excentricidades[i]<raio)
            raio=vetor_excentricidades[i];
        i++;    
    }
    if(raio==INF){
        cout<<"Gráfo é desconexo, logo raio não se aplica, é infinito!"<<endl;
        return {};
    }    
    vector<char> centro;
    for(int i=0;i<ordem;i++){
        if(vetor_excentricidades[i]==raio)
            centro.push_back(lista_adj[i]->id);
    }
    




    return centro;
}

vector<char> Grafo::periferia() {
    std::vector<std::vector<int>> matriz_distancias(this->ordem, std::vector<int>(this->ordem));
    std::vector<std::vector<int>> matriz_precedentes(this->ordem, std::vector<int>(this->ordem));
    auxiliar_matrizes_floyd(matriz_distancias,matriz_precedentes);
    int diametro=INT_MIN;
    vector<int> vetor_excentricidades(ordem);
    int i=0;
    for(vector<int> distancias : matriz_distancias){
        vetor_excentricidades[i]=excentricidade(distancias);
        if(vetor_excentricidades[i]>diametro && vetor_excentricidades[i]!=INF)
            diametro=vetor_excentricidades[i];
        i++;    
    }
    if(diametro==INT_MIN){
       cout<<"Gráfo é desconexo, logo diâmetro não se aplica, é infinito!"<<endl;
       return {};
    }
    cout<<diametro;       
    vector<char> periferia;
    for(int i=0;i<ordem;i++){
        cout<<vetor_excentricidades[i]<<endl;
        if(vetor_excentricidades[i]==diametro)
            periferia.push_back(lista_adj[i]->id);
    }
    



    return periferia;
}

void Grafo::buscaProfundidadeVistadosExcludente( No* no,bool* visitado,char id_pai,char id_excluido){

    int pos_lista=0;
    for(Aresta* aresta : no->arestas){
        pos_lista=aresta->posicao_alvo_lista_adj;
        if(aresta->id_no_alvo!=id_excluido && !visitado[pos_lista]){
            visitado[pos_lista]=true;
            buscaProfundidadeVistadosExcludente(lista_adj[pos_lista],visitado,no->id,id_excluido);
        }
    }
}

vector<char> Grafo::vertices_de_articulacao() {
    vector<char> v_art;
    if(ordem==0)
        return {};
    if(ordem==1){
        v_art.push_back(lista_adj[0]->id);
    }  
    else{
                if(!this->in_direcionado){
                bool *visitados = new bool[ordem];
                visitados[0]=true;
                for(int i=1;i<ordem;i++)
                    visitados[i]=false;
                buscaProfundidadeVistadosExcludente(lista_adj[1],visitados,'\0',lista_adj[0]->id);
                int k=0;
                for(;k<ordem;k++){
                    if(k==0) continue;
                    if(!visitados[k]){
                        
                        v_art.push_back(lista_adj[0]->id);
                        break;
                    }  
                    visitados[k]=false;
                }    
                for(k=k+1;k<ordem;k++)
                    visitados[k]=false;
                k=0;    
                for(int i=1;i<ordem;i++){
                    buscaProfundidadeVistadosExcludente(lista_adj[0],visitados,'\0',lista_adj[i]->id);
                    visitados[i]=true;
                    for(;k<ordem;k++){
                        if(!visitados[k]){
                        
                            v_art.push_back(lista_adj[i]->id);
                            break;
                        }  
                        visitados[k]=false;
                    }    
                    for(k=k+1;k<ordem;k++)
                        visitados[k]=false;
                    k=0;
                }  
                    

            }

            else{
                this->gerarTransposto();
                bool *visitados1 = new bool[ordem];
                bool *visitados2 = new bool[ordem];
                visitados1[0]=true;
                visitados2[0]=true;
                for(int i=1;i<ordem;i++){
                    visitados1[i]=false;
                    visitados2[i]=false;
                }
                this->buscaProfundidadeVistadosExcludente(lista_adj[1],visitados1,'\0',lista_adj[0]->id);
                transposto->buscaProfundidadeVistadosExcludente(transposto->lista_adj[1],visitados2,'\0',transposto->lista_adj[0]->id);
                int k=0;
                for(;k<ordem;k++){
                    if(k==0) continue;
                    if(!visitados1[k] && visitados2[k]){
                        
                        v_art.push_back(lista_adj[0]->id);
                        break;
                    }  
                    visitados1[k]=false;
                    visitados2[k]=false;
                }    
                for(k=k+1;k<ordem;k++){
                    visitados1[k]=false;
                    visitados2[k]=false;
                }
                k=0;    
                for(int i=1;i<ordem;i++){
                    buscaProfundidadeVistadosExcludente(lista_adj[0],visitados1,'\0',lista_adj[i]->id);
                    transposto->buscaProfundidadeVistadosExcludente(transposto->lista_adj[0],visitados2,'\0',transposto->lista_adj[i]->id);
                    visitados1[i]=true;
                    visitados2[i]=true;
                    for(;k<ordem;k++){
                        if(!visitados1[k] && visitados2[k]){
                        
                            v_art.push_back(lista_adj[i]->id);
                            break;
                        }  
                         visitados1[k]=false;
                    visitados2[k]=false;
                }    
                for(k=k+1;k<ordem;k++){
                    visitados1[k]=false;
                    visitados2[k]=false;
                }
                    k=0;
                }  
            }
}    
return v_art;
}


