#include <iostream>
#include <fstream>
#include <chrono>
#include "./Grafo.h"

using namespace std;
using namespace std::chrono;

No *encontrarNo(const vector<No *> listaAdj, char id)
{
    for (No *no : listaAdj)
    {
        if (no->id == id)
            return no;
    }
    return NULL;
}

void lerArquivoGrafo(ifstream &arquivo, Grafo *grafo)
{
    int bitDir, bitPondVert, bitPondAresta;

    arquivo >> bitDir >> bitPondAresta >> bitPondVert;

    grafo->in_direcionado = (bitDir != 0);
    grafo->in_ponderado_aresta = (bitPondAresta != 0);
    grafo->in_ponderado_vertice = (bitPondVert != 0);
    int tam = 0;
    arquivo >> tam;
    char id = '\0';
    int peso = 1;
    for (int i = 0; i < tam; i++)
    {

        arquivo >> id;
        cout << id << endl;
        if (grafo->in_ponderado_vertice)
            arquivo >> peso;
        if (!grafo->inserirNo(id, peso))
        {
            cout << "Falaha ao inserir Nó, já estava no grafo!" << endl;
        }
    }
    char origem, destino;
    while (arquivo >> origem >> destino)
    {
        cout << origem << " " << destino << endl;
        if (grafo->in_ponderado_aresta)
            arquivo >> peso;
        if (!grafo->inserirAresta(origem, destino, peso))
        {
            cout << "Um dos nós da aresta( " << origem << " ou " << destino << " ) não está no grafo" << endl;
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
    cout << "saiu" << endl;
}

int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        cerr << "Erro: nome do arquivo não fornecido.\n";
        cerr << "Uso correto: " << argv[0] << " <nome_do_arquivo>\n";
        return 1;
    }
    string nomeArquivo = argv[1];
    string caminhoCompleto = "../instancias/" + nomeArquivo;
    ifstream arquivo(caminhoCompleto);
    if (!arquivo.is_open())
    {
        cerr << "Erro: não foi possível abrir o arquivo '" << argv[1] << "'\n";
        return 1;
    }

    cout << argv[1] << endl;

    cout << "Arquivo '" << caminhoCompleto << "' aberto com sucesso!\n";
    Grafo *grafo = new Grafo();
    lerArquivoGrafo(arquivo, grafo);

    cout << "Escolha o algoritmo: " << endl;
    bool continuar = true;
    vector<char> solucao;

    vector<double> alphas = {0.05, 0.1, 0.15, 0.3, 0.5};
    vector<vector<char>> solucoes_adapt_rand(alphas.size());
    int reps_adapRand = 500;

    double exp_sens = 3;
    int reps_reat = 1000;
    int reps_troca = 30;

    while (continuar)
    {

        cout << "(a) Guloso \n(b) Guloso Randomizado adaptativo \n(c) Guloso Randomizado Adaptativo Reativo\n (qualquer outro caractere) sair" << endl;
        char opcao = 0;
        cin >> opcao;

        high_resolution_clock::time_point inicio, fim;
        nanoseconds tempo_execucao;

        switch (opcao)
        {
        case 'a':
        {
            inicio = high_resolution_clock::now();
            solucao = grafo->dominanteMin2_guloso();
            fim = high_resolution_clock::now();
            tempo_execucao = duration_cast<nanoseconds>(fim - inicio);
            cout << "Tempo de execução (Guloso): " << tempo_execucao.count() << " ns" << endl;
            break;
        }
        case 'b':
        {
            inicio = high_resolution_clock::now();
            for (int i = 0; i < alphas.size(); i++)
            {
                solucoes_adapt_rand[i] = grafo->dominanteMin2_gulosoAdaptRand(alphas[i], reps_adapRand);
            }
            fim = high_resolution_clock::now();
            tempo_execucao = duration_cast<nanoseconds>(fim - inicio);
            cout << "Tempo de execução (Guloso Randomizado Adaptativo): " << tempo_execucao.count() << " ns" << endl;
            break;
        }
        case 'c':
        {
            inicio = high_resolution_clock::now();
            solucao = grafo->dominanteMin2_gulosoAdaptRandReat(alphas, reps_reat, reps_troca, exp_sens);
            fim = high_resolution_clock::now();
            tempo_execucao = duration_cast<nanoseconds>(fim - inicio);
            cout << "Tempo de execução (Guloso Randomizado Adaptativo Reativo): " << tempo_execucao.count() << " ns" << endl;
            break;
        }
        default:
        {
            continuar = false;
            break;
        }
        }

        if (continuar)
        {
            if (opcao == 'b')
            {
                int cont = 0;
                for (vector<char> sol : solucoes_adapt_rand)
                {
                    cout << "alpha: " << alphas[cont] << endl;
                    cout << "Tamanho da solução: " << sol.size() << endl;
                    cout << "elementos da solução:{";
                    for (int i = 0; i < sol.size(); i++)
                    {
                        cout << sol[i];
                        if (i != sol.size() - 1)
                            cout << ", ";
                    }
                    cout << "}" << endl;
                    cont++;
                }
            }
            else
            {
                cout << "Tamanho da solução: " << solucao.size() << endl;
                cout << "elementos da solução:{";
                for (int i = 0; i < solucao.size(); i++)
                {
                    cout << solucao[i];
                    if (i != solucao.size() - 1)
                        cout << ", ";
                }
                cout << "}" << endl;
            }
            cout << endl
                 << endl
                 << endl;
        }
    }

    vector<char> solucaoAlgAtual = grafo->dominanteMin2_guloso();

    arquivo.close();

    delete grafo;
    return 0;
}
