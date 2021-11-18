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

//função que cria uma árvore B com um noh vazio como raiz
BTree *cria_arvore_b(BTree *T, int t)
{
    BTreeNode *novo = init_btree_node(t, 1);
    novo->leaf = 1;
    novo->n = 0;
    T->root = novo;

    return T;
}

//funcao que printa as keys de um noh
void printa_noh(BTreeNode *noh)
{
    for (int i = 0; i < noh->n; i++)
    {
        printf("%d ", noh->keys[i]);
    }
    printf("\n");
}

//recebe como entrada um nó x não cheio e um índice i, tal que x->filho[i] é um nó filho cheio de x
void divide_filho(BTreeNode *x, int i, int t)
{
    //os dois nohs metade
    BTreeNode *new_2_metade = init_btree_node(t, 1);
    BTreeNode *new_1_metade = x->filhos[i];

    //se for folha a metade também será folha
    new_2_metade->leaf = new_1_metade->leaf;
    //tamanha da segunda metade
    new_2_metade->n = t - 1;

    //colocando as keys da 2 metade
    for (int j = 0; j < t - 1; j++)
        new_2_metade->keys[j] = new_1_metade->keys[j + t];

    //se nao for folha arruma os filhos
    if (!new_1_metade->leaf)
    {
        for (int j = 0; j < t; j++)
            new_2_metade->filhos[j] = new_1_metade->filhos[j + t];
    }
    //arruma tamanho da primeira metade
    new_1_metade->n = t - 1;

    //arruma filhos do pai para inserir a nova metade (new_2_metade)
    for (int j = x->n + 1; j > i + 1; j--)
        x->filhos[j + 1] = x->filhos[j];

    //coloca a nova metade na posicao certa
    x->filhos[i + 1] = new_2_metade;

    //arruma as keys do noh pai
    for (int j = x->n; j > i; j--)
        x->keys[j + 1] = x->keys[j];

    //insere a nova key no pai, que era a mediana do filho antes cheio
    x->keys[i] = new_1_metade->keys[t - 1];
    //aumenta o n do pai
    x->n = x->n + 1;

    //mostra as coisas
    printf("Noh pai: ");
    printa_noh(x);
    printf("Filho esquerdo: ");
    printa_noh(new_1_metade);
    printf("Filho direito: ");
    printa_noh(new_2_metade);
}

void insere_arvore_b_nao_cheio(BTreeNode *x, int k, int t)
{
    int i = x->n;

    //caso seja folha
    if (x->leaf)
    {
        //vai shiffitando as keys para direita até achar a posicao certa
        while (i >= 0 && (k < x->keys[i] || x->keys[i] == 0))
        {
            x->keys[i + 1] = x->keys[i];
            i--;
        }
        //insere na posicao certa
        x->keys[i + 1] = k;
        //aumenta o n do noh
        x->n++;
    }
    //se nao for folha
    else
    {
        //procura a key para achar o filho que deve descer
        while (i >= 0 && (k < x->keys[i] || x->keys[i] == 0))
            i--;
        i++;
        //se o filho estiver cheio, divide ele
        if (x->filhos[i]->n == (2 * t) - 1)
        {
            divide_filho(x, i, t);
            if (k > x->keys[i])
                i++;
        }
        //insere em um filho nao cheio
        insere_arvore_b_nao_cheio(x->filhos[i], k, t);
    }
}

//função que insere uma nova key na arvore
void insere_arvore_b(BTree *T, int k, int t)
{
    //pegando a raiz da arvore
    BTreeNode *r = T->root;

    //se estiver cheio, tem que dividir o noh
    if (r->n == (2 * t) - 1)
    {
        BTreeNode *s = init_btree_node(t, 0);
        T->root = s;
        s->leaf = 0;
        s->n = 0;
        s->filhos[0] = r;
        divide_filho(s, 0, t);
        //insere noh novo
        insere_arvore_b_nao_cheio(s, k, t);
    }
    else
    {
        //insere no noh nao cheio
        insere_arvore_b_nao_cheio(r, k, t);
    }
}

//função que printa todas as keys em ordem -> busca em profundida
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

//função que busca uma key na arvore
BTreeNode *Busca_Key(BTreeNode *x, int k, int *p_posicao)
{
    //procurando a key que deve entrar
    int i = 0;
    while (i <= x->n && (k > x->keys[i] && x->keys[i] != 0))
        i++;

    //se achar
    if (i <= x->n && k == x->keys[i])
    {
        *p_posicao = i;
        printf("Entrou!\n");
        return x;
    }
    //se nao achar e for folha
    else if (x->leaf)
        return NULL;
    //se nao achar, mas nao é folha, continua procurando no filho
    return Busca_Key(x->filhos[i], k, p_posicao);
}

int main()
{

    int t, k; // grau da árvore B
    printf("Digite o grau da arvore B desejado: ");
    scanf("%d", &t);
    while (t < 2)
    {
        printf("Grau deve ser maior ou igual a 2!\n");
        printf("Digite o grau da arvore B desejado: ");
        scanf("%d", &t);
    }

    BTree btree_aux;
    BTree *btree = cria_arvore_b(&btree_aux, t);

    printf("Digite 0 para parar ou uma key: ");
    scanf("%d", &k);
    while (k > 0)
    {
        insere_arvore_b(btree, k, t);
        emOrdem(btree->root);
        printf("\n");
        printf("Digite 0 para parar ou uma key: ");
        scanf("%d", &k);
    }

    int procurada, posicao;
    int *p_posicao = &posicao;

    printf("Buscar key: ");
    scanf("%d", &procurada);

    BTreeNode *noh_encontrado = Busca_Key(btree->root, procurada, p_posicao);

    if (noh_encontrado != NULL)
    {
        printf("Noh Encontrado!\n");

        printf("Key: %d na posição %d\n", noh_encontrado->keys[*p_posicao], *p_posicao);
        printf("Conteúdo do noh encontrado:\n");
        printa_noh(noh_encontrado);
    }
    else
    {
        printf("CHAVE NÃO ENCONTRADA!\n");
    }
}
