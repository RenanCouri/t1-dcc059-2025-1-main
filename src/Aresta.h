#ifndef ARESTA_H
#define ARESTA_H

using namespace std;
class Aresta {
public:
    Aresta();
    ~Aresta(){};
    Aresta(char id_alvo);
    char id_no_alvo;
    int peso;
    bool retorno;
};



#endif //ARESTA_H
