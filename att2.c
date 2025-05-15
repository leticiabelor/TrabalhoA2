#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { RED, BLACK } Color;

typedef struct Produto {
    int codigo;
    char nome[100];
    int quantidade;
    float preco;
    Color cor;
    struct Produto *esq, *dir, *pai;
} Produto;

Produto* raiz = NULL;

// Função para limpar o buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Funções auxiliares
Produto* criarProduto(int codigo, const char* nome, int quantidade, float preco) {
    Produto* novo = (Produto*)malloc(sizeof(Produto));
    novo->codigo = codigo;
    strcpy(novo->nome, nome);
    novo->quantidade = quantidade;
    novo->preco = preco;
    novo->cor = RED;
    novo->esq = novo->dir = novo->pai = NULL;
    return novo;
}

Produto* rotacaoEsquerda(Produto* raiz, Produto* x) {
    Produto* y = x->dir;
    x->dir = y->esq;
    if (y->esq) y->esq->pai = x;
    y->pai = x->pai;
    if (!x->pai) raiz = y;
    else if (x == x->pai->esq) x->pai->esq = y;
    else x->pai->dir = y;
    y->esq = x;
    x->pai = y;
    return raiz;
}

Produto* rotacaoDireita(Produto* raiz, Produto* x) {
    Produto* y = x->esq;
    x->esq = y->dir;
    if (y->dir) y->dir->pai = x;
    y->pai = x->pai;
    if (!x->pai) raiz = y;
    else if (x == x->pai->dir) x->pai->dir = y;
    else x->pai->esq = y;
    y->dir = x;
    x->pai = y;
    return raiz;
}

Produto* ajustarInsercao(Produto* raiz, Produto* z) {
    while (z->pai && z->pai->cor == RED) {
        Produto* avo = z->pai->pai;
        if (z->pai == avo->esq) {
            Produto* tio = avo->dir;
            if (tio && tio->cor == RED) {
                z->pai->cor = BLACK;
                tio->cor = BLACK;
                avo->cor = RED;
                z = avo;
            } else {
                if (z == z->pai->dir) {
                    z = z->pai;
                    raiz = rotacaoEsquerda(raiz, z);
                }
                z->pai->cor = BLACK;
                avo->cor = RED;
                raiz = rotacaoDireita(raiz, avo);
            }
        } else {
            Produto* tio = avo->esq;
            if (tio && tio->cor == RED) {
                z->pai->cor = BLACK;
                tio->cor = BLACK;
                avo->cor = RED;
                z = avo;
            } else {
                if (z == z->pai->esq) {
                    z = z->pai;
                    raiz = rotacaoDireita(raiz, z);
                }
                z->pai->cor = BLACK;
                avo->cor = RED;
                raiz = rotacaoEsquerda(raiz, avo);
            }
        }
    }
    raiz->cor = BLACK;
    return raiz;
}

Produto* inserir(Produto* raiz, Produto* novo) {
    Produto* y = NULL;
    Produto* x = raiz;

    while (x) {
        y = x;
        if (novo->codigo < x->codigo)
            x = x->esq;
        else if (novo->codigo > x->codigo)
            x = x->dir;
        else {
            printf("Produto com código %d já existe!\n", novo->codigo);
            free(novo);
            return raiz;
        }
    }

    novo->pai = y;
    if (!y)
        raiz = novo;
    else if (novo->codigo < y->codigo)
        y->esq = novo;
    else
        y->dir = novo;

    return ajustarInsercao(raiz, novo);
}

Produto* buscar(Produto* raiz, int codigo) {
    if (!raiz || raiz->codigo == codigo)
        return raiz;
    if (codigo < raiz->codigo)
        return buscar(raiz->esq, codigo);
    else
        return buscar(raiz->dir, codigo);
}

void listarProdutos(Produto* raiz) {
    if (raiz) {
        listarProdutos(raiz->esq);
        printf("Código: %d | Nome: %s | Qtd: %d | Preço: R$%.2f | Cor: %s\n",
               raiz->codigo, raiz->nome, raiz->quantidade,
               raiz->preco, raiz->cor == RED ? "Vermelho" : "Preto");
        listarProdutos(raiz->dir);
    }
}

// Função principal de menu
int main() {
    int opcao;
    do {
        printf("\n===== MENU INVENTÁRIO =====\n");
        printf("1. Cadastrar Produto\n");
        printf("2. Buscar Produto\n");
        printf("3. Listar Produtos\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Entrada inválida! Tente novamente.\n");
            limparBuffer();
            continue;
        }
        limparBuffer();

        if (opcao == 1) {
            int codigo, quantidade;
            float preco;
            char nome[100];
            printf("Código do Produto: ");
            scanf("%d", &codigo); limparBuffer();
            printf("Nome do Produto: ");
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = 0;
            printf("Quantidade: ");
            scanf("%d", &quantidade); limparBuffer();
            printf("Preço: ");
            scanf("%f", &preco); limparBuffer();
            Produto* novo = criarProduto(codigo, nome, quantidade, preco);
            raiz = inserir(raiz, novo);
            printf("Produto cadastrado com sucesso!\n");

        } else if (opcao == 2) {
            int codigo;
            printf("Código do Produto para busca: ");
            scanf("%d", &codigo); limparBuffer();
            Produto* prod = buscar(raiz, codigo);
            if (prod) {
                printf("Produto encontrado:\n");
                printf("Nome: %s | Quantidade: %d | Preço: R$%.2f | Cor: %s\n",
                       prod->nome, prod->quantidade, prod->preco,
                       prod->cor == RED ? "Vermelho" : "Preto");
            } else {
                printf("Produto não encontrado.\n");
            }

        } else if (opcao == 3) {
            printf("\nLista de Produtos:\n");
            listarProdutos(raiz);
        }

    } while (opcao != 0);

    return 0;
}
