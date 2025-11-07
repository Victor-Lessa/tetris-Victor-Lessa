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
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
int pilhaVazia(Pilha *p);
int pilhaCheia(Pilha *p);
void enfileirar(Fila *f, Peca p);
Peca desenfileirar(Fila *f);
void empilhar(Pilha *p, Peca pc);
Peca desempilhar(Pilha *p);
void exibirEstado(Fila *f, Pilha *p);
Peca gerarPeca();

int main() {
    Fila fila;
    Pilha pilha;
    int opcao;
    int contadorId = 0;

    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche a fila inicial com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        Peca nova = gerarPeca();
        nova.id = contadorId++;
        enfileirar(&fila, nova);
    }

    do {
        printf("\nEstado atual:\n");
        exibirEstado(&fila, &pilha);

        printf("\nOpções de Ação:\n");
        printf("1\tJogar peça\n");
        printf("2\tReservar peça\n");
        printf("3\tUsar peça reservada\n");
        printf("0\tSair\n");
        printf("Escolha: ");
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
            case 0:
                printf("Encerrando o jogo...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

        // Após cada ação (exceto sair), gera nova peça e adiciona à fila
        if (opcao >= 1 && opcao <= 3) {
            if (!filaCheia(&fila)) {
                Peca nova = gerarPeca();
                nova.id = contadorId++;
                enfileirar(&fila, nova);
            }
        }

    } while (opcao != 0);

    return 0;
}

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->quantidade = 0;
}

// Inicializa a pilha
void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->quantidade == 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->quantidade == TAM_FILA;
}

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *p) {
    return p->topo == TAM_PILHA - 1;
}

// Insere uma peça no final da fila
void enfileirar(Fila *f, Peca p) {
    if (!filaCheia(f)) {
        f->elementos[f->fim] = p;
        f->fim = (f->fim + 1) % TAM_FILA;
        f->quantidade++;
    }
}

// Remove uma peça do início da fila
Peca desenfileirar(Fila *f) {
    Peca p = {'-', -1};
    if (!filaVazia(f)) {
        p = f->elementos[f->inicio];
        f->inicio = (f->inicio + 1) % TAM_FILA;
        f->quantidade--;
    }
    return p;
}

// Adiciona uma peça ao topo da pilha
void empilhar(Pilha *p, Peca nova) {
    if (!pilhaCheia(p)) {
        p->topo++;
        p->elementos[p->topo] = nova;
    }
}

// Remove a peça do topo da pilha
Peca desempilhar(Pilha *p) {
    Peca peca = {'-', -1};
    if (!pilhaVazia(p)) {
        peca = p->elementos[p->topo];
        p->topo--;
    }
    return peca;
}

// Exibe o estado atual da fila e da pilha
void exibirEstado(Fila *f, Pilha *p) {
    printf("Fila de peças\t");
    int i, pos = f->inicio;
    for (i = 0; i < f->quantidade; i++) {
        Peca peca = f->elementos[pos];
        printf("[%c %d] ", peca.nome, peca.id);
        pos = (pos + 1) % TAM_FILA;
    }

    printf("\nPilha de reserva\t(Topo -> Base): ");
    for (i = p->topo; i >= 0; i--) {
        printf("[%c %d] ", p->elementos[i].nome, p->elementos[i].id);
    }
    printf("\n");
}

// Gera uma nova peça aleatória
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = -1; // Será atribuído ao inserir
    return nova;
}
