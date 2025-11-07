#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5

// Estrutura que representa uma peça
typedef struct {
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca fila[TAMANHO_FILA];
    int inicio;
    int fim;
    int quantidade;
} FilaPecas;

// Protótipos das funções
void inicializarFila(FilaPecas *fila);
int estaCheia(FilaPecas *fila);
int estaVazia(FilaPecas *fila);
void enfileirar(FilaPecas *fila, Peca nova);
Peca desenfileirar(FilaPecas *fila);
void exibirFila(FilaPecas *fila);
Peca gerarPeca();

// Função principal
int main() {
    FilaPecas fila;
    int opcao;
    int contadorId = 0;

    srand(time(NULL)); // Inicializa gerador de números aleatórios
    inicializarFila(&fila);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAMANHO_FILA; i++) {
        Peca nova = gerarPeca();
        nova.id = contadorId++;
        enfileirar(&fila, nova);
    }

    do {
        printf("\nFila de peças:\n");
        exibirFila(&fila);

        printf("\nOpções de ação:\n");
        printf("1\tJogar peça (dequeue)\n");
        printf("2\tInserir nova peça (enqueue)\n");
        printf("0\tSair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (!estaVazia(&fila)) {
                    Peca jogada = desenfileirar(&fila);
                    printf("Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                } else {
                    printf("Fila vazia! Nenhuma peça para jogar.\n");
                }
                break;
            case 2:
                if (!estaCheia(&fila)) {
                    Peca nova = gerarPeca();
                    nova.id = contadorId++;
                    enfileirar(&fila, nova);
                    printf("Nova peça inserida: [%c %d]\n", nova.nome, nova.id);
                } else {
                    printf("Fila cheia! Não é possível inserir nova peça.\n");
                }
                break;
            case 0:
                printf("Encerrando o jogo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}

// Inicializa a fila
void inicializarFila(FilaPecas *fila) {
    fila->inicio = 0;
    fila->fim = 0;
    fila->quantidade = 0;
}

// Verifica se a fila está cheia
int estaCheia(FilaPecas *fila) {
    return fila->quantidade == TAMANHO_FILA;
}

// Verifica se a fila está vazia
int estaVazia(FilaPecas *fila) {
    return fila->quantidade == 0;
}

// Insere uma peça no final da fila
void enfileirar(FilaPecas *fila, Peca nova) {
    if (!estaCheia(fila)) {
        fila->fila[fila->fim] = nova;
        fila->fim = (fila->fim + 1) % TAMANHO_FILA;
        fila->quantidade++;
    }
}

// Remove uma peça do início da fila
Peca desenfileirar(FilaPecas *fila) {
    Peca removida = {'-', -1};
    if (!estaVazia(fila)) {
        removida = fila->fila[fila->inicio];
        fila->inicio = (fila->inicio + 1) % TAMANHO_FILA;
        fila->quantidade--;
    }
    return removida;
}

// Exibe o conteúdo da fila
void exibirFila(FilaPecas *fila) {
    int i, pos;
    for (i = 0, pos = fila->inicio; i < fila->quantidade; i++) {
        Peca p = fila->fila[pos];
        printf("[%c %d] ", p.nome, p.id);
        pos = (pos + 1) % TAMANHO_FILA;
    }
    printf("\n");
}

// Gera uma nova peça aleatória
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = -1; // Será atribuído no momento da inserção
    return nova;
}
