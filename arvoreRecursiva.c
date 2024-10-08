#include <stdio.h>
#include <stdlib.h>

// Definição do nó da árvore binária
struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

// Função para criar um novo nó
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Função recursiva para inserir um novo nó na árvore
struct Node* insertRecursive(struct Node* root, int data) {
    if (root == NULL) {
        return createNode(data);
    }
    if (data < root->data) {
        root->left = insertRecursive(root->left, data);
    } else if (data > root->data) {
        root->right = insertRecursive(root->right, data);
    }
    return root;
}

// Função para percorrer a árvore em ordem (in-order traversal)
void inOrder(struct Node* root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%d ", root->data);
        inOrder(root->right);
    }
}

int main() {
    struct Node* root = NULL;
    
    // Inserindo elementos na árvore
    root = insertRecursive(root, 50);
    insertRecursive(root, 30);
    insertRecursive(root, 70);
    insertRecursive(root, 20);
    insertRecursive(root, 40);
    insertRecursive(root, 60);
    insertRecursive(root, 80);
    
    // Imprimindo a árvore em ordem
    printf("Árvore (In-Order Traversal): ");
    inOrder(root);
    printf("\n");
    
    return 0;
}

