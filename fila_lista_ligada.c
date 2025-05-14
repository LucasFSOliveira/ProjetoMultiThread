#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "listaLigada.h"

typedef struct _fila
{
    struct _no *ini, *fim;
    int qtde;
} Fila;
typedef Fila *p_fila;

Fila *criarFila()
{
    Fila *f;
    f = malloc(sizeof(Fila));
    f->ini = NULL;
    f->fim = NULL;
    f->qtde = 0;
    return f;
}

void destruirFila(Fila *f) {
    destroiLista(&f->ini);
    free(f);
}


void insereFila(Fila *f, int x)
{
    No *novo = malloc(sizeof(No));
    novo->dado = x;
    novo->prox = NULL;

    if (f->ini == NULL)
        f->ini = novo;
    else
        f->fim->prox = novo;
    f->fim = novo;
    f->qtde++;
}

int removeFila(Fila *f)
{
    No *primeiro = f->ini;

    if (f->ini == NULL)
    {
        f->fim = NULL;
        return 0;
    }

    int x = primeiro->dado;
    f->ini = f->ini->prox;
    f->qtde--;

    free(primeiro);
    return x;
}

int FilaVazia(Fila *Q)
{
    if (Q->qtde == 0) {
        return 1;
    }
    else {
        return 0;
    }
}

int Tamanho(Fila *Q)
{
    return Q->qtde;
}

int *getFila(Fila *Q)
{
    int *vetor = malloc(sizeof(int) * Q->qtde);
    No *atual = Q->ini;
    for (int i = 0; i < Q->qtde; i++)
    {
        vetor[i] = atual->dado;
        atual = atual->prox;
    }
    return vetor;
}