#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_COMPONENTES 20
#define MAX_NOME 30
#define MAX_TIPO 20

typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int prioridade; // 1..10
} Componente;

/* ---------- Protótipos (pedidos nas instruções) ---------- */
void selectionSortPrioridade(Componente arr[], int n, long *comparacoes);
void insertionSortTipo(Componente arr[], int n, long *comparacoes);
void bubbleSortNome(Componente arr[], int n, long *comparacoes);
int buscaBinariaPorNome(Componente arr[], int n, const char chave[], long *comparacoes); // retorna índice ou -1
void mostrarComponentes(Componente arr[], int n);
double medirTempo(void (*algoritmo)(Componente[], int, long*), Componente arr[], int n, long *comparacoes);

/* ---------- Funções utilitárias ---------- */
void limparNovoline(char *s) {
    s[strcspn(s, "\n")] = '\0';
}

void trocar(Componente *a, Componente *b) {
    Componente tmp = *a;
    *a = *b;
    *b = tmp;
}

/* ---------- Implementações dos algoritmos ---------- */

/*
 Bubble Sort por nome (alfabético)
 Conta cada strcmp como 1 comparação.
 */
void bubbleSortNome(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    if (n <= 1) return;
    int trocou;
    for (int i = 0; i < n - 1; ++i) {
        trocou = 0;
        for (int j = 0; j < n - 1 - i; ++j) {
            (*comparacoes)++; // comparação entre arr[j].nome e arr[j+1].nome
            if (strcmp(arr[j].nome, arr[j+1].nome) > 0) {
                trocar(&arr[j], &arr[j+1]);
                trocou = 1;
            }
        }
        if (!trocou) break;
    }
}

/*
 Insertion Sort por tipo (string)
 Conta cada comparação strcmp como 1 comparação.
 */
void insertionSortTipo(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < n; ++i) {
        Componente chave = arr[i];
        int j = i - 1;
        while (j >= 0) {
            (*comparacoes)++; // comparação entre arr[j].tipo e chave.tipo
            if (strcmp(arr[j].tipo, chave.tipo) > 0) {
                arr[j+1] = arr[j];
                j--;
            } else {
                break;
            }
        }
        arr[j+1] = chave;
    }
}

/*
 Selection Sort por prioridade (int) - maior prioridade numérica vem antes? 
 Aqui assumimos: prioridade maior = mais importante. Vamos ordenar DECRESCENTE por prioridade.
 Conta cada comparação de int como 1 comparação.
 */
void selectionSortPrioridade(Componente arr[], int n, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < n - 1; ++i) {
        int idxMax = i;
        for (int j = i + 1; j < n; ++j) {
            (*comparacoes)++; // comparação de prioridades
            if (arr[j].prioridade > arr[idxMax].prioridade) {
                idxMax = j;
            }
        }
        if (idxMax != i) {
            trocar(&arr[i], &arr[idxMax]);
        }
    }
}

/*
 Busca binária por nome (aplicável apenas se o vetor estiver ordenado por nome crescente).
 Retorna índice encontrado ou -1. Conta comparações de strcmp.
 */
int buscaBinariaPorNome(Componente arr[], int n, const char chave[], long *comparacoes) {
    *comparacoes = 0;
    int l = 0, r = n - 1;
    while (l <= r) {
        int m = l + (r - l) / 2;
        (*comparacoes)++; // comparação strcmp entre arr[m].nome e chave
        int cmp = strcmp(arr[m].nome, chave);
        if (cmp == 0) return m;
        if (cmp < 0) l = m + 1;
        else r = m - 1;
    }
    return -1;
}

/* Medir tempo de execução de um algoritmo de ordenação.
   Recebe função do tipo void alg(Componente[], int, long*)
   Retorna tempo em segundos e preenche *comparacoes. */
double medirTempo(void (*algoritmo)(Componente[], int, long*), Componente arr[], int n, long *comparacoes) {
    clock_t inicio = clock();
    algoritmo(arr, n, comparacoes);
    clock_t fim = clock();
    double secs = (double)(fim - inicio) / (double)CLOCKS_PER_SEC;
    return secs;
}

/* Mostrar componentes formatado */
void mostrarComponentes(Componente arr[], int n) {
    if (n <= 0) {
        printf("\n[Nenhum componente cadastrado]\n");
        return;
    }
    printf("\n--- COMPONENTES (total: %d) ---\n", n);
    printf("%-3s | %-28s | %-18s | %-10s\n", "#", "NOME", "TIPO", "PRIORIDADE");
    printf("----+------------------------------+--------------------+-----------\n");
    for (int i = 0; i < n; ++i) {
        printf("%3d | %-28s | %-18s | %-10d\n", i+1, arr[i].nome, arr[i].tipo, arr[i].prioridade);
    }
}

/* ---------- Função principal / Menu ---------- */
int main() {
    Componente componentes[MAX_COMPONENTES];
    int n = 0; // número atual de componentes

    int opcao;
    int ultimoOrdenadoPor = 0; // 0 = nenhum, 1 = nome, 2 = tipo, 3 = prioridade

    printf("=== MÓDULO: MONTAGEM DA TORRE DE FUGA ===\n");

    do {
        printf("\nMenu principal:\n");
        printf("1 - Cadastrar componente (até %d)\n", MAX_COMPONENTES);
        printf("2 - Listar componentes\n");
        printf("3 - Ordenar componentes (escolher algoritmo)\n");
        printf("4 - Busca binária por nome (apenas após ordenar por nome)\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        if (scanf("%d", &opcao) != 1) { // proteção contra entrada inválida
            printf("Entrada invalida. Tente novamente.\n");
            // limpar stdin
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }
        int c; while ((c = getchar()) != '\n' && c != EOF) {} // limpar buffer

        if (opcao == 1) {
            if (n >= MAX_COMPONENTES) {
                printf("\nLimite de componentes atingido (%d).\n", MAX_COMPONENTES);
            } else {
                Componente tmp;
                printf("\n--- CADASTRO DE COMPONENTE (%d/%d) ---\n", n+1, MAX_COMPONENTES);
                printf("Nome (ex: chip central): ");
                fgets(tmp.nome, sizeof(tmp.nome), stdin);
                limparNovoline(tmp.nome);

                printf("Tipo (ex: controle, suporte, propulsao): ");
                fgets(tmp.tipo, sizeof(tmp.tipo), stdin);
                limparNovoline(tmp.tipo);

                int pr;
                do {
                    printf("Prioridade (1 a 10, 10 = mais importante): ");
                    if (scanf("%d", &pr) != 1) {
                        printf("Valor invalido. Tente novamente.\n");
                        int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
                        pr = -1;
                    } else {
                        int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
                        if (pr < 1 || pr > 10) {
                            printf("Prioridade deve estar entre 1 e 10.\n");
                        }
                    }
                } while (pr < 1 || pr > 10);
                tmp.prioridade = pr;

                componentes[n++] = tmp;
                ultimoOrdenadoPor = 0; // após inserir, ordem anterior pode não ser válida
                printf("\nComponente '%s' cadastrado com sucesso.\n", tmp.nome);
            }
        } else if (opcao == 2) {
            mostrarComponentes(componentes, n);
        } else if (opcao == 3) {
            if (n == 0) {
                printf("\nNao ha componentes para ordenar.\n");
                continue;
            }
            printf("\nEscolha o algoritmo de ordenacao:\n");
            printf("1 - Bubble Sort (ordenar por NOME, crescente)\n");
            printf("2 - Insertion Sort (ordenar por TIPO, crescente)\n");
            printf("3 - Selection Sort (ordenar por PRIORIDADE, decrescente)\n");
            printf("Escolha: ");
            int escolha;
            if (scanf("%d", &escolha) != 1) {
                printf("Entrada invalida.\n");
                int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
                continue;
            }
            int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
            long comparacoes = 0;
            double tempo = 0.0;
            // Ordenamos diretamente o vetor componentes
            if (escolha == 1) {
                tempo = medirTempo(bubbleSortNome, componentes, n, &comparacoes);
                ultimoOrdenadoPor = 1;
                printf("\n--- Resultado: Bubble Sort por NOME ---\n");
                printf("Comparacoes (strcmp) = %ld\n", comparacoes);
                printf("Tempo de execucao = %.6f segundos\n", tempo);
                mostrarComponentes(componentes, n);
            } else if (escolha == 2) {
                tempo = medirTempo(insertionSortTipo, componentes, n, &comparacoes);
                ultimoOrdenadoPor = 2;
                printf("\n--- Resultado: Insertion Sort por TIPO ---\n");
                printf("Comparacoes (strcmp) = %ld\n", comparacoes);
                printf("Tempo de execucao = %.6f segundos\n", tempo);
                mostrarComponentes(componentes, n);
            } else if (escolha == 3) {
                tempo = medirTempo(selectionSortPrioridade, componentes, n, &comparacoes);
                ultimoOrdenadoPor = 3;
                printf("\n--- Resultado: Selection Sort por PRIORIDADE (decrescente) ---\n");
                printf("Comparacoes (int) = %ld\n", comparacoes);
                printf("Tempo de execucao = %.6f segundos\n", tempo);
                mostrarComponentes(componentes, n);
            } else {
                printf("Opcao invalida.\n");
            }
        } else if (opcao == 4) {
            if (n == 0) {
                printf("\nNao ha componentes cadastrados.\n");
                continue;
            }
            if (ultimoOrdenadoPor != 1) {
                printf("\nA busca binaria so e permitida apos ordenar por NOME (Bubble Sort).\n");
                printf("Atualmente o vetor esta ordenado por: ");
                if (ultimoOrdenadoPor == 0) printf("nenhum/sem garantia de ordenacao.\n");
                else if (ultimoOrdenadoPor == 2) printf("TIPO (insertion sort).\n");
                else if (ultimoOrdenadoPor == 3) printf("PRIORIDADE (selection sort).\n");
                printf("Ordene por NOME primeiro (menu -> ordenar -> bubble) e tente novamente.\n");
                continue;
            }
            char chave[MAX_NOME];
            printf("\nDigite o NOME do componente-chave a buscar: ");
            fgets(chave, sizeof(chave), stdin);
            limparNovoline(chave);

            long compBusca = 0;
            int idx = buscaBinariaPorNome(componentes, n, chave, &compBusca);
            printf("\n--- Resultado da Busca Binaria ---\n");
            printf("Comparacoes (strcmp) durante a busca = %ld\n", compBusca);
            if (idx >= 0) {
                printf("Componente encontrado no indice (0-based) %d:\n", idx);
                printf("Nome: %s | Tipo: %s | Prioridade: %d\n",
                       componentes[idx].nome, componentes[idx].tipo, componentes[idx].prioridade);
                printf("\n>>> Componente-chave presente: desbloqueio POSSIVEL. (simulacao)\n");
            } else {
                printf("Componente '%s' nao encontrado.\n", chave);
                printf("\n>>> Componente-chave ausente: nao e possivel ativar a torre.\n");
            }
        } else if (opcao == 0) {
            printf("\nSaindo... boa sorte na fuga!\n");
        } else {
            printf("\nOpcao invalida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}