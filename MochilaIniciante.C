#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ITENS 10

// ==============================
// ESTRUTURA DE DADOS
// ==============================
typedef struct {
    char nome[30];     // Nome do item (ex: "Pistola", "Kit Médico")
    char tipo[20];     // Tipo do item (ex: "Arma", "Cura", "Munição")
    int quantidade;    // Quantidade do item
} Item;

// ==============================
// DECLARAÇÃO DAS FUNÇÕES
// ==============================
void inserirItem(Item mochila[], int *contador);
void removerItem(Item mochila[], int *contador);
void listarItens(Item mochila[], int contador);
void buscarItem(Item mochila[], int contador);

// ==============================
// FUNÇÃO PRINCIPAL
// ==============================
int main() {
    Item mochila[MAX_ITENS];
    int contador = 0;
    int opcao;

    do {
        printf("\n======= MOCHILA DE LOOT =======\n");
        printf("1 - Cadastrar item\n");
        printf("2 - Remover item\n");
        printf("3 - Listar itens\n");
        printf("4 - Buscar item\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // limpar o buffer

        switch(opcao) {
            case 1:
                inserirItem(mochila, &contador);
                listarItens(mochila, contador);
                break;
            case 2:
                removerItem(mochila, &contador);
                listarItens(mochila, contador);
                break;
            case 3:
                listarItens(mochila, contador);
                break;
            case 4:
                buscarItem(mochila, contador);
                break;
            case 0:
                printf("\nSaindo do sistema...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
        }
    } while(opcao != 0);

    return 0;
}

// ==============================
// FUNÇÃO: INSERIR ITEM
// ==============================
void inserirItem(Item mochila[], int *contador) {
    if (*contador >= MAX_ITENS) {
        printf("\nA mochila esta cheia! Nao e possivel adicionar mais itens.\n");
        return;
    }

    Item novo;
    printf("\n--- CADASTRO DE ITEM ---\n");
    printf("Nome do item: ");
    fgets(novo.nome, sizeof(novo.nome), stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0'; // remover \n

    printf("Tipo do item (arma, municao, cura, etc): ");
    fgets(novo.tipo, sizeof(novo.tipo), stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    getchar(); // limpar buffer

    mochila[*contador] = novo;
    (*contador)++;

    printf("\nItem adicionado com sucesso!\n");
}

// ==============================
// FUNÇÃO: REMOVER ITEM
// ==============================
void removerItem(Item mochila[], int *contador) {
    if (*contador == 0) {
        printf("\nA mochila esta vazia! Nao ha itens para remover.\n");
        return;
    }

    char nomeRemover[30];
    printf("\n--- REMOCAO DE ITEM ---\n");
    printf("Digite o nome do item que deseja remover: ");
    fgets(nomeRemover, sizeof(nomeRemover), stdin);
    nomeRemover[strcspn(nomeRemover, "\n")] = '\0';

    int encontrado = 0;
    for (int i = 0; i < *contador; i++) {
        if (strcmp(mochila[i].nome, nomeRemover) == 0) {
            encontrado = 1;
            for (int j = i; j < *contador - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            (*contador)--;
            printf("\nItem '%s' removido com sucesso!\n", nomeRemover);
            break;
        }
    }

    if (!encontrado)
        printf("\nItem nao encontrado na mochila.\n");
}

// ==============================
// FUNÇÃO: LISTAR ITENS
// ==============================
void listarItens(Item mochila[], int contador) {
    if (contador == 0) {
        printf("\nA mochila esta vazia!\n");
        return;
    }

    printf("\n--- ITENS NA MOCHILA ---\n");
    for (int i = 0; i < contador; i++) {
        printf("%d. Nome: %s | Tipo: %s | Quantidade: %d\n",
               i + 1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
    }
}

// ==============================
// FUNÇÃO: BUSCAR ITEM (BUSCA SEQUENCIAL)
// ==============================
void buscarItem(Item mochila[], int contador) {
    if (contador == 0) {
        printf("\nA mochila esta vazia! Nao ha itens para buscar.\n");
        return;
    }

    char nomeBusca[30];
    printf("\n--- BUSCA DE ITEM ---\n");
    printf("Digite o nome do item: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    for (int i = 0; i < contador; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            printf("\nItem encontrado!\n");
            printf("Nome: %s\nTipo: %s\nQuantidade: %d\n",
                   mochila[i].nome, mochila[i].tipo, mochila[i].quantidade);
            return;
        }
    }

    printf("\nItem nao encontrado na mochila.\n");
}