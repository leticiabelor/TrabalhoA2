#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Usuario {
    char nome[100];
    int id;
    char email[100];
    int altura;
    struct Usuario* esq;
    struct Usuario* dir;
    
} Usuario;

int altura(Usuario* no) {
    return no ? no->altura : 0;
    
}

int max(int a, int b) {
    return (a > b) ? a : b;
    
}

int fatorBalanceamento(Usuario* no) {
    return no ? altura(no->esq) - altura(no->dir) : 0;
    
}

Usuario* rotacaoDireita(Usuario* y) {
    Usuario* x = y->esq;
    Usuario* T2 = x->dir;

    x->dir = y;
    y->esq = T2;

    y->altura = 1 + max(altura(y->esq), altura(y->dir));
    x->altura = 1 + max(altura(x->esq), altura(x->dir));

    return x;
    
}

Usuario* rotacaoEsquerda(Usuario* x) {
    Usuario* y = x->dir;
    Usuario* T2 = y->esq;

    y->esq = x;
    x->dir = T2;

    x->altura = 1 + max(altura(x->esq), altura(x->dir));
    y->altura = 1 + max(altura(y->esq), altura(y->dir));

    return y;
    
}

Usuario* inserir(Usuario* raiz, char nome[], int id, char email[]) {
    if (!raiz) {
        Usuario* novo = (Usuario*)malloc(sizeof(Usuario));
        strcpy(novo->nome, nome);
        novo->id = id;
        strcpy(novo->email, email);
        novo->esq = novo->dir = NULL;
        novo->altura = 1;
        return novo;
        
    }

    if (strcmp(nome, raiz->nome) < 0)
        raiz->esq = inserir(raiz->esq, nome, id, email);
    else if (strcmp(nome, raiz->nome) > 0)
        raiz->dir = inserir(raiz->dir, nome, id, email);
    else {
        printf("Usuário com esse nome já existe.\n");
        return raiz;
        
    }

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));

    int fb = fatorBalanceamento(raiz);

    if (fb > 1 && strcmp(nome, raiz->esq->nome) < 0)
        return rotacaoDireita(raiz);

    if (fb < -1 && strcmp(nome, raiz->dir->nome) > 0)
        return rotacaoEsquerda(raiz);

    if (fb > 1 && strcmp(nome, raiz->esq->nome) > 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
        
    }

    if (fb < -1 && strcmp(nome, raiz->dir->nome) < 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
        
    }

    return raiz;
    
}

Usuario* buscar(Usuario* raiz, char nome[]) {
    if (!raiz)
        return NULL;
        
    if (strcmp(nome, raiz->nome) == 0)
        return raiz;
        
    if (strcmp(nome, raiz->nome) < 0)
        return buscar(raiz->esq, nome);
        
    return buscar(raiz->dir, nome);
    
}

Usuario* menorValor(Usuario* no) {
    Usuario* atual = no;
    while (atual->esq)
        atual = atual->esq;
    return atual;
    
}

Usuario* remover(Usuario* raiz, char nome[]) {
    if (!raiz)
        return NULL;

    if (strcmp(nome, raiz->nome) < 0)
        raiz->esq = remover(raiz->esq, nome);
        
    else if (strcmp(nome, raiz->nome) > 0)
        raiz->dir = remover(raiz->dir, nome);
        
    else {
        if (!raiz->esq || !raiz->dir) {
            Usuario* temp = raiz->esq ? raiz->esq : raiz->dir;
            free(raiz);
            return temp;
            
        } else {
            Usuario* temp = menorValor(raiz->dir);
            strcpy(raiz->nome, temp->nome);
            raiz->id = temp->id;
            strcpy(raiz->email, temp->email);
            raiz->dir = remover(raiz->dir, temp->nome);
            
        }
        
    }

    raiz->altura = 1 + max(altura(raiz->esq), altura(raiz->dir));

    int fb = fatorBalanceamento(raiz);

    if (fb > 1 && fatorBalanceamento(raiz->esq) >= 0)
        return rotacaoDireita(raiz);

    if (fb > 1 && fatorBalanceamento(raiz->esq) < 0) {
        raiz->esq = rotacaoEsquerda(raiz->esq);
        return rotacaoDireita(raiz);
        
    }

    if (fb < -1 && fatorBalanceamento(raiz->dir) <= 0)
        return rotacaoEsquerda(raiz);

    if (fb < -1 && fatorBalanceamento(raiz->dir) > 0) {
        raiz->dir = rotacaoDireita(raiz->dir);
        return rotacaoEsquerda(raiz);
        
    }

    return raiz;
    
}

void listar(Usuario* raiz) {
    if (raiz) {
        listar(raiz->esq);
        printf("Nome: %s | ID: %d | Email: %s\n", raiz->nome, raiz->id, raiz->email);
        listar(raiz->dir);
        
    }
    
}

void liberarArvore(Usuario* raiz) {
    if (raiz) {
        liberarArvore(raiz->esq);
        liberarArvore(raiz->dir);
        free(raiz);
        
    }
    
}

void menu() {
    printf("\nSistema de Gerenciamento de Usuários (AVL)\n");
    printf("1. Cadastrar usuário\n");
    printf("2. Remover usuário\n");
    printf("3. Buscar usuário\n");
    printf("4. Listar usuários\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
    
}

int main() {
    Usuario* raiz = NULL;
    int opcao;
    char nome[100], email[100];
    int id;

    do {
        menu();
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                printf("Nome: ");
                fgets(nome, 100, stdin); nome[strcspn(nome, "\n")] = '\0';
                printf("ID: ");
                scanf("%d", &id); getchar();
                printf("Email: ");
                fgets(email, 100, stdin); email[strcspn(email, "\n")] = '\0';
                raiz = inserir(raiz, nome, id, email);
                break;

            case 2:
                printf("Nome do usuário a remover: ");
                fgets(nome, 100, stdin); nome[strcspn(nome, "\n")] = '\0';
                raiz = remover(raiz, nome);
                break;

            case 3:
                printf("Nome do usuário a buscar: ");
                fgets(nome, 100, stdin); nome[strcspn(nome, "\n")] = '\0';
                Usuario* u = buscar(raiz, nome);
                if (u)
                    printf("Usuário encontrado: ID: %d | Nome: %s | Email: %s\n", u->id, u->nome, u->email);
                else
                    printf("Usuário não encontrado.\n");
                break;

            case 4:
                printf("Lista de usuários:\n");
                listar(raiz);
                break;

            case 0:
                printf("Saindo...\n");
                break;

            default:
                printf("Opção inválida!\n");
                
        }
        
    } while (opcao != 0);

    liberarArvore(raiz);
    return 0;
    
}

