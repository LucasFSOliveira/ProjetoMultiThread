#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "listaLigada.h"

int isNullList(No *listaligada) {
    if(listaligada == NULL)
        return 1;
    else
        return 0;
}

void removeInicioLista(No **inicio) {
    No *p = (*inicio);
    if (!isNullList(*inicio)) {
        (*inicio) = (*inicio)->prox;
        free(p);
    }
}

No *retornaNo(No *NoAtual, int indice){
    No *NoRetorno = NoAtual;
    for(int i = 0; i < indice; i++)
        NoRetorno = NoRetorno->prox;
    return NoRetorno;
}

void destroiLista(No **inicio) {
    while (isNullList(*inicio) != 1)
        removeInicioLista(inicio);
}