// Implementação em C de árvore binária
#include <stdio.h>
#define DEGREE 10 // Aqui entra o tamanho do sistema de arquivos dividido pelo tamanho da entrada

typedef struct BTreeNode_T BTreeNode;
void traverse_node(BTreeNode* btree_node);

// Nó da árvore
typedef struct BTreeNode_T
{
    int* keys; 
    int t;      // Minimum degree (defines the range for number of keys)
    BTreeNode** C; // Ponteiros para childs
    int n;     // Numero de folhas
    int leaf; // verificação de folha
} BTreeNode;

//printando em profundidade - DFS
void traverse_node(BTreeNode* btree_node) {

    int i;
    for (i = 0; i < btree_node->n; i++)
    {
        if (btree_node->leaf)
            traverse_node(btree_node->C[i]);
        printf("%d\n", btree_node->keys[i]);
    }
 
    if (btree_node->leaf == 0)
    traverse_node(btree_node->C[i]);

}

// BTree
typedef struct BTree
{
    BTreeNode *root; // ponteiro para a raiz
    int t;  // grau minimo
} BTree;

BTree init_btree() {
    BTree new_btree;
    new_btree.root = NULL;
    new_btree.t = DEGREE;
    return new_btree;
}

void traverse_btree(BTree* btree) {
    if (btree->root != NULL) traverse_node(btree->root); 
}


BTreeNode* search_btree_node(BTreeNode *node, int k)
{
    int i = 0;
    while (i < node->n && k > node->keys[i])
        i++;
 
    if (node->keys[i] == k)
        return node;
 
    if (node->leaf == 1)
        return NULL;
 
    return search_btree_node(node->C[i], k);
}

void search_tree(BTree* btree, int k) {
    return (btree->root == NULL)? NULL : search_btree_node(btree->root, k);
}
 
BTreeNode init_btree_node(int t, int leaf)
{
    BTreeNode node;
    node.t = t;
    node.leaf = leaf;
 
    node.keys = malloc(sizeof(int)*(2*t-1));
    node.C = malloc(sizeof(BTreeNode*)*(2*t));
    node.n = 0;

    return node;
}

int main() {
    
    BTree btree = init_btree();
    BTreeNode node = init_btree_node(DEGREE, 0);
    btree.root = &node;
    btree.root->C[0] = 20;

    printf("%d\n", btree.root->C[0]);

}
