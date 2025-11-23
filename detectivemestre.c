#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 10

/* ---------------------------------------------------------
   Struct Sala
   Representa um cômodo da mansão.
--------------------------------------------------------- */
typedef struct sala {
    char nome[50];
    char* pista; 
    struct sala* esquerda;
    struct sala* direita;
} Sala;

/* ---------------------------------------------------------
   Struct PistaNode
   BST para armazenar pistas coletadas em ordem alfabética
--------------------------------------------------------- */
typedef struct pistaNode {
    char* pista;
    struct pistaNode* esquerda;
    struct pistaNode* direita;
} PistaNode;

/* ---------------------------------------------------------
   Tabela hash: chave = pista, valor = suspeito
--------------------------------------------------------- */
typedef struct hashNode {
    char* pista;
    char* suspeito;
    struct hashNode* proximo;
} HashNode;

HashNode* tabelaHash[HASH_SIZE] = {NULL};

/* ---------------------------------------------------------
   Função hash simples: soma dos caracteres modulo HASH_SIZE
--------------------------------------------------------- */
int hash(const char* chave) {
    int soma = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        soma += chave[i];
    }
    return soma % HASH_SIZE;
}

/* ---------------------------------------------------------
   Inserir na hash
--------------------------------------------------------- */
void inserirNaHash(const char* pista, const char* suspeito) {
    int indice = hash(pista);
    HashNode* novo = (HashNode*) malloc(sizeof(HashNode));
    novo->pista = (char*) malloc(strlen(pista)+1);
    novo->suspeito = (char*) malloc(strlen(suspeito)+1);
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->proximo = tabelaHash[indice];
    tabelaHash[indice] = novo;
}

/* ---------------------------------------------------------
   Encontrar suspeito pela pista
--------------------------------------------------------- */
char* encontrarSuspeito(const char* pista) {
    int indice = hash(pista);
    HashNode* atual = tabelaHash[indice];
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    return NULL;
}

/* ---------------------------------------------------------
   Cria dinamicamente uma sala
--------------------------------------------------------- */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    if (pista != NULL) {
        nova->pista = (char*) malloc(strlen(pista)+1);
        strcpy(nova->pista, pista);
    } else {
        nova->pista = NULL;
    }
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/* ---------------------------------------------------------
   Cria nó de BST de pistas
--------------------------------------------------------- */
PistaNode* criarPistaNode(const char* pista) {
    PistaNode* novo = (PistaNode*) malloc(sizeof(PistaNode));
    novo->pista = (char*) malloc(strlen(pista)+1);
    strcpy(novo->pista, pista);
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

/* ---------------------------------------------------------
   Inserir pista na BST
--------------------------------------------------------- */
PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (!raiz) return criarPistaNode(pista);
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->direita = inserirPista(raiz->direita, pista);
    return raiz;
}

/* ---------------------------------------------------------
   Exibe BST de pistas em ordem alfabética
--------------------------------------------------------- */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        printf(" - %s (Suspeito: %s)\n", raiz->pista, encontrarSuspeito(raiz->pista));
        exibirPistas(raiz->direita);
    }
}

/* ---------------------------------------------------------
   Exploração interativa da mansão
--------------------------------------------------------- */
void explorarSalas(Sala* atual, PistaNode** arvorePistas) {
    char opcao;
    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        if (atual->pista != NULL) {
            printf("Você encontrou a pista: %s\n", atual->pista);
            *arvorePistas = inserirPista(*arvorePistas, atual->pista);
        }

        if (!atual->esquerda && !atual->direita) {
            printf("Não há mais caminhos nesta sala.\n");
            break;
        }

        printf("\nEscolha o próximo caminho:\n");
        printf(" (e) Esquerda\n");
        printf(" (d) Direita\n");
        printf(" (s) Sair\n");
        printf("Opção: ");
        scanf(" %c", &opcao);

        if (opcao == 'e') atual = atual->esquerda;
        else if (opcao == 'd') atual = atual->direita;
        else if (opcao == 's') break;
        else printf("Opção inválida! Tente novamente.\n");
    }
}

/* ---------------------------------------------------------
   Verificar se o suspeito tem evidências suficientes
--------------------------------------------------------- */
void verificarSuspeitoFinal(PistaNode* raiz, const char* suspeito, int* contador) {
    if (!raiz) return;
    char* suspeitoPista = encontrarSuspeito(raiz->pista);
    if (suspeitoPista && strcmp(suspeitoPista, suspeito) == 0) (*contador)++;
    verificarSuspeitoFinal(raiz->esquerda, suspeito, contador);
    verificarSuspeitoFinal(raiz->direita, suspeito, contador);
}

/* ---------------------------------------------------------
   Função principal
--------------------------------------------------------- */
int main() {
    PistaNode* arvorePistas = NULL;

    // Montagem da mansão
    Sala* hall = criarSala("Hall de Entrada", "Chave antiga");
    hall->esquerda = criarSala("Sala de Estar", "Rastro de pegadas");
    hall->direita = criarSala("Cozinha", "Garrafa quebrada");
    hall->esquerda->esquerda = criarSala("Biblioteca", "Livro fora do lugar");
    hall->esquerda->direita = criarSala("Jardim", "Pegadas na terra molhada");
    hall->direita->esquerda = criarSala("Despensa", "Cacos de vidro");
    hall->direita->direita = criarSala("Garagem", "Rastro de pneu");

    // Montagem da tabela hash de suspeitos
    inserirNaHash("Chave antiga", "Alice");
    inserirNaHash("Rastro de pegadas", "Bob");
    inserirNaHash("Garrafa quebrada", "Carol");
    inserirNaHash("Livro fora do lugar", "Bob");
    inserirNaHash("Pegadas na terra molhada", "Alice");
    inserirNaHash("Cacos de vidro", "Carol");
    inserirNaHash("Rastro de pneu", "Bob");

    printf("=== Bem-vindo ao Detective Quest - Nível Mestre ===\n");
    printf("Explore a mansão e colete pistas para acusar o culpado.\n");

    explorarSalas(hall, &arvorePistas);

    // Exibir pistas coletadas
    printf("\n=== Pistas coletadas ===\n");
    exibirPistas(arvorePistas);

    // Acusação final
    char suspeito[50];
    printf("\nDigite o nome do suspeito que você acusa: ");
    scanf(" %49[^\n]", suspeito);

    int contador = 0;
    verificarSuspeitoFinal(arvorePistas, suspeito, &contador);

    if (contador >= 2)
        printf("\nAcusação correta! Há %d pistas apontando para %s.\n", contador, suspeito);
    else
        printf("\nAcusação incorreta. Apenas %d pista(s) aponta(m) para %s.\n", contador, suspeito);

    return 0;
}
