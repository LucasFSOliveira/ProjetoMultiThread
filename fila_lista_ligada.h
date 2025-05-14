#include "listaLigada.h"

typedef struct _fila
{
    struct _no *ini, *fim;
    int qtde;
} Fila;

typedef Fila *p_fila;

Fila *criarFila();
void destruirFila(Fila *f);
void insereFila(Fila *f, int x);
int removeFila(Fila *f);
int FilaVazia(Fila *Q);
int Tamanho(Fila *Q);
int *getFila(Fila *Q);