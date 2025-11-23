#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ---------------------------------------------------------
   Struct Sala
   Representa cada cômodo da mansão (nó da árvore binária)
   --------------------------------------------------------- */
typedef struct sala {
    char nome[50];
    struct sala* esquerda;
    struct sala* direita;
} Sala;

/* ---------------------------------------------------------
   criarSala()
   Cria dinamicamente uma nova sala com o nome fornecido.
   --------------------------------------------------------- */
Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (nova == NULL) {
        printf("Erro ao alocar memória para sala!\n");
        exit(1);
    }

    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

/* ---------------------------------------------------------
   explorarSalas()
   Permite ao jogador navegar pela mansão.
   O jogador escolhe ir para a esquerda (e) ou direita (d).
   A exploração termina quando chega a um nó-folha.
   --------------------------------------------------------- */
void explorarSalas(Sala* atual) {
    char opcao;

    printf("\n=== EXPLORAÇÃO DA MANSÃO ===\n");

    while (atual != NULL) {
        printf("\nVocê está em: %s\n", atual->nome);

        // Caso seja um nó-folha (sem caminhos)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Este cômodo não possui mais caminhos. Fim da exploração.\n");
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
        }
        else if (opcao == 'd') {
            atual = atual->direita;
        }
        else if (opcao == 's') {
            printf("Exploração encerrada pelo jogador.\n");
            break;
        }
        else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

/* ---------------------------------------------------------
   main()
   Monta manualmente o mapa da mansão usando criarSala().
   Depois inicia a exploração com explorarSalas().
   --------------------------------------------------------- */
int main() {
    /* Estrutura da mansão (exemplo):

                     Hall de Entrada
                      /            \
               Sala de Estar       Cozinha
                /        \          /      \
            Biblioteca   Jardim  Despensa  Garagem
    */

    // Criação dos nós principais
    Sala* hall = criarSala("Hall de Entrada");

    hall->esquerda = criarSala("Sala de Estar");
    hall->direita  = criarSala("Cozinha");

    hall->esquerda->esquerda = criarSala("Biblioteca");
    hall->esquerda->direita  = criarSala("Jardim");

    hall->direita->esquerda = criarSala("Despensa");
    hall->direita->direita  = criarSala("Garagem");

    // Inicia a exploração da mansão
    explorarSalas(hall);

    return 0;
}
