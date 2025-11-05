#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ==============================
// DEFINIÇÕES E ESTRUTURAS
// ==============================
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// ==============================
// PROTÓTIPOS
// ==============================
void inserirItem(No **cabeca);
void removerItem(No **cabeca);
void listarItens(No *cabeca);
void buscarItem(No *cabeca);
void liberarLista(No *cabeca);
void limparBuffer();

// ==============================
// FUNÇÃO PRINCIPAL
// ==============================
int main() {
    No *cabeca = NULL;
    int opcao;

    do {
        printf("\n======= MOCHILA DE SOBREVIVENCIA =======\n");
        printf("1 - Adicionar Item (Loot)\n");
        printf("2 - Remover Item\n");
        printf("3 - Listar Itens\n");
        printf("4 - Buscar Item\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                inserirItem(&cabeca);
                listarItens(cabeca);
                break;
            case 2:
                removerItem(&cabeca);
                listarItens(cabeca);
                break;
            case 3:
                listarItens(cabeca);
                break;
            case 4:
                buscarItem(cabeca);
                break;
            case 0:
                printf("\nSaindo do sistema...\n");
                liberarLista(cabeca);
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}

// ==============================
// FUNÇÕES UTILITÁRIAS
// ==============================
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

// ==============================
// INSERIR ITEM
// ==============================
void inserirItem(No **cabeca) {
    Item novo;
    printf("\n--- CADASTRO DE ITEM ---\n");

    printf("Nome do item: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';

    printf("Tipo do item (arma, municao, cura, etc): ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    limparBuffer();

    No *novoNo = (No*) malloc(sizeof(No));
    if (novoNo == NULL) {
        printf("\nErro de alocacao de memoria!\n");
        return;
    }

    novoNo->dados = novo;
    novoNo->proximo = *cabeca;
    *cabeca = novoNo;

    printf("\nItem '%s' adicionado com sucesso!\n", novo.nome);
}

// ==============================
// REMOVER ITEM
// ==============================
void removerItem(No **cabeca) {
    if (*cabeca == NULL) {
        printf("\nA mochila esta vazia! Nao ha itens para remover.\n");
        return;
    }

    char nomeRemover[30];
    printf("\n--- REMOCAO DE ITEM ---\n");
    printf("Digite o nome do item que deseja remover: ");
    fgets(nomeRemover, sizeof(nomeRemover), stdin);
    nomeRemover[strcspn(nomeRemover, "\n")] = '\0';

    No *atual = *cabeca;
    No *anterior = NULL;

    while (atual != NULL && strcmp(atual->dados.nome, nomeRemover) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("\nItem '%s' nao encontrado na mochila.\n", nomeRemover);
        return;
    }

    if (anterior == NULL) {
        *cabeca = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    free(atual);
    printf("\nItem '%s' removido com sucesso!\n", nomeRemover);
}

// ==============================
// LISTAR ITENS
// ==============================
void listarItens(No *cabeca) {
    if (cabeca == NULL) {
        printf("\nA mochila esta vazia!\n");
        return;
    }

    printf("\n--- ITENS NA MOCHILA ---\n");
    int i = 1;
    No *atual = cabeca;
    while (atual != NULL) {
        printf("%d. Nome: %s | Tipo: %s | Quantidade: %d\n",
               i++, atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
        atual = atual->proximo;
    }
}

// ==============================
// BUSCAR ITEM
// ==============================
void buscarItem(No *cabeca) {
    if (cabeca == NULL) {
        printf("\nA mochila esta vazia! Nao ha itens para buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("\n--- BUSCA DE ITEM ---\n");
    printf("Digite o nome do item: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    No *atual = cabeca;
    while (atual != NULL) {
        if (strcmp(atual->dados.nome, nomeBusca) == 0) {
            printf("\nItem encontrado!\n");
            printf("Nome: %s\nTipo: %s\nQuantidade: %d\n",
                   atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
            return;
        }
        atual = atual->proximo;
    }

    printf("\nItem '%s' nao encontrado na mochila.\n", nomeBusca);
}

// ==============================
// LIBERAR MEMÓRIA
// ==============================
void liberarLista(No *cabeca) {
    No *atual = cabeca;
    No *proximo;

    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
}
