# 🎢 Montanha-Russa Multithread: Produtor-Consumidor com Fila VIP

## 📌 Descrição

Este projeto simula o funcionamento de uma montanha-russa utilizando **programação multithread em C**, com **semáforos**, **mutexes** e **filas com lista ligada**. Ele resolve uma variação do problema clássico **Produtor-Consumidor**, ambientado em um parque de diversões onde há filas VIP e Tradicional.

A proposta é modelar com precisão a lógica de embarque e desembarque de passageiros, priorizando a sincronização entre threads produtoras, consumidoras e o carrinho da montanha-russa.

## 🎯 Objetivo

- Gerenciar **6 viagens** de um carrinho com **capacidade para 6 passageiros** por viagem.
- Controlar o acesso a **duas filas (VIP e Tradicional)**, com prioridade alternada.
- Sincronizar as ações de:
  - Um produtor que insere passageiros nas filas;
  - Um consumidor que coordena o embarque e desembarque;
  - Um carrinho que executa as viagens;
  - Um monitor visual que atualiza o estado do parque.

## 🧵 Conceitos Aplicados

- Programação concorrente com **pthreads**
- **Mutexes** (`pthread_mutex_t`) para controle de acesso às filas
- **Semáforos** (`sem_t`) para sincronização entre as etapas do sistema
- **Fila com lista ligada** para gerenciar a ordem de embarque dos passageiros
- Simulação visual em tempo real (limpeza da tela com escape ANSI)

## 🛠️ Como Executar

1. Compile o programa:
   ```bash
   gcc -o montanha_russa main.c fila_lista_ligada.c -lpthread
2. Execute:
   ```bash
   ./montanha_russa

## 📁 Estrutura do Código

- `main.c`: Contém toda a lógica multithread do sistema.
- `fila_lista_ligada.h` / `fila_lista_ligada.c`: Estrutura de fila dinâmica baseada em lista ligada.
- **Threads principais:**
  - `funcionarioProdutor`: Gera novos passageiros nas filas (VIP ou Tradicional).
  - `funcionarioConsumidor`: Controla o embarque e desembarque com base na prioridade alternada.
  - `threadCarro`: Realiza o ciclo das viagens.
  - `monitorVisual`: Atualiza o status visual das filas e do carrinho em tempo real.

## ⚙️ Parâmetros Configuráveis

- `#define MAX_VIAGENS 6`: Número total de viagens realizadas pelo carrinho.
- `#define CAPACIDADE_MAXIMA 6`: Capacidade de passageiros por viagem.
- `#define TEMPO_VIAGEM`, `TEMPO_EMBARQUE_DESEMBARQUE`, `TEMPO_POR_PESSOA_NOVA`: Controlam a duração de cada fase do processo (viagem, embarque/desembarque e tempo de chegada de novos passageiros).
