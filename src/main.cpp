#include <iostream>
#include <fstream>
#include "Gerenciador.h"

using namespace std;

No* encontrarNo(const vector<No*> listaAdj,char id){
    for (No* no : listaAdj) {
        if (no->id == id)
            return no;
    }
    return NULL;
}

void lerArquivoGrafo(ifstream& arquivo,Grafo* grafo){
    int bitDir,bitPondVert,bitPondAresta;

    arquivo>>bitDir>>bitPondVert>>bitPondAresta;
    
    
    
    grafo->in_direcionado=(bitDir!=0);
    grafo->in_ponderado_aresta=(bitPondVert!=0);
    grafo->in_ponderado_vertice=(bitPondVert!=0);
    int tam=0;
    arquivo>>tam;
    char id='\0';
    int peso=1;
    for (int i = 0; i < tam; i++) {
        
        arquivo>>id;
        cout<<id<<endl;
        if(grafo->in_ponderado_vertice)
            arquivo>>peso;
        if(!grafo->inserirNo(id,peso)){
            cout<<"Falaha ao inserir Nó, já estava no grafo!"<<endl;
        }
    }
    char origem,destino;
    while(arquivo>>origem>>destino){
        cout<<origem<<" "<<destino<<endl;
        if(grafo->in_ponderado_aresta)
            arquivo>>peso;
        if(!grafo->inserirAresta(origem,destino,peso)){
            cout<<"Um dos nós da aresta( "<<origem<<" ou "<<destino<<" ) não está no grafo"<<endl;
        }    
            /*
        Aresta* aresta = new Aresta();
        aresta->id_no_alvo = destino;
        if(bitPondAresta)
            arquivo>>aresta->peso;
        else
            aresta->peso = 1; 

        noOrigem->arestas.push_back(aresta);
        if(!grafo->in_direcionado){
            Aresta* aresta2 = new Aresta();
            aresta2->id_no_alvo = origem;
            if(bitPondAresta)
                arquivo>>aresta2->peso;
            else
                aresta2->peso = 1; 

        }
                */
    }
    cout<<"saiu"<<endl;
    

    
}

int main(int argc, char *argv[])
{
    
    if (argc < 2) {
        cerr << "Erro: nome do arquivo não fornecido.\n";
        cerr << "Uso correto: " << argv[0] << " <nome_do_arquivo>\n";
        return 1;
    }
    string nomeArquivo = argv[1];
    string caminhoCompleto = "../instancias/" + nomeArquivo;
    ifstream arquivo(caminhoCompleto);
    if (!arquivo.is_open()) {
        cerr << "Erro: não foi possível abrir o arquivo '" << argv[1] << "'\n";
        return 1;
    }
    
    
    
    cout<<argv[1]<<endl;
    cout << "Arquivo '" << caminhoCompleto << "' aberto com sucesso!\n";
    Grafo* grafo = new Grafo();
    lerArquivoGrafo(arquivo,grafo);
    arquivo.close();
    Gerenciador::comandos(grafo);
    
    delete grafo;
    return 0;
}
