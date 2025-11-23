#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------------------------------------------
   Struct Sala
   Cada nó da árvore binária representa uma sala da mansão.
   Contém:
   - nome da sala
   - pista (string, pode ser NULL)
   - ponteiros para salas à esquerda e direita
--------------------------------------------------------- */
typedef struct sala {
    char nome[50];
    char* pista; 
    struct sala* esquerda;
    struct sala* direita;
} Sala;

/* ---------------------------------------------------------
   Struct PistaNode
   Cada nó da BST armazena uma pista coletada.
   --------------------------------------------------------- */
typedef struct pistaNode {
    char* pista;
    struct pistaNode* esquerda;
    struct pistaNode* direita;
} PistaNode;

/* ---------------------------------------------------------
   criarSala()
   Cria dinamicamente uma sala com nome e pista.
--------------------------------------------------------- */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memória para a sala!\n");
        exit(1);
    }
    strcpy(nova->nome, nome);
    if (pista != NULL) {
        nova->pista = (char*) malloc(strlen(pista) + 1);
        strcpy(nova->pista, pista);
    } else {
        nova->pista = NULL;
    }
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/* ---------------------------------------------------------
   criarPistaNode()
   Cria um nó da BST para armazenar uma pista.
--------------------------------------------------------- */
PistaNode* criarPistaNode(const char* pista) {
    PistaNode* novo = (PistaNode*) malloc(sizeof(PistaNode));
    if (!novo) {
        printf("Erro ao alocar memória para a pista!\n");
        exit(1);
    }
    novo->pista = (char*) malloc(strlen(pista) + 1);
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

/* ---------------------------------------------------------
   inserirPista()
   Insere uma pista na BST (ordenada alfabeticamente)
--------------------------------------------------------- */
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        return criarPistaNode(pista);
    }
    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (strcmp(pista, raiz->pista) > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se a pista já existir, não insere duplicado
    return raiz;
}

/* ---------------------------------------------------------
   exibirPistas()
   Exibe a BST de pistas em ordem alfabética
--------------------------------------------------------- */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf(" - %s\n", raiz->pista);
        exibirPistas(raiz->direita);
    }
}

/* ---------------------------------------------------------
   explorarSalasComPistas()
   Permite ao jogador explorar a mansão e coletar pistas
--------------------------------------------------------- */
void explorarSalasComPistas(Sala* atual, PistaNode** arvorePistas) {
    char opcao;

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Coleta a pista se houver
        if (atual->pista != NULL) {
            printf("Você encontrou uma pista: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        }

        // Verifica se é um nó-folha
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Não há mais caminhos nesta sala.\n");
            break;
        }

        printf("\nEscolha seu caminho:\n");
        printf("  (e) Ir para a esquerda\n");
        printf("  (d) Ir para a direita\n");
        printf("  (s) Sair da exploração\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        if (opcao == 'e') {
            atual = atual->esquerda;
        } else if (opcao == 'd') {
            atual = atual->direita;
        } else if (opcao == 's') {
            printf("Exploração encerrada pelo jogador.\n");
            break;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

/* ---------------------------------------------------------
   Função principal
--------------------------------------------------------- */
int main() {
    // BST de pistas coletadas
    PistaNode* arvorePistas = NULL;

    // Montagem fixa da mansão
    Sala* hall = criarSala("Hall de Entrada", "Chave antiga encontrada");
    hall->esquerda = criarSala("Sala de Estar", "Rastro de pegadas");
    hall->direita = criarSala("Cozinha", "Garrafa quebrada");
    hall->esquerda->esquerda = criarSala("Biblioteca", "Livro fora do lugar");
    hall->esquerda->direita  = criarSala("Jardim", "Pegadas na terra molhada");
    hall->direita->esquerda  = criarSala("Despensa", "Cacos de vidro");
    hall->direita->direita   = criarSala("Garagem", "Rastro de pneu");

    printf("Bem-vindo ao Detective Quest!\n");
    printf("Explore a mansão e colete todas as pistas.\n");

    // Exploração da mansão com coleta de pistas
    explorarSalasComPistas(hall, &arvorePistas);

    // Exibe todas as pistas coletadas em ordem alfabética
    printf("\n=== Pistas coletadas em ordem alfabética ===\n");
    exibirPistas(arvorePistas);

    return 0;
}
