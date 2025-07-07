#include "Gerenciador.h"
#include <fstream>
#include <limits>
#include <string>

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

void Gerenciador::comandos(Grafo* grafo) {

    // trocar a recursão por um loop `while(true)` para evitar o aviso de recursão infinita
    while(true) {
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

        //ler a entrada como string para validar o tamanho
        string entrada_usuario;
        cin >> entrada_usuario;

        if (entrada_usuario.length() == 1) {
            char resp = entrada_usuario[0];
            switch (resp) {
                case 'a': {

                    char id_no = get_id_entrada();
                    vector<char> fecho_transitivo_direto = grafo->fecho_transitivo_direto(id_no);
                
                    cout<<endl<<endl<<"Lista em formato de conjunto: "<<endl;
                    imprimirConjuntoChar(fecho_transitivo_direto);
                    cout<<endl<<endl;
                    imprimirGrafoPorIndices(grafo,fecho_transitivo_direto);
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
                    cout<<endl<<endl;
                    imprimirGrafoPorIndices(grafo,fecho_transitivo_indireto);
                    if(pergunta_imprimir_arquivo("fecho_trans_indir.txt")) {
                        imprimeArquivo_a_ate_d(fecho_transitivo_indireto,"fecho_trans_indir.txt");
                    }

                    ;
                    break;
                }

                case 'c': {
                    char id_no_1 = get_id_entrada();
                    char id_no_2 = get_id_entrada();

                    vector<char> caminho_minimo = grafo->caminho_minimo_dijkstra(id_no_1, id_no_2);
                    if (!caminho_minimo.empty()) {
                        for (size_t i = 0; i < caminho_minimo.size(); ++i) {
                            cout << caminho_minimo[i];
                            if (i < caminho_minimo.size() - 1) {
                                cout << ",";
                            }
                        }
                        cout << endl << endl;
                    }

                    if (pergunta_imprimir_arquivo("caminho_minimo_dijkstra.txt")) {
                        imprimeArquivo_a_ate_d(caminho_minimo,"caminho_minimo_dijkstra.txt");
                    }

                    break;
                }

                case 'd': {

                    char id_no_1 = get_id_entrada();
                    char id_no_2 = get_id_entrada();
                    vector<char> caminho_minimo_floyd = grafo->caminho_minimo_floyd(id_no_1,id_no_2);
                    cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

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
                        cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

                        if(pergunta_imprimir_arquivo("agm_prim.txt")) {
                           imprimeArquivo_e_ate_g(arvore_geradora_minima_prim,"agm_prim.txt");
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
                            imprimeArquivo_e_ate_g(arvore_geradora_minima_kruskal,"agm_kruskal.txt");
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
                        imprimeArquivo_e_ate_g(arvore_caminhamento_profundidade,"arvore_caminhamento_profundidade.txt");
                    }

                    delete arvore_caminhamento_profundidade;
                    break;
                }

                case 'h': {
                    vector<char> articulacao = grafo->vertices_de_articulacao();
                    cout<<"Metodo de impressao em tela nao implementado"<<endl<<endl;

                    if(pergunta_imprimir_arquivo("raio_diam_centro_perif.txt")) {
                        imprimeArquivo_h(raio, diametro, centro, periferia,"raio_diam_centro_perif.txt");                    }

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
                    cout << "Saindo do programa." << endl;
                    return; // Retorna da função, encerrando o loop while.
                }
                default: {
                    cout<<"Opção inválida. Por favor, tente novamente."<<endl << endl;
                }
            }
        }
        else {
            cout << "Opção inválida. Por favor, digite apenas um caractere." << endl << endl;
        }
        //limpa o buffer de entrada para a próxima iteração do loop
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
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