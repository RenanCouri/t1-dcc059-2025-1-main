#include "Grafo.h"
#include <iostream>
#include <queue>
#include <set>
#include <algorithm>
#include <map>  // Para as distâncias e predecessores no Dijkstra

#define infinito INT_MAX/2 

Grafo::Grafo()
{
    in_direcionado = false;
    in_ponderado_vertice = false;
    in_ponderado_aresta = false;
    ha_peso_negativo = false;
    transposto_valido = false;
    transposto = NULL;
    ordem = 0;
}

Grafo::Grafo(bool direcionado, bool ponderadoVertice, bool ponderadoAresta)
{
    in_direcionado = direcionado;
    in_ponderado_vertice = ponderadoVertice;
    in_ponderado_aresta = ponderadoAresta;
    ha_peso_negativo = false;
    transposto_valido = false;
    transposto = NULL;
    ordem = 0;
}

Grafo::~Grafo()
{
    for (No *no : lista_adj)
    {
        for (Aresta *aresta : no->arestas)
        {
            delete aresta;
        }
        delete no;
    }
    if (transposto != NULL)
        delete transposto;
}

// Criados:
bool Grafo::ehUmNovoNo(char id)
{
    return posicaoNoNaLista(id) == -1;
}

int Grafo::posicaoNoNaLista(char id)
{
    int i = 0;
    for (No *no : lista_adj)
    {

        if (no->id == id)
            return i;
        i++;
    }
    return -1;
}

No *Grafo::encontrarNo(char id)
{
    for (No *no : lista_adj)
    {
        if (no->id == id)
            return no;
    }
    return NULL;
}

bool Grafo::inserirNo(char id, int pesoNo)
{
    if (!ehUmNovoNo(id))
    {
        cout << "Nó já presente no Grafo de id: " << id << endl;
        return false;
    }
    No *novoNo = new No(id);
    if (in_ponderado_vertice)
        novoNo->peso = pesoNo;

    lista_adj.push_back(novoNo);
    ordem++;
    transposto_valido = false;
    return true;
}

bool Grafo::inserirAresta(char id_org, char id_dest, int pesoArst)
{
    int posOrg = posicaoNoNaLista(id_org);
    int posDest = posicaoNoNaLista(id_dest);

    if (posOrg == -1 || posDest == -1)
        return false;
    Aresta *novaAresta = new Aresta(id_dest);
    if (in_ponderado_aresta)
    {
        novaAresta->peso = pesoArst;
        if (!ha_peso_negativo && pesoArst < 0)
            ha_peso_negativo = true;
    }
    lista_adj[posOrg]->arestas.push_back(novaAresta);

    if (!in_direcionado)
    {
        Aresta *novaAresta2 = new Aresta(id_org);
        if (in_ponderado_aresta)
            novaAresta2->peso = pesoArst;
        lista_adj[posDest]->arestas.push_back(novaAresta2);
    }
    transposto_valido = false;
    return true;
}

bool Grafo::inserirArestaRetorno(char id_org, char id_dest, int pesoArst)
{
    return false;
}

void Grafo::gerarTransposto()
{ // Gera grafo transposto para fazer fecho transitivo indireto
    if (!transposto_valido && in_direcionado)
    {
        if (transposto != NULL)
            delete transposto;
        transposto = new Grafo(this->in_direcionado, this->in_ponderado_vertice, this->in_ponderado_aresta);
        for (No *no : lista_adj)
        {
            No *novoNo = new No(no->id);
            if (in_direcionado)
                novoNo->peso = no->peso;
            transposto->lista_adj.push_back(novoNo);
        }
        for (No *no : lista_adj)
        {
            for (Aresta *aresta : no->arestas)
            {
                transposto->inserirAresta(aresta->id_no_alvo, no->id, aresta->peso);
            }
        }
    }
}

void Grafo::auxiliarFechos(char id_no, vector<char> &lista_fecho)
{
    No *atual = encontrarNo(id_no);
    if (atual != NULL)
    {

        if (std::find(lista_fecho.begin(), lista_fecho.end(), id_no) == lista_fecho.end())
        {
            lista_fecho.push_back(id_no);
            cout << (atual->arestas.size() != 0);
            for (Aresta *aresta : atual->arestas)
            {

                auxiliarFechos(aresta->id_no_alvo, lista_fecho);
            }
        }
    }
}

// Fim das que criei, a partir daqui são as funções que já exisitiam. Implementei as de fecho transitivo direto e indireto:
vector<char> Grafo::fecho_transitivo_direto(char id_no)
{

    if (!in_direcionado)
    {
        cout << "Grafo não direcionado, não há conceito de fecho" << endl;
        return {};
    }
    vector<char> retorno;

    auxiliarFechos(id_no, retorno);
    if (retorno.size() == 0)
    {
        cout << "Nó não existe!!!!" << endl;
        return {};
    }
    return retorno;
}

vector<char> Grafo::fecho_transitivo_indireto(char id_no)
{
    if (!in_direcionado)
    {
        cout << "Grafo não direcionado, não há conceito de fecho" << endl;
        return {};
    }

    vector<char> retorno;
    gerarTransposto();
    retorno = transposto->fecho_transitivo_direto(id_no);

    return retorno;
}

// A partir daqui, ainda devemos implementar:
vector<char> Grafo::caminho_minimo_dijkstra(char id_no_a, char id_no_b) {
    if (encontrarNo(id_no_a) == NULL || encontrarNo(id_no_b) == NULL) {
        cout << "Erro: Um dos nos (" << id_no_a << " ou " << id_no_b << ") nao existe no grafo." << endl;
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

vector<char> Grafo::caminho_minimo_floyd(char id_no, char id_no_b)
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

Grafo *Grafo::subGrafoVerticeInduzido(vector<char> ids_nos)
{
    int ordemSubGrafo = ids_nos.size();
    vector<char> utilizados(ordemSubGrafo);
    Grafo *subGrafo = new Grafo(this->in_direcionado, this->in_ponderado_vertice, this->in_ponderado_aresta);
    for (No *no : lista_adj)
    {
        auto iterador = std::find(ids_nos.begin(), ids_nos.end(), no->id);
        if (iterador != ids_nos.end())
        {
            No *novoNo = new No(no->id);
            novoNo->peso = no->peso;
            if (iterador != ids_nos.end() - 1)
                std::iter_swap(iterador, ids_nos.end() - 1);
            ids_nos.pop_back();
            utilizados.push_back(no->id);
            for (Aresta *aresta : no->arestas)
            {
                iterador = std::find(ids_nos.begin(), ids_nos.end(), aresta->id_no_alvo);
                if (iterador != ids_nos.end())
                {
                    Aresta *novaAresta = new Aresta(aresta->id_no_alvo);
                    novaAresta->peso = aresta->peso;
                    novoNo->arestas.push_back(novaAresta);
                }
                else if (!this->in_direcionado && std::find(utilizados.begin(), utilizados.end(), aresta->id_no_alvo) != utilizados.end())
                {
                    Aresta *novaAresta = new Aresta(aresta->id_no_alvo);
                    novaAresta->peso = aresta->peso;
                    novoNo->arestas.push_back(novaAresta);
                }
            }

            subGrafo->lista_adj.push_back(novoNo);
        }
    }
    subGrafo->ordem = ordemSubGrafo;
    return subGrafo;
}

std::vector<ArestaCompleta> Grafo::listaArestasOrdenadas(Grafo *grafo)
{
    std::vector<ArestaCompleta> arestasOrdenadas;

    for (No *no : grafo->lista_adj)
    {
        char idOrigem = no->id;

        for (Aresta *aresta : no->arestas)
        {
            char idDestino = aresta->id_no_alvo;

            // Se for grafo direcionado, inclui direto;
            // Se for não-direcionado, inclui apenas uma vez (origem <= destino evita duplicatas)
            if (grafo->in_direcionado || idOrigem <= idDestino)
            {
                ArestaCompleta nova;
                nova.origem = idOrigem;
                nova.destino = idDestino;
                nova.peso = aresta->peso;
                arestasOrdenadas.push_back(nova);
            }
        }
    }

    std::sort(arestasOrdenadas.begin(), arestasOrdenadas.end(), [](ArestaCompleta a, ArestaCompleta b)
              {
                  return a.peso > b.peso; // ordena em ordem decrescente
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

Grafo *Grafo::arvore_geradora_minima_kruskal(vector<char> ids_nos)
{

    Grafo *subGrafoVertInd = this->subGrafoVerticeInduzido(ids_nos);
    Grafo *arvoreGeradoraMin = new Grafo(this->in_direcionado, this->in_ponderado_vertice, this->in_ponderado_aresta);
    vector<ArestaCompleta> listaArestasOrdenad = listaArestasOrdenadas(subGrafoVertInd);
    vector<vector<char>> nosJuntos;

    for (No *no : subGrafoVertInd->lista_adj)
    {
        No *novoNo = new No(no->id);
        novoNo->peso = no->peso;
        arvoreGeradoraMin->lista_adj.push_back(novoNo);
    }
    arvoreGeradoraMin->ordem = subGrafoVertInd->ordem;
    int contador = 0;
    while (contador < subGrafoVertInd->ordem - 1 && !listaArestasOrdenad.empty())
    {

        ArestaCompleta ar = listaArestasOrdenad.back();
        listaArestasOrdenad.pop_back();
        bool achou1 = false;
        bool achou2NaMesma = false;
        bool terminar = false;
        int pos1 = -1;
        int pos2 = -1;
        int contadorLocalNos = 0;
        char naoAchado = '\0';
        for (vector<char> nos : nosJuntos)
        {
            for (char id : nos)
            {
                if (id == ar.origem || id == ar.destino)
                {
                    if (achou1)
                    {
                        achou2NaMesma = true;
                        terminar = true;
                        break;
                    }
                    achou1 = true;
                    if (pos1 == -1)
                    {
                        pos1 = contadorLocalNos;
                        if (ar.origem == id)
                            naoAchado = ar.destino;
                        else
                            naoAchado = ar.origem;
                    }
                    else
                    {
                        pos2 = contadorLocalNos;
                        terminar = true;
                        break;
                    }
                }
            }
            contadorLocalNos++;

            if (terminar)
                break;
            achou1 = false;
        }
        if (!achou2NaMesma)
        {
            arvoreGeradoraMin->inserirAresta(ar.origem, ar.destino, ar.peso);
            contador++;
            if (pos1 == -1)
            {
                vector<char> novoConj;
                novoConj.push_back(ar.origem);
                novoConj.push_back(ar.destino);
                nosJuntos.push_back(novoConj);
            }
            else
            {
                if (pos2 == -1)
                    (nosJuntos[pos1]).push_back(naoAchado);
                else
                {
                    vector<char> *juncaoExcluidaParaFusao = &nosJuntos[pos2];
                    vector<char> *fusaoNosJuntos = &nosJuntos[pos1];
                    for (char no : *juncaoExcluidaParaFusao)
                        fusaoNosJuntos->push_back(no);
                    nosJuntos.erase(nosJuntos.begin() + pos2);
                }
            }
        }
    }

    if (nosJuntos.size() != 1)
    {
        cout << endl
             << "Não foi gerada árvore geradora mínima, pois o grafo não era conexo. Assim é retornado um grafo com árvores geradoras" << endl;
    }
    delete subGrafoVertInd;
    return arvoreGeradoraMin;
}

void Grafo::buscaProfundidadeNo(Grafo *prof, No *no, bool *visitado, char id_pai)
{
    No *novoNo = new No(no->id);
    novoNo->peso = no->peso;
    No *novoNoRet = new No(no->id);
    novoNoRet->peso = no->peso;
    prof->lista_adj.push_back(novoNo);
    prof->ordem++;
    prof->lista_adj_retorno.push_back(novoNoRet);
    int pos_lista = 0;
    for (Aresta *aresta : no->arestas)
    {
        pos_lista = posicaoNoNaLista(aresta->id_no_alvo);
        Aresta *ar = new Aresta(aresta->id_no_alvo);
        ar->peso = aresta->peso;
        if (!visitado[pos_lista])
        {
            visitado[pos_lista] = true;
            novoNo->arestas.push_back(ar);
            buscaProfundidadeNo(prof, lista_adj[pos_lista], visitado, no->id);
        }
        else
        {
            if (!this->in_direcionado && id_pai == aresta->id_no_alvo)
            {
                novoNo->arestas.push_back(ar);
            }
            else
            {
                novoNoRet->arestas.push_back(ar);
            }
        }
    }
}

Grafo *Grafo::arvore_caminhamento_profundidade(char id_no)
{
    int pos_lis = this->posicaoNoNaLista(id_no);
    Grafo *profundidade = new Grafo(this->in_direcionado, this->in_ponderado_vertice, this->in_ponderado_aresta);
    if (pos_lis == -1)
    {
        cout << "Id inválido passado!!!" << endl;
        return nullptr;
    }

    bool *visitado = new bool[ordem];
    for (int i = 0; i < ordem; i++)
        visitado[i] = false;
    visitado[pos_lis] = true;
    No *no = lista_adj[pos_lis];
    buscaProfundidadeNo(profundidade, no, visitado, '\0');
    delete[] visitado;
    return profundidade;
}

int Grafo::raio()
{
    cout << "Metodo nao implementado" << endl;
    return 0;
}

int Grafo::diametro()
{
    cout << "Metodo nao implementado" << endl;
    return 0;
}

vector<char> Grafo::centro()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::periferia()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}

vector<char> Grafo::vertices_de_articulacao()
{
    cout << "Metodo nao implementado" << endl;
    return {};
}
