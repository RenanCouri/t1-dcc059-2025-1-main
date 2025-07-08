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

    for (No* no : grafo->lista_adj) {
        cout << no->id << ": ";
        for (int i = 0; i < no->arestas.size(); i++) {
            cout << no->arestas[i]->id_no_alvo;
            if (i < no->arestas.size() - 1)
                cout << "->";
        }
        cout << endl;
    }

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
    cout<<"(i) Vértices de articulação do grafo;"<<endl;
    cout<<"(0) Sair;"<<endl<<endl;

    char resp;
    cin >> resp;
    switch (resp) {
        case 'a': {

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);
           
            cout<<endl<<endl<<"Lista em formato de conjunto: "<<endl;
            imprimirConjuntoChar(fecho_transitivo_direto);

            if(pergunta_imprimir_arquivo("fecho_trans_dir.txt")) {
                imprimeArquivo_a_ate_d(fecho_transitivo_direto,"fecho_trans_dir.txt");
            }


            break;
        }

        case 'b':{

            char id_no = get_id_entrada();
            vector<char> fecho_transitivo_indireto = grafo->fecho_transitivo_indireto(id_no);
            cout<<endl<<endl<<"Lista em formato de conjunto: "<<endl;
            imprimirConjuntoChar(fecho_transitivo_indireto);
        
            if(pergunta_imprimir_arquivo("fecho_trans_indir.txt")) {
                imprimeArquivo_a_ate_d(fecho_transitivo_indireto,"fecho_trans_indir.txt");
            }

            ;
            break;
        }

        case 'c': {

            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_dijkstra = grafo->caminho_minimo_dijkstra(id_no_1,id_no_2);
            imprimirConjuntoChar(caminho_minimo_dijkstra);

            if(pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt")) {
                imprimeArquivo_a_ate_d(caminho_minimo_dijkstra,"caminho_minimo_dijkstra.txt");
            }


            break;
        }

        case 'd': {

            char id_no_1 = get_id_entrada();
            char id_no_2 = get_id_entrada();
            vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1,id_no_2);
            imprimirConjuntoChar( caminho_minimo_floyd);

            if(pergunta_imprimir_arquivo("caminho_minimo_floyd.txt")) {
                imprimeArquivo_a_ate_d(caminho_minimo_floyd,"caminho_minimo_floyd.txt");
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
                if( arvore_geradora_minima_prim!=NULL) 
                    imprimirGrafo(arvore_geradora_minima_prim);
                else 
                    cout<<"Impossível imprimir grafo não formado!"<<endl;
               
                if( arvore_geradora_minima_prim!=NULL && pergunta_imprimir_arquivo("agm_prim.txt")) {
                   imprimeArquivo_e_ate_g(arvore_geradora_minima_prim,"agm_prim.txt");
                }
                if(arvore_geradora_minima_prim!=NULL)
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
                if( arvore_geradora_minima_kruskal!=NULL) 
                    imprimirGrafo(arvore_geradora_minima_kruskal);
                else 
                    cout<<"Impossível imprimir grafo não formado!"<<endl;
               
                if( arvore_geradora_minima_kruskal!=NULL && pergunta_imprimir_arquivo("agm_kruskal.txt")) {
                   imprimeArquivo_e_ate_g(arvore_geradora_minima_kruskal,"agm_kruskal.txt");
                }
                if(arvore_geradora_minima_kruskal!=NULL)
                    delete arvore_geradora_minima_kruskal;

            }else {
                cout<<"Valor invalido"<<endl;
            }

            break;
        }

        case 'g': {

            char id_no = get_id_entrada();
            Grafo* arvore_caminhamento_profundidade = grafo->arvore_caminhamento_profundidade(id_no);
            if( arvore_caminhamento_profundidade!=NULL) 
                    imprimirGrafo(arvore_caminhamento_profundidade);
                else 
                    cout<<"Impossível imprimir grafo não formado!"<<endl;
               
                if( arvore_caminhamento_profundidade!=NULL && pergunta_imprimir_arquivo("arvore_caminhamento_profundidade.txt")) {
                   imprimeArquivo_e_ate_g(arvore_caminhamento_profundidade,"arvore_caminhamento_profundidade.txt");
                }
                if(arvore_caminhamento_profundidade!=NULL)
                    delete arvore_caminhamento_profundidade;
            break;
        }

        case 'h': {
            int raio = grafo->raio();
            int diametro= grafo->diametro();
            vector<char> centro = grafo->centro();
            vector<char> periferia = grafo->periferia();
            cout<<"Raio: "<<raio<<endl;
            cout<<"Diâmetro: "<<diametro<<endl;
            cout<<"Centro: "<<endl;
            imprimirConjuntoChar(centro);
            cout<<endl<<"Periferia: "<<endl;
            imprimirConjuntoChar(periferia);

            if(pergunta_imprimir_arquivo("distancias.txt")) {
                imprimeArquivo_h(raio,diametro,centro,periferia,"distancias.txt");
            }

            break;
        }
        case 'i': {

            vector<char> articulacao = grafo->vertices_de_articulacao();
            imprimirConjuntoChar(articulacao);

            if(pergunta_imprimir_arquivo("vertices_articulacao.txt")) {
                imprimeArquivo_a_ate_d(articulacao,"vertices_articulacao.txt");
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


void Gerenciador::imprimeArquivo_a_ate_d(vector<char> resultado, string nome){
    ofstream arq1(nome); //ofstream: apenas para saida de dados. abre o arquivo. cria o arquivo caso ele não exista.
    size_t i = 1;
    for (char id :resultado){
        if(i == resultado.size()){
            arq1 << id;
        }
        else{
        arq1 << id << ",";
        }
        i++;
    }

    arq1.close();
}

void Gerenciador::imprimeArquivo_e_ate_g(Grafo* grafo, string nome){
    ofstream arq2(nome);
    for(No *no : grafo->lista_adj){
        arq2 << no->id << ": ";
        size_t j = 1;
        for(Aresta *aresta : no->arestas){
            if(j == no->arestas.size()){
                arq2 << aresta->id_no_alvo;
            }
            else{
                arq2 << aresta->id_no_alvo << " -> ";
            }
            j++;
        }
        arq2<<endl;
    }
    arq2.close();
}

void Gerenciador::imprimeArquivo_h(int raio, int diametro, vector<char> centro, vector<char> periferia, string nome){
    ofstream arq1(nome); //ofstream: apenas para saida de dados. abre o arquivo. cria o arquivo caso ele não exista.
    arq1 << raio << endl;
    arq1 << diametro << endl;
    size_t i = 1;
    for (char id :centro){
        if(i == centro.size()){
            arq1 << id;
        }
        else{
        arq1 << id << ",";
        }
        i++;
    }
    i = 1;
    arq1 << endl;
        for (char id :periferia){
        if(i == periferia.size()){
            arq1 << id;
        }
        else{
        arq1 << id << ",";
        }
        i++;
    }

    arq1.close();
}