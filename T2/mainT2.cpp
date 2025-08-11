#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <climits>
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
        // cout << id << endl;
        if (grafo->in_ponderado_vertice)
            arquivo >> peso;
        if (!grafo->inserirNo(id, peso))
        {
            cout << "Falha ao inserir Nó, já estava no grafo!" << endl;
        }
    }
    char origem, destino;
    while (arquivo >> origem >> destino)
    {
        // cout << origem << " " << destino << endl;
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
    // cout << "saiu" << endl;
}

int main()
{
    const string listaPath = "../instancias/instances.txt"; // acessa as instâncias fornecidas
    ifstream lista(listaPath);
    if (!lista.is_open())
    {
        cerr << "Erro: não foi possível abrir o arquivo de lista '" << listaPath << "'\n";
        return 1;
    }

    // Parâmetros dos experimentos
    const int runs = 10;                             // 10 execuções por instância/algoritmo
    const vector<double> alphas = {0.1, 0.25, 0.45}; // 3 valores de alfa
    const int reps_adapRand = 30;                    // >=30 por execução do randomizado adaptativo
    const int reps_reat = 300;                       // >=300 por execução do reativo
    const int reps_troca = 30;                       // bloco de 30

    string nomeArquivo;
    while (getline(lista, nomeArquivo))
    { // roda para cada instância de instances.txt
        if (nomeArquivo.empty())
            continue;

        const string caminhoCompleto = "../instancias/" + nomeArquivo;
        cout << "\n========================================\n";
        cout << "Instância: " << nomeArquivo << "\n"; // nome da instância

        // ---------- Algoritmo (a) Guloso ----------
        {
            cout << "\n[A] Guloso - " << runs << " execuções\n";
            long long best_size = LLONG_MAX; // guarda o menor tamanho encontrado
            // iniciado com LLONG_MAX para que a primeira solução entre.
            double sum_sizes = 0.0;    // acumula tamanhos para depois tirar a média
            long long sum_time_us = 0; // acumula o tempo em microssegundos para depois tirar a média.

            for (int r = 0; r < runs; r++)
            {
                ifstream arquivo(caminhoCompleto);
                if (!arquivo.is_open())
                {
                    cerr << "Erro: não foi possível abrir '" << caminhoCompleto << "'\n";
                    break;
                }

                Grafo *grafo = new Grafo();
                lerArquivoGrafo(arquivo, grafo); // cria o grafo para esta instância
                arquivo.close();

                auto t0 = high_resolution_clock::now(); // inicia a contagem de tempo
                vector<char> sol = grafo->dominanteMin2_guloso();
                auto t1 = high_resolution_clock::now();

                long long dur_us = duration_cast<microseconds>(t1 - t0).count(); // duração
                int sz = (int)sol.size();                                        // tamanho da solução

                best_size = min<long long>(best_size, sz); // guarda o menor size visto até agora
                sum_sizes += sz;
                sum_time_us += dur_us;

                delete grafo;
            }

            double mean_size = sum_sizes / runs;              // calcula o tamanho médio
            double mean_time_us = (double)sum_time_us / runs; // calcula o tempo médio

            cout << "Melhor (menor) tamanho: " << best_size << "\n";
            cout << "Tamanho médio: " << fixed << setprecision(2) << mean_size << "\n";
            cout << "Tempo médio: " << (long long)mean_time_us << " µs"
                 << " (" << setprecision(6) << mean_time_us / 1e6 << " s)\n";
        }

        // ---------- Algoritmo (b) Guloso Randomizado Adaptativo ----------
        {
            cout << "\n[B] Guloso Randomizado Adaptativo - " << runs << " execuções por alfa\n";
            for (size_t ai = 0; ai < alphas.size(); ai++)
            {
                double alpha = alphas[ai]; // seleciona um valor de alfa
                long long best_size = LLONG_MAX;
                double sum_sizes = 0.0;
                long long sum_time_us = 0;

                for (int r = 0; r < runs; r++)
                { //'runs' repetições para cada alfa
                    ifstream arquivo(caminhoCompleto);
                    if (!arquivo.is_open())
                    {
                        cerr << "Erro: não foi possível abrir '" << caminhoCompleto << "'\n";
                        break;
                    }

                    Grafo *grafo = new Grafo();
                    lerArquivoGrafo(arquivo, grafo);
                    arquivo.close();

                    auto t0 = high_resolution_clock::now();
                    // reps_adapRand garante chamar o construtivo >=30 vezes
                    vector<char> sol = grafo->dominanteMin2_gulosoAdaptRand(alpha, reps_adapRand);
                    auto t1 = high_resolution_clock::now();

                    long long dur_us = duration_cast<microseconds>(t1 - t0).count();
                    int sz = (int)sol.size();

                    best_size = min<long long>(best_size, sz);
                    sum_sizes += sz;
                    sum_time_us += dur_us;

                    delete grafo;
                }

                double mean_size = sum_sizes / runs;
                double mean_time_us = (double)sum_time_us / runs;

                cout << "\n alpha = " << fixed << setprecision(2) << alpha << "\n";
                cout << "  Melhor (menor) tamanho: " << best_size << "\n";
                cout << "  Tamanho médio: " << fixed << setprecision(2) << mean_size << "\n";
                cout << "  Tempo médio: " << (long long)mean_time_us << " µs"
                     << " (" << setprecision(6) << mean_time_us / 1e6 << " s)\n";
            }
        }

        // ---------- Algoritmo (c) Guloso Randomizado Adaptativo Reativo ----------
        {
            cout << "\n[C] Guloso Randomizado Adaptativo Reativo - " << runs << " execuções\n";
            long long best_size = LLONG_MAX;
            double sum_sizes = 0.0;
            long long sum_time_us = 0;

            for (int r = 0; r < runs; r++)
            {
                ifstream arquivo(caminhoCompleto);
                if (!arquivo.is_open())
                {
                    cerr << "Erro: não foi possível abrir '" << caminhoCompleto << "'\n";
                    break;
                }

                Grafo *grafo = new Grafo();
                lerArquivoGrafo(arquivo, grafo);
                arquivo.close();

                auto t0 = high_resolution_clock::now();
                // reps_reat = 300, reps_troca = 30 (blocos de 30)
                vector<char> sol = grafo->dominanteMin2_gulosoAdaptRandReat(alphas, reps_reat, reps_troca, /*exp_sens=*/3.0);
                auto t1 = high_resolution_clock::now();

                long long dur_us = duration_cast<microseconds>(t1 - t0).count();
                int sz = (int)sol.size();

                best_size = min<long long>(best_size, sz);
                sum_sizes += sz;
                sum_time_us += dur_us;

                delete grafo;
            }

            double mean_size = sum_sizes / runs;
            double mean_time_us = (double)sum_time_us / runs;

            cout << "Melhor (menor) tamanho: " << best_size << "\n";
            cout << "Tamanho médio: " << fixed << setprecision(2) << mean_size << "\n";
            cout << "Tempo médio: " << (long long)mean_time_us << " µs"
                 << " (" << setprecision(6) << mean_time_us / 1e6 << " s)\n";
        }

        cout << "========================================\n";

        // aguarda decisão do usuário: digite 'q' ou 'Q' para encerrar, qualquer outra tecla para continuar
        cout << "\nDigite 'q' (ou 'Q') e Enter para encerrar, ou qualquer outra tecla e Enter para continuar para a próxima instância: ";
        string resp;
        getline(cin, resp); // usa getline para ler a linha inteira

        if (!resp.empty() && (resp[0] == 'q' || resp[0] == 'Q'))
        {
            cout << "\nEncerrando conforme solicitado pelo usuário.\n";
            break;
        }
    }

    lista.close();
    return 0;
}
