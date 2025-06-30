#include "Gerenciador.h"
#include <fstream>

using namespace std;

// Função que criei:
void Gerenciador::imprimirConjuntoChar(vector<char> conjunto){
    cout<<"{ ";
            bool primeiro=true;
            for(char noFecho : conjunto){
                if(primeiro)
                    primeiro=false;
                else
                    cout<<", "  ;  
            
                cout<<noFecho<<" ";
            }    
            cout<<"}";
}

void Gerenciador::imprimirGrafoPorIndices(Grafo* grafo,vector<char>& conjunto){
    cout<<endl;
    cout<<grafo->in_direcionado<<" "<<grafo->in_ponderado_aresta<<" "<<grafo->in_ponderado_vertice<<endl;
    vector<vector<Aresta*>> listasArestasInclusas;
    vector<char> inclusos;
    for(char indice_no : conjunto){  
        vector<Aresta*> arestas_inclusas;
        cout<<indice_no;
        
        No* no=grafo->encontrarNo(indice_no);
        if(grafo->in_ponderado_vertice)
            cout<<" "<<no->peso;
        for(Aresta* aresta : no->arestas){
            if(find(conjunto.begin(), conjunto.end(),aresta->id_no_alvo) != conjunto.end() && 
            (grafo->in_direcionado || find(inclusos.begin(), inclusos.end(),aresta->id_no_alvo)== inclusos.end()  ) )
                arestas_inclusas.push_back(aresta);
            

        }
        listasArestasInclusas.push_back(arestas_inclusas); 
        inclusos.push_back(indice_no);
        cout<<endl;   
    } 
    int i=0;
    for(char indice_no : conjunto){
        
        for(Aresta* aresta : listasArestasInclusas[i]){
            cout<<indice_no<<" "<<aresta->id_no_alvo;
            if(grafo->in_ponderado_aresta)
                cout<<" "<<aresta->peso;
            cout<<endl;    
        }
        i++;
    } 
    cout<<endl;

}  

void Gerenciador::imprimirGrafo(Grafo* grafo){
    cout<<endl;
    cout<<grafo->in_direcionado<<" "<<grafo->in_ponderado_aresta<<" "<<grafo->in_ponderado_vertice<<endl;
    vector<char> inclusos;
    for(No* no: grafo->lista_adj)
    {
        cout<<no->id;
        if(grafo->in_ponderado_vertice)
            cout<<" "<<no->peso;

        cout<<endl;    
    }  
    for(No* no: grafo->lista_adj)
    {
        for(Aresta* aresta: no->arestas){
            if(grafo->in_direcionado || find(inclusos.begin(), inclusos.end(),aresta->id_no_alvo)== inclusos.end()){
                cout<<no->id<<" "<<aresta->id_no_alvo;
                if(grafo->in_ponderado_aresta)
                    cout<<" "<<aresta->peso;
                cout<<endl;    
            }    
        }
        if(!grafo->in_direcionado)
            inclusos.push_back(no->id);
    }
    inclusos.clear();
    cout<<endl<<"Arestas de retorno:"<<endl;
    for(No* no: grafo->lista_adj_retorno)
    {
        for(Aresta* aresta: no->arestas){
            if(grafo->in_direcionado || find(inclusos.begin(), inclusos.end(),aresta->id_no_alvo)== inclusos.end()){
                cout<<no->id<<" "<<aresta->id_no_alvo;
                if(grafo->in_ponderado_aresta)
                    cout<<" "<<aresta->peso;
                cout<<endl;    
            }    
        }
        if(!grafo->in_direcionado)
            inclusos.push_back(no->id);
    }

    cout<<endl;
}

void Gerenciador::comandos(Grafo* grafo) {
    cout<<"Digite uma das opcoes abaixo e pressione enter:"<<endl<<endl;
    cout<<"(a) Fecho transitivo direto de um no;"<<endl;
    cout<<"(b) Fecho transitivo indireto de um no;"<<endl;
    cout<<"(c) Caminho minimo (Djikstra);"<<endl;
    cout<<"(d) Caminho minimo (Floyd);"<<endl;
    cout<<"(e) Arvore Geradora Minima (Algoritmo de Prim);"<<endl;
    cout<<"(f) Arvore Geradora Minima (Algoritmo de Kruskal);"<<endl;
    cout<<"(g) Arvore de caminhamento em profundidade;"<<endl;
    cout<<"(h) Raio, diametro, centro e periferia do grafo;"<<endl;
    cout<<"(0) Sair;"<<endl<<endl;

    char resp;
    cin >> resp;
    switch (resp) {
        case 'a': {

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);
           
            cout<<endl<<endl<<"Lista em formato de conjunto: "<<endl;
            imprimirConjuntoChar(fecho_transitivo_direto);
            cout<<endl<<endl;
            imprimirGrafoPorIndices(grafo,fecho_transitivo_direto);
            if(pergunta_imprimir_arquivo("fecho_trans_dir.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl<<endl;
            }


            break;
        }

        case 'b':{

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);
            cout<<endl<<endl<<"Lista em formato de conjunto: "<<endl;
            imprimirConjuntoChar(fecho_transitivo_indireto);
            cout<<endl<<endl;
            imprimirGrafoPorIndices(grafo,fecho_transitivo_indireto);
            if(pergunta_imprimir_arquivo("fecho_trans_indir.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            }

            ;
            break;
        }

        case 'c': {

            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_dijkstra = grafo->caminho_minimo_dijkstra(id_no_1,id_no_2);
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            }


            break;
        }

        case 'd': {

            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1,id_no_2);
            imprimirConjuntoChar( caminho_minimo_floyd);
            imprimirGrafoPorIndices(grafo,caminho_minimo_floyd);

            if(pergunta_imprimir_arquivo("caminho_minimo_floyd.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            }


            break;
        }
        case 'e': {

            int tam;
            cout<<"Digite o tamanho do subconjunto: ";
            cin>>tam;

            if(tam > 0 && tam <= grafo->ordem) {

                vector<char> ids = get_conjunto_ids(grafo,tam);
                Grafo* arvore_geradora_minima_prim = grafo->arvore_geradora_minima_prim(ids);
                cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

                if(pergunta_imprimir_arquivo("agm_prim.txt")) {
                    cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
                }

                delete arvore_geradora_minima_prim;

            }else {
                cout<<"Valor invalido"<<endl;
            }

            break;
        }

        case 'f': {

            int tam;
            cout<<"Digite o tamanho do subconjunto: ";
            cin>>tam;

            if(tam > 0 && tam <= grafo->ordem) {

                vector<char> ids = get_conjunto_ids(grafo,tam);
                Grafo* arvore_geradora_minima_kruskal = grafo->arvore_geradora_minima_kruskal(ids);
                imprimirGrafo(arvore_geradora_minima_kruskal);

                if(pergunta_imprimir_arquivo("agm_kruskal.txt")) {
                    cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
                }

                delete arvore_geradora_minima_kruskal;

            }else {
                cout<<"Valor invalido"<<endl;
            }

            break;
        }

        case 'g': {

            char id_no = get_id_entrada();
            Grafo* arvore_caminhamento_profundidade = grafo->arvore_caminhamento_profundidade(id_no);
            cout<<endl<<endl;
            imprimirGrafo(arvore_caminhamento_profundidade);
            cout<<endl;
            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            }

            delete arvore_caminhamento_profundidade;
            break;
        }

        case 'h': {
            vector<char> articulacao = grafo->vertices_de_articulacao();
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            }

            break;
        }
        case 'i': {

            vector<char> articulacao = grafo->vertices_de_articulacao();
            cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

            if(pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                cout<<"Metodo de impressao em arquivo nao implementado"<<endl;
            }

            break;
        }

        case '0': {
            exit(0);
        }
        default: {
            cout<<"Opção inválida"<<endl;
        }
    }

    comandos(grafo);

}

char Gerenciador::get_id_entrada() {
    cout<<"Digite o id de um no: ";
    char id;
    cin>>id;
    cout<<endl;
    return id;
}

vector<char> Gerenciador::get_conjunto_ids(Grafo *grafo, int tam) {
    vector<char> ids = {};
    while((int)ids.size() < tam) {
        char id_no =get_id_entrada();
        bool existe = false;
        for(No* no: grafo->lista_adj){
            if(no->id == id_no){
                existe = true;
                break;
            }
        }

        if(!existe){
            cout<<"Vertice nao existe"<<endl;
        }else{
            bool repetido = find(ids.begin(), ids.end(),id_no) != ids.end();
            if(repetido) {
                    cout<<"Valor repetido"<<endl;
            }else {
                ids.push_back(id_no);
            }
        }

    }

    return ids;
}


bool Gerenciador::pergunta_imprimir_arquivo(string nome_arquivo) {

    cout<<"Imprimir em arquivo externo? ("<<nome_arquivo<<")"<<endl;
    cout<<"(1) Sim;"<<endl;
    cout<<"(2) Nao."<<endl;
    int resp;
    cin>>resp;
    cout<<endl;

    switch (resp) {
        case 1:
            return true;
        case 2:
            return false;
        default:
            cout<<"Resposta invalida"<<endl;
            return pergunta_imprimir_arquivo(nome_arquivo);
    }
}