// Implementação em C de árvore binária
#include <stdio.h>
#include <stdlib.h>

typedef struct BTreeNode_T BTreeNode;

// Nó da árvore
typedef struct BTreeNode_T
{
    int *keys;          // chaves do nó
    int t;              // Grau mínimo
    BTreeNode **filhos; // Ponteiros para filhos
    int n;              // Numero de chaves
    int leaf;           // verificação de folha
} BTreeNode;

// Arvore B
typedef struct BTree
{
    BTreeNode *root; // ponteiro para a raiz
    int t;           // grau minimo
} BTree;

//função que aloca um nó da árvore B
BTreeNode *init_btree_node(int t, int leaf)
{
    BTreeNode *node = malloc(sizeof(BTreeNode));
    node->t = t;
    node->leaf = leaf;

    node->keys = malloc(sizeof(int) * ((2 * t) - 1));
    node->filhos = malloc(sizeof(BTreeNode *) * (2 * t));
    node->n = 0;

    return node;
}

//função que cria uma árvore B
BTree *cria_arvore_b(BTree *T, int t)
{
    BTreeNode *novo = init_btree_node(t, 1);
    novo->leaf = 1;
    novo->n = 0;
    T->root = novo;

    return T;
}

//recebe como entrada um nó x não cheio e um índice i, tal que x->filho[i] é um nó filho cheio de x
void divide_filho(BTreeNode *x, int i, int t)
{
    BTreeNode *new_2_metade = init_btree_node(t, 1);
    BTreeNode *new_1_metade = x->filhos[i];

    printf("n = %d ; t = %d\n", new_1_metade->n, t);

    new_2_metade->leaf = new_1_metade->leaf;
    new_2_metade->n = t - 1;

    for (int j = 1; j < t - 1; j++)
        new_2_metade->keys[j] = new_1_metade->keys[j + t];

    if (!new_1_metade->leaf)
    {
        for (int j = 1; j < t; j++)
            new_2_metade->filhos[j] = new_1_metade->filhos[j + t];
    }
    new_1_metade->n = t - 1;

    for (int j = x->n + 1; j > i + 1; j--)
        x->filhos[j + 1] = x->filhos[j];

    x->filhos[i + 1] = new_2_metade;

    for (int j = x->n; j > i; j--)
        x->keys[j + 1] = x->keys[j];

    x->keys[i] = new_1_metade->keys[t];
    x->n = x->n + 1;
}

void insere_arvore_b_nao_cheio(BTreeNode *x, int k, int t)
{
    int i = x->n;
    if (x->leaf)
    {
        while (i >= 1 && k < x->keys[i])
        {
            x->keys[i + 1] = x->keys[i];
            i--;
        }
        x->n++;
        x->keys[i + 1] = k;
    }
    else
    {

        while (i >= 1 && k < x->keys[i])
            i--;
        i++;
        if (x->filhos[i]->n == (2 * t) - 1)
        {
            divide_filho(x, i, t);
            if (k > x->keys[i])
                i++;
        }
        insere_arvore_b_nao_cheio(x->filhos[i], k, t);
    }
}

void insere_arvore_b(BTree *T, int k, int t)
{
    BTreeNode *r = T->root;

    //se estiver cheio
    if (r->n == (2 * t) - 1)
    {
        BTreeNode *s = init_btree_node(t, 0);
        T->root = s;
        s->leaf = 0;
        s->n = 0;
        s->filhos[1] = r;
        divide_filho(s, 1, t);
        insere_arvore_b_nao_cheio(s, k, t);
    }
    else
    {
        insere_arvore_b_nao_cheio(r, k, t);
    }
}

void emOrdem(BTreeNode *raiz)
{
    if (raiz == NULL)
        return;

    for (int i = 0; i < raiz->n; i++)
    {
        emOrdem(raiz->filhos[i]);
        printf("%i ", raiz->keys[i]);
    }
    emOrdem(raiz->filhos[raiz->n]);
}

//aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
//bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb
//ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc

int main()
{

    int t; // grau da árvore B
    printf("Digite o grau da arvore B desejado: ");
    scanf("%d", &t);

    BTree btree_aux;
    BTree *btree = cria_arvore_b(&btree_aux, t);

    insere_arvore_b(btree, 4, t);

    insere_arvore_b(btree, 3, t);
    insere_arvore_b(btree, 1, t);
    insere_arvore_b(btree, 2, t);
    insere_arvore_b(btree, 5, t);
    insere_arvore_b(btree, 1, t);
    insere_arvore_b(btree, 2, t);
    insere_arvore_b(btree, 5, t);

    emOrdem(btree->root);
}
