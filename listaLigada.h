#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef struct _no {
    int dado;
    struct _no *prox;
} No;

int isNullList(No *listaligada);

void removeInicioLista(No **inicio);

No *retornaNo(No *NoAtual, int indice);

void destroiLista(No **inicio);