#include "./Grafo.h"
#include <cstdlib> // rand, srand
#include <ctime>   // time
#include <cmath>

using namespace std;

vector<char> Grafo::dominanteMin2_guloso()
{
    vector<NoLA2 *> listaAdj2;
    int cont = 0;
    bool *visitado = new bool[ordem];
    for (int i = 0; i < ordem; i++)
        visitado[i] = false;
    for (No *no : this->lista_adj)
    {
        NoLA2 *novoNo = new NoLA2();
        novoNo->posLista = cont;
        novoNo->id = no->id;
        novoNo->peso = no->peso;
        visitado[cont]=true;
        buscaProfundidadeNivelK(no, visitado, no->id, 1, novoNo->arestasPrim);
        buscaProfundidadeNivelK(no, visitado, no->id, 2, novoNo->arestasSec);
        novoNo->cobertosPrim = novoNo->arestasPrim.size() + 1;
        novoNo->cobertosSec = novoNo->arestasSec.size();
        novoNo->cobertosTotal = novoNo->cobertosPrim + novoNo->cobertosSec;
        for (int i = 0; i < ordem; i++)
            visitado[i] = false;
        listaAdj2.push_back(novoNo);
        cont++;
    }

    vector<char> solucao;

    vector<NoLA2 *> candidatos;

    int num_cobertos = 0;

    int contador_passos = 0;

    candidatos = listaAdj2;

    std::sort(candidatos.begin()+contador_passos, candidatos.end(), [](NoLA2* noA, NoLA2* noB)
                  {
                      return (noA->cobertosTotal > noB->cobertosTotal) || (noA->cobertosTotal == noB->cobertosTotal && noA->cobertosPrim > noB->cobertosPrim); // ordena em ordem decrescente
                  });

    while (num_cobertos < this->ordem)
    {
        NoLA2 *atual = candidatos[contador_passos];
        int cobertos_atual = 0;
        if (atual->cobre_a_si)
            cobertos_atual++;
        for (Aresta *a : atual->arestasPrim)
        {
            if (listaAdj2[a->posicao_alvo_lista_adj]->cobre_a_si && !listaAdj2[a->posicao_alvo_lista_adj]->na_solucao)
            {
                cobertos_atual++;
                listaAdj2[a->posicao_alvo_lista_adj]->cobre_a_si = false;
            }
        }
        for (Aresta *a : atual->arestasSec)
        {
            if (listaAdj2[a->posicao_alvo_lista_adj]->cobre_a_si && !listaAdj2[a->posicao_alvo_lista_adj]->na_solucao)
            {
                cobertos_atual++;
                listaAdj2[a->posicao_alvo_lista_adj]->cobre_a_si = false;
            }
        }
        num_cobertos += cobertos_atual;
        if (cobertos_atual > 0)
        {
            atual->na_solucao = true;
            solucao.push_back(atual->id);
        }
        contador_passos++;
        
    }
    
    int total_a_desalocar_nos=listaAdj2.size();
    for(int i=0;i<total_a_desalocar_nos;i++){
        int total_a_desalocar_arestas= listaAdj2[i]->arestasPrim.size();
        for(int j=0; j< total_a_desalocar_arestas;j++){
            delete listaAdj2[i]->arestasPrim[j];
        }
        total_a_desalocar_arestas= listaAdj2[i]->arestasSec.size();
        for(int j=0; j< total_a_desalocar_arestas;j++){
            delete listaAdj2[i]->arestasSec[j];
        }
        delete listaAdj2[i];
    }

    return solucao;
}

void Grafo::buscaProfundidadeNivelK(No *no, bool *visitado, char id_pai, int nivel_k, vector<Aresta *> &k_arestas)
{
    nivel_k--;
    int pos_lista = 0;
    for (Aresta *aresta : no->arestas)
    {
        pos_lista = aresta->posicao_alvo_lista_adj;

        
            
            if (nivel_k < 0)
                return;
            if (nivel_k == 0)
            {
                if (!visitado[pos_lista])
                {
                    visitado[pos_lista] = true;
                    Aresta *ar = new Aresta(aresta->id_no_alvo);
                    ar->peso = aresta->peso;
                    ar->posicao_alvo_lista_adj = pos_lista;
                    k_arestas.push_back(ar);
                }
            }
            else
                buscaProfundidadeNivelK(lista_adj[pos_lista], visitado, no->id, nivel_k, k_arestas);
        
    }
}

vector<char> Grafo::dominanteMin2_gulosoAdaptRand(double alpha, int repeticoes)
{
    return auxiliar_domM_dist2_adaptRand(alpha,repeticoes);
}

vector<char> Grafo::dominanteMin2_gulosoAdaptRandReat(vector<double> alphas,int repeticoes,int reps_para_recal,double exp_sens)
{
    double prob_ini=1/alphas.size();
    vector<double> probabilidades(alphas.size(),prob_ini);
    vector<double> medias(alphas.size(),0);
    return auxiliar_domM_dist2_adaptRandReat(alphas,probabilidades,medias,repeticoes,reps_para_recal,exp_sens);
}



vector<char> Grafo::auxiliar_domM_dist2_adaptRand(double alpha, int repeticoes)
{
    
   
    vector<NoLA2 *> listaAdj2(lista_adj.size());
  
    int N=lista_adj.size();
    vector<int> primOrig(N), secOrig(N);
    
    inicializaListaAlgGul(listaAdj2,primOrig,secOrig);
    
   


    vector<char> solucao_final;
   
    for (int reps_exec = 0; reps_exec < repeticoes; reps_exec++)
    {
        execucao_alg_guloso_rand_adap(alpha, listaAdj2,primOrig,secOrig, solucao_final, reps_exec == 0);
    }

    int total_a_desalocar_nos=listaAdj2.size();
    for(int i=0;i<total_a_desalocar_nos;i++){
        int total_a_desalocar_arestas= listaAdj2[i]->arestasPrim.size();
        for(int j=0; j< total_a_desalocar_arestas;j++){
            delete listaAdj2[i]->arestasPrim[j];
        }
        total_a_desalocar_arestas= listaAdj2[i]->arestasSec.size();
        for(int j=0; j< total_a_desalocar_arestas;j++){
            delete listaAdj2[i]->arestasSec[j];
        }
        delete listaAdj2[i];
    }

   

    return solucao_final;
}

vector<char> Grafo::auxiliar_domM_dist2_adaptRandReat(vector<double> alphas, vector<double> probabilidades, vector<double> medias, int repeticoes, int reps_para_recal, double exp_sens)
{
   

    vector<NoLA2 *> listaAdj2(lista_adj.size());
    int N=lista_adj.size();
    vector<int> primOrig(N), secOrig(N);
    inicializaListaAlgGul(listaAdj2,primOrig,secOrig);
    

    vector<char> solucao_otima;

    int indice_alpha_atual = 0;


    vector<long long> somas_soluces_alphas(alphas.size(), 0);
    vector<int> execucoes_alphas(alphas.size(), 0);

    inicializacao_reativo(listaAdj2,primOrig,secOrig,alphas,probabilidades,medias,10,exp_sens,solucao_otima,somas_soluces_alphas,execucoes_alphas);

    
    

    for (int reps_exec = 0; reps_exec < repeticoes; reps_exec++)
    {
        if(reps_exec%reps_para_recal==0)
        {
            recalcula_probabilidades(probabilidades,solucao_otima.size(),medias,exp_sens);
        }    

        indice_alpha_atual = sorteia_indice_alpha(probabilidades);

        int tam_sol_atual=execucao_alg_guloso_rand_adap(alphas[indice_alpha_atual], listaAdj2,primOrig,secOrig, solucao_otima, false);

         execucoes_alphas[indice_alpha_atual]++;
            somas_soluces_alphas[indice_alpha_atual]+=tam_sol_atual;
            medias[indice_alpha_atual]= double(somas_soluces_alphas[indice_alpha_atual])/execucoes_alphas[indice_alpha_atual];
    }


    int total_a_desalocar_nos=listaAdj2.size();
    for(int i=0;i<total_a_desalocar_nos;i++){
        int total_a_desalocar_arestas= listaAdj2[i]->arestasPrim.size();
        for(int j=0; j< total_a_desalocar_arestas;j++){
            delete listaAdj2[i]->arestasPrim[j];
        }
        total_a_desalocar_arestas= listaAdj2[i]->arestasSec.size();
        for(int j=0; j< total_a_desalocar_arestas;j++){
            delete listaAdj2[i]->arestasSec[j];
        }
        delete listaAdj2[i];
    }
    
    return solucao_otima;
}

void Grafo::inicializacao_reativo(vector<NoLA2 *> &listaAdj2,const vector<int> &primOrig, const vector<int> &secOrig, vector<double> &alphas, vector<double> &probabilidades, vector<double> &medias, int repeticoes_ini, double exp_sens, vector<char> &solucao_otima,vector<long long> &somas_soluces_alphas,vector<int> &execucoes_alphas)
{
    
    for (int alpha_atual = 0; alpha_atual < alphas.size(); alpha_atual++)
    {
        for (int reps_exec = 0; reps_exec < repeticoes_ini; reps_exec++)
        {
            int tam_sol_atual= execucao_alg_guloso_rand_adap(alphas[alpha_atual],listaAdj2,primOrig,secOrig,solucao_otima,(alpha_atual==0 && reps_exec==0));
            execucoes_alphas[alpha_atual]++;
            somas_soluces_alphas[alpha_atual]+=tam_sol_atual;
            medias[alpha_atual]= double(somas_soluces_alphas[alpha_atual])/execucoes_alphas[alpha_atual];
            
        }
    }
}
int Grafo::sorteia_indice_alpha(vector<double> &probabilidades)
{
  
    double r = double (std::rand()) / RAND_MAX; // gera um inteiro aleatório entre 0 e RAND_MAX

    double acumulado = 0.0;
    for (int i = 0; i < probabilidades.size(); i++)
    {
        acumulado += probabilidades[i];
        if (r < acumulado)
            return i;
    }

    return probabilidades.size()-1;
}
void Grafo::recalcula_probabilidades(vector<double> &probabilidades, int f_sol_otm, vector<double> &medias, double exp_sens)
{

    vector<double> coef_q(probabilidades.size());
    double soma_qs = 0.0;
    for (int i = 0; i < probabilidades.size(); i++)
    {
        if(medias[i]==0)
            coef_q[i]=pow(0.5,exp_sens);
        else    
            coef_q[i] = pow(double(f_sol_otm) / medias[i], exp_sens);
        soma_qs += coef_q[i];
    }

    for (int i = 0; i < probabilidades.size(); i++)
    {
        probabilidades[i] = coef_q[i] / soma_qs;
    }
}

void Grafo::inicializaListaAlgGul(vector<NoLA2*> &listaAdj2, vector<int> &primOrig, vector<int> &secOrig){
    bool *visitado = new bool[ordem];
    int cont = 0;
    
    for (int i = 0; i < ordem; i++)
        visitado[i] = false;
    for (No *no : this->lista_adj)
    {
        NoLA2 *novoNo = new NoLA2();
        novoNo->posLista = cont;
        novoNo->id = no->id;
        novoNo->peso = no->peso;
        visitado[cont]=true;
        buscaProfundidadeNivelK(no, visitado, no->id, 1, novoNo->arestasPrim);
        buscaProfundidadeNivelK(no, visitado, no->id, 2, novoNo->arestasSec);
        novoNo->cobertosPrim = novoNo->arestasPrim.size() + 1;
        primOrig[cont]=novoNo->cobertosPrim;
        novoNo->cobertosSec = novoNo->arestasSec.size();
        secOrig[cont]=novoNo->cobertosSec;
        novoNo->cobertosTotal = novoNo->cobertosPrim + novoNo->cobertosSec;
        for (int i = 0; i < ordem; i++)
            visitado[i] = false;
        listaAdj2[cont] =novoNo;
        cont++;
    }

    delete [] visitado;
}

int Grafo::execucao_alg_guloso_rand_adap(float alpha, vector<NoLA2 *> &listaAdj2,const vector<int> &primOrig, const vector<int> &secOrig, vector<char> &solucao_otima, bool primeira_iter)
{
    int num_cobertos = 0;

    vector<char> solucao;

    int contador_passos = 0;

    vector<NoLA2 *> candidatos = listaAdj2;

    int tam_atual = candidatos.size();

    std::sort(candidatos.begin()+contador_passos, candidatos.end(), [](NoLA2* noA, NoLA2* noB)
                  {
                      return (noA->cobertosTotal > noB->cobertosTotal) || (noA->cobertosTotal == noB->cobertosTotal && noA->cobertosPrim > noB->cobertosPrim); // ordena em ordem decrescente
                  });
    auto iterador = candidatos.begin();


    
    while (true)
    {
        NoLA2 *atual = candidatos[contador_passos];
        int cobertos_atual = 0;
        if (atual->cobre_a_si)
            cobertos_atual++;
        for (Aresta *a : atual->arestasPrim)
        {
            if (listaAdj2[a->posicao_alvo_lista_adj]->cobre_a_si  && !listaAdj2[a->posicao_alvo_lista_adj]->na_solucao)
            {
                cobertos_atual++;
                listaAdj2[a->posicao_alvo_lista_adj]->cobre_a_si = false;
                NoLA2 *noCob = listaAdj2[a->posicao_alvo_lista_adj];
                for (Aresta *b : noCob->arestasPrim)
                {
                    listaAdj2[b->posicao_alvo_lista_adj]->alteraCobertPrim(-1);
                }
                for (Aresta *b : noCob->arestasSec)
                {
                    listaAdj2[b->posicao_alvo_lista_adj]->alteraCobertSec(-1);
                }
                noCob->alteraCobertPrim(-1);
            }
        }
        for (Aresta *a : atual->arestasSec)
        {
            if (listaAdj2[a->posicao_alvo_lista_adj]->cobre_a_si  && !listaAdj2[a->posicao_alvo_lista_adj]->na_solucao)
            {
                cobertos_atual++;
                listaAdj2[a->posicao_alvo_lista_adj]->cobre_a_si = false;
                NoLA2 *noCob = listaAdj2[a->posicao_alvo_lista_adj];
                for (Aresta *b : noCob->arestasPrim)
                {
                    listaAdj2[b->posicao_alvo_lista_adj]->alteraCobertPrim(-1);
                }
                for (Aresta *b : noCob->arestasSec)
                {
                    listaAdj2[b->posicao_alvo_lista_adj]->alteraCobertSec(-1);
                }
                noCob->alteraCobertPrim(-1);
            }
        }


        num_cobertos += cobertos_atual;
        if (cobertos_atual > 0)
        {
            atual->na_solucao = true;
            solucao.push_back(atual->id);
        }
        contador_passos++;
        tam_atual--;
        iterador++;
        std::sort(candidatos.begin()+contador_passos, candidatos.end(), [](NoLA2* noA, NoLA2* noB)
                  {
                      return (noA->cobertosTotal > noB->cobertosTotal)  || (noA->cobertosTotal == noB->cobertosTotal && noA->cobertosPrim > noB->cobertosPrim) ; // ordena em ordem decrescente
                  });

        if (num_cobertos >= this->ordem)
            break;
        int r = std::rand(); // gera um inteiro aleatório entre 0 e RAND_MAX
        int max = std::ceil(alpha * tam_atual);
        if(max==0)
            max=1;
        int prox = contador_passos + r % max;
        while (candidatos[prox]->cobertosTotal == 0)
        {
            tam_atual = prox - contador_passos;
            max = std::ceil(alpha * tam_atual);
            prox = contador_passos + r % max;
            if(max==0)
                max=1;
        }

        if (prox != contador_passos)
        {
            NoLA2 *aux = candidatos[contador_passos];
            candidatos[contador_passos] = candidatos[prox];
            candidatos[prox] = aux;
        }
    }
    if (primeira_iter || solucao.size() < solucao_otima.size())
        solucao_otima = solucao;
    
    int contador=0;
    for (NoLA2 *no : listaAdj2)
    {
        no->cobre_a_si = true;
        no->na_solucao = false;
        no->cobertosPrim=primOrig[contador];
        no->cobertosSec=secOrig[contador];
        no->cobertosTotal=no->cobertosPrim+no->cobertosSec;
        contador++;
    }

    return solucao.size();
}