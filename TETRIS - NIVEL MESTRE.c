#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5
#define TAM_PILHA 3

// Estrutura que representa uma peça
typedef struct {
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca elementos[TAM_FILA];
    int inicio;
    int fim;
    int quantidade;
} Fila;

// Estrutura da pilha
typedef struct {
    Peca elementos[TAM_PILHA];
    int topo;
} Pilha;

// Funções auxiliares
void inicializarFila(Fila *fila);
void inicializarPilha(Pilha *pilha);
int filaVazia(Fila *fila);
int filaCheia(Fila *fila);
int pilhaVazia(Pilha *pilha);
int pilhaCheia(Pilha *pilha);
void enfileirar(Fila *fila, Peca nova);
Peca desenfileirar(Fila *fila);
void empilhar(Pilha *pilha, Peca novaPeca);
Peca desempilhar(Pilha *pilha);
void exibirEstado(Fila *fila, Pilha *pilha);
Peca gerarPeca();
void trocarTopoComFrente(Fila *fila, Pilha *pilha);
void trocaMultipla(Fila *fila, Pilha *pilha);

int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    int contadorId = 0;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila com 5 peças iniciais
    for (int i = 0; i < TAM_FILA; i++) {
        Peca nova = gerarPeca();
        nova.id = contadorId++;
        enfileirar(&fila, nova);
    }

    do {
        printf("\nEstado atual:\n");
        exibirEstado(&fila, &pilha);

        printf("\nOpções disponíveis:\n");
        printf("1\tJogar peça da frente da fila\n");
        printf("2\tEnviar peça da fila para a pilha de reserva\n");
        printf("3\tUsar peça da pilha de reserva\n");
        printf("4\tTrocar peça da frente da fila com o topo da pilha\n");
        printf("5\tTrocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0\tSair\n");
        printf("Opção escolhida: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (!filaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                } else {
                    printf("Fila vazia!\n");
                }
                break;
            case 2:
                if (!filaVazia(&fila) && !pilhaCheia(&pilha)) {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("Peça reservada: [%c %d]\n", reservada.nome, reservada.id);
                } else {
                    printf("Não foi possível reservar a peça (fila vazia ou pilha cheia).\n");
                }
                break;
            case 3:
                if (!pilhaVazia(&pilha)) {
                    Peca usada = desempilhar(&pilha);
                    printf("Peça usada da reserva: [%c %d]\n", usada.nome, usada.id);
                } else {
                    printf("Pilha de reserva vazia!\n");
                }
                break;
            case 4:
                trocarTopoComFrente(&fila, &pilha);
                break;
            case 5:
                trocaMultipla(&fila, &pilha);
                break;
            case 0:
                printf("Encerrando o jogo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

        // Após ações que removem da fila, repõe com nova peça
        if ((opcao >= 1 && opcao <= 2) && !filaCheia(&fila)) {
            Peca nova = gerarPeca();
            nova.id = contadorId++;
            enfileirar(&fila, nova);
        }

    } while (opcao != 0);

    return 0;
}

// Inicializa a fila
void inicializarFila(Fila *fila) {
    fila->inicio = 0;
    fila->fim = 0;
    fila->quantidade = 0;
}

// Inicializa a pilha
void inicializarPilha(Pilha *pilha) {
    pilha->topo = -1;
}

// Verificações
int filaVazia(Fila *fila) {
    return fila->quantidade == 0;
}

int filaCheia(Fila *fila) {
    return fila->quantidade == TAM_FILA;
}

int pilhaVazia(Pilha *pilha) {
    return pilha->topo == -1;
}

int pilhaCheia(Pilha *pilha) {
    return pilha->topo == TAM_PILHA - 1;
}

// Operações de fila
void enfileirar(Fila *fila, Peca nova) {
    if (!filaCheia(fila)) {
        fila->elementos[fila->fim] = nova;
        fila->fim = (fila->fim + 1) % TAM_FILA;
        fila->quantidade++;
    }
}

Peca desenfileirar(Fila *fila) {
    Peca p = {'-', -1};
    if (!filaVazia(fila)) {
        p = fila->elementos[fila->inicio];
        fila->inicio = (fila->inicio + 1) % TAM_FILA;
        fila->quantidade--;
    }
    return p;
}

// Operações de pilha
void empilhar(Pilha *pilha, Peca novaPeca) {
    if (!pilhaCheia(pilha)) {
        pilha->topo++;
        pilha->elementos[pilha->topo] = novaPeca;
    }
}

Peca desempilhar(Pilha *pilha) {
    Peca p = {'-', -1};
    if (!pilhaVazia(pilha)) {
        p = pilha->elementos[pilha->topo];
        pilha->topo--;
    }
    return p;
}

// Exibe o estado atual
void exibirEstado(Fila *fila, Pilha *pilha) {
    printf("Fila de peças\t");
    int i, pos = fila->inicio;
    for (i = 0; i < fila->quantidade; i++) {
        Peca peca = fila->elementos[pos];
        printf("[%c %d] ", peca.nome, peca.id);
        pos = (pos + 1) % TAM_FILA;
    }

    printf("\nPilha de reserva\t(Topo -> base): ");
    for (i = pilha->topo; i >= 0; i--) {
        printf("[%c %d] ", pilha->elementos[i].nome, pilha->elementos[i].id);
    }
    printf("\n");
}

// Gera nova peça aleatória
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = -1;
    return nova;
}

// Troca a peça da frente da fila com o topo da pilha
void trocarTopo;