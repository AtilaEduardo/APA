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

// Função para inserir um novo nó usando repetição
struct Node* insertIterative(struct Node* root, int data) {
    struct Node* newNode = createNode(data);
    
    if (root == NULL) {
        return newNode;
    }
    
    struct Node* current = root;
    struct Node* parent = NULL;
    
    while (current != NULL) {
        parent = current;
        if (data < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    if (data < parent->data) {
        parent->left = newNode;
    } else {
        parent->right = newNode;
    }
    
    return root;
}

// Função para percorrer a árvore em ordem (in-order traversal)
void emOrdem(struct Node* root) {
    if (root != NULL) {
        emOrdem(root->left);
        printf("%d ", root->data);
        emOrdem(root->right);
    }
}

int main() {
    struct Node* root = NULL;
    
    // Inserindo elementos na árvore usando repetição
    root = insertIterative(root, 50);
    insertIterative(root, 30);
    insertIterative(root, 70);
    insertIterative(root, 20);
    insertIterative(root, 40);
    insertIterative(root, 60);
    insertIterative(root, 80);
    
    // Imprimindo a árvore em ordem
    printf("Árvore (In-Order Traversal): ");
    emOrdem(root);
    printf("\n");
    
    return 0;
}

