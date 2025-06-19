#include "Aresta.h"

Aresta::Aresta(){
    id_no_alvo='\0';
    peso=1;
}

Aresta::Aresta(int id_alvo){
    id_no_alvo=id_alvo;
    peso=1;
}