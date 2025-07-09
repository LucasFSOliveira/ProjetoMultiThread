#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "fila_lista_ligada.h"

#define MAX_VIAGENS 6
#define CAPACIDADE_MAXIMA 6

#define TEMPO_VIAGEM 1
#define TEMPO_EMBARQUE_DESEMBARQUE 1
#define TEMPO_POR_PESSOA_NOVA 2

pthread_mutex_t lockFilaVip;
pthread_mutex_t lockFilaTradicional;

sem_t chamadaEmbarque;
sem_t todosEmbarcados;
sem_t chamadaDesembarque;
sem_t todosDesembarcados;
sem_t passageirosAguardando;

Fila *filaVip;
Fila *filaTradicional;

volatile int continuarRodando = 1;
volatile int carroRodando = 0;

void carregar(int viagemAtual)
{
    //printf("\n-------------------------------------------------\n\nViagem #%d vai comecar, hora de embarcar!\n", viagemAtual);
    sleep(0.1);
}
void rodar()
{
    //printf("O carro esta cheio, hora de rodar!\n");
    sleep(0.1);
    carroRodando = 1;
    //printf("O carro esta na montanha russa!\n");
    sleep(TEMPO_VIAGEM);
    carroRodando = 0;
}
void descarregar()
{
    //printf("O passeio acabou, hora de desembarcar!\n");
    sleep(1);
}
void embarcar(int embarcados)
{
    //printf("%d passageiros embarcaram no carro...\n", embarcados);
    sleep(TEMPO_EMBARQUE_DESEMBARQUE);
}
void desembarcar(int embarcados)
{
    //printf("%d passageiros desembarcaram do carro...\n", CAPACIDADE_MAXIMA - embarcados);
    sleep(TEMPO_EMBARQUE_DESEMBARQUE);
}

void imprimirFilas(Fila *filaVip, Fila *filaTradicional) {
    
    int *vip = getFila(filaVip);
    int *tradicional = getFila(filaTradicional);

    //pthread_mutex_lock(&lockFilaVip);
    printf("Fila VIP: [ ");
    for (int i = 0; i < filaVip->qtde; i++) {
        printf("%d", vip[i]);
        if (i < filaVip->qtde - 1)
            printf(", ");
    }
    printf(" ]\n");
    //pthread_mutex_unlock(&lockFilaVip);

    //pthread_mutex_lock(&lockFilaTradicional);
    printf("Fila Tradicional: [ ");
    for (int i = 0; i < filaTradicional->qtde; i++) {
        printf("%d", tradicional[i]);
        if (i < filaTradicional->qtde - 1)
            printf(", ");
    }
    printf(" ]\n");
    //pthread_mutex_unlock(&lockFilaTradicional);

    free(vip);
    free(tradicional);

}

void *monitorVisual(void *arg)
{
    while (continuarRodando)
    {
        printf("\033[2J\033[H");
        fflush(stdout);

        // Simula limpar a tela imprimindo muitas linhas em branco
        // for (int i = 0; i < 50; i++) printf("\n");

        printf("=========== PARQUE DE DIVERSOES ===========\n");

        // Estado da Montanha-Russa
        printf("\nMontanha Russa: ");
        if (carroRodando)
            printf("[ EM MOVIMENTO ]\n");
        else
            printf("[ PARADA ]\n");

        printf("\n");
        printf("                _________\n");
        printf("               /         \\\n");
        printf("         ___/               \\___\n");
        printf("       _/                         \\_\n");
        printf("      /                             \\\n");
        printf("     (                               )\n");
        printf("      \\___________________________/\n");

        // Funcionario produtor que coloca pessoas na fila
        printf("\nFuncionario (entrada de passageiros): [ PRODUTOR 1 ]\n");

        imprimirFilas(filaVip, filaTradicional);

        // Funcionario consumidor que coordena o embarque
        printf("\n\nFuncionario (embarque no carro): [ PRODUTOR 2 ]\n");

        // Estado do carrinho
        printf("\nCarrinho: ");
        if (carroRodando)
            printf("[ CARRO EM MOVIMENTO NA MONTANHA RUSSA ]\n");
        else
            printf("[ CARRO PARADO NA ESTACAO ]\n");

        printf("\n===========================================\n");

        usleep(100000);
        //printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    }

    return NULL;
}

void *threadCarro(void *arg)
{
    int viagemAtual = 0;

    while (viagemAtual < MAX_VIAGENS)
    {
        carregar(viagemAtual + 1);
        sleep(0.4);

        sem_post(&chamadaEmbarque);
        sem_wait(&todosEmbarcados);

        rodar();
        sleep(0.4);
        descarregar();

        sem_post(&chamadaDesembarque);
        sem_wait(&todosDesembarcados);

        viagemAtual++;
    }
    
    continuarRodando = 0;
    sem_post(&passageirosAguardando);
    sem_post(&chamadaEmbarque);
    sem_post(&chamadaDesembarque);

    return NULL;
}

void *funcionarioConsumidor(void *arg)
{
    while (1)
    {
        if (!continuarRodando) return NULL;
        sem_wait(&chamadaEmbarque);
        int embarcando = 1;
        int embarcados = 0;

        while (embarcando && continuarRodando)
        {
            sem_wait(&passageirosAguardando);
            embarcados++;

            if (embarcados % 2 == 0)
            {
                pthread_mutex_lock(&lockFilaVip);
                    
                if (FilaVazia(filaVip))
                {
                    pthread_mutex_lock(&lockFilaTradicional);
                    removeFila(filaTradicional);
                    embarcar(embarcados);
                    pthread_mutex_unlock(&lockFilaTradicional);
                }
                else
                {
                    removeFila(filaVip);
                    embarcar(embarcados);
                }

                pthread_mutex_unlock(&lockFilaVip);
            }
            else
            {
                pthread_mutex_lock(&lockFilaTradicional);

                if (FilaVazia(filaTradicional))
                {
                    pthread_mutex_lock(&lockFilaVip);
                    removeFila(filaVip);
                    embarcar(embarcados);
                    pthread_mutex_unlock(&lockFilaVip);
                }
                else
                {
                    
                    removeFila(filaTradicional);
                    embarcar(embarcados);
                }

                pthread_mutex_unlock(&lockFilaTradicional);
            }

            if (embarcados == CAPACIDADE_MAXIMA)
            {
                sem_post(&todosEmbarcados);
                embarcando = 0;
            }
        }

        sem_wait(&chamadaDesembarque);
        embarcados = 0;
        sleep(0.4);
        desembarcar(embarcados);
        sleep(0.4);
        sem_post(&todosDesembarcados);
    }

    return NULL;
}

void *funcionarioProdutor(void *arg)
{
    int idPassageiro = 0;
    while (1)
    {
        if (!continuarRodando) return NULL;
        idPassageiro++;

        if (idPassageiro % 3 == 0)
        {
            pthread_mutex_lock(&lockFilaVip);
            insereFila(filaVip, idPassageiro);
            pthread_mutex_unlock(&lockFilaVip);
        }
        else
        {
            pthread_mutex_lock(&lockFilaTradicional);
            insereFila(filaTradicional, idPassageiro);
            pthread_mutex_unlock(&lockFilaTradicional);
        }

        sem_post(&passageirosAguardando);
        sleep(TEMPO_POR_PESSOA_NOVA);
    }
    return NULL;
}

int main()
{
    srand(time(NULL));
    filaVip = criarFila();
    filaTradicional = criarFila();

    pthread_t produtor;
    pthread_t consumidor;
    pthread_t carro;
    pthread_t monitor;

    pthread_mutex_init(&lockFilaVip, NULL);
    pthread_mutex_init(&lockFilaTradicional, NULL);

    sem_init(&chamadaEmbarque, 0, 0);
    sem_init(&todosEmbarcados, 0, 0);
    sem_init(&chamadaDesembarque, 0, 0);
    sem_init(&todosDesembarcados, 0, 0);
    sem_init(&passageirosAguardando, 0, 0);

    printf("Hoje a montanha russa fara %d viagens!\n", MAX_VIAGENS);
    printf("A capacidade do carro e %d passageiros.\n", CAPACIDADE_MAXIMA);
    printf("Havera passageiros ilimitados esperando na fila!\n\n");
    
    sleep(1);
    printf("A montanha russa esta pronta para comecar!\n");
    sleep(1);
    
    pthread_create(&produtor, NULL, funcionarioProdutor, NULL);
    pthread_create(&consumidor, NULL, funcionarioConsumidor, NULL);
    pthread_create(&carro, NULL, threadCarro, NULL);
    pthread_create(&monitor, NULL, monitorVisual, NULL);

    pthread_join(produtor, NULL);
    pthread_join(consumidor, NULL);
    pthread_join(carro, NULL);
    pthread_join(monitor, NULL);

    printf("Essas foram todas as viagens de hoje, a montanha russa esta encerrando.\n");

    sem_destroy(&chamadaEmbarque);
    sem_destroy(&todosEmbarcados);
    sem_destroy(&chamadaDesembarque);
    sem_destroy(&todosDesembarcados);
    sem_destroy(&passageirosAguardando);

    destruirFila(filaVip);
    destruirFila(filaTradicional);

    return 0;
}
