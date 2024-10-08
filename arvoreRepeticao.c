#include <stdio.h>
#include <stdlib.h>

// Defini��o do n� da �rvore bin�ria
struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

// Fun��o para criar um novo n�
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Fun��o para inserir um novo n� usando repeti��o
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

// Fun��o para percorrer a �rvore em ordem (in-order traversal)
void emOrdem(struct Node* root) {
    if (root != NULL) {
        emOrdem(root->left);
        printf("%d ", root->data);
        emOrdem(root->right);
    }
}

int main() {
    struct Node* root = NULL;
    
    // Inserindo elementos na �rvore usando repeti��o
    root = insertIterative(root, 50);
    insertIterative(root, 30);
    insertIterative(root, 70);
    insertIterative(root, 20);
    insertIterative(root, 40);
    insertIterative(root, 60);
    insertIterative(root, 80);
    
    // Imprimindo a �rvore em ordem
    printf("�rvore (In-Order Traversal): ");
    emOrdem(root);
    printf("\n");
    
    return 0;
}

