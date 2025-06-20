#include "Aresta.h"

Aresta::Aresta(){
    id_no_alvo='\0';
    peso=1;
    retorno=false;
}

Aresta::Aresta(char id_alvo){
    id_no_alvo=id_alvo;
    retorno=false;
    peso=1;
}