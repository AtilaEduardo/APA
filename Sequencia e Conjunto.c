#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
/*
	Name: Sequencia e Conjunto 
	Author: �tila Eduardo da Cruz Santos Cardoso
	Date: 22/09/24 23:05
	Description: Recurs�o envolvendo Sequencia de Collatz e Conjunto de Subconjuntos
*/

void collatz(int n, int* resultado, int* tamanho);
void imprimirSubconjunto(int arr[], int n, int index, int subconjunto[], int tamanhoSubconjunto);

void collatz(int n, int* resultado, int* tamanho) { // Fun��o recursiva para calcular a sequencia de Collatz
    resultado[(*tamanho)++] = n;
    if (n == 1) return;
    if (n % 2 == 0) {
        collatz(n / 2, resultado, tamanho);
    } else {
        collatz(3 * n + 1, resultado, tamanho);
    }
}

void imprimirSubconjunto(int arr[], int n, int index, int subconjunto[], int tamanhoSubconjunto) {
	int i;
    // Imprime o subconjunto atual
    printf("{ ");
    for (i = 0; i < tamanhoSubconjunto; i++) {
        printf("%d ", subconjunto[i]);
    }
    printf("}\n");

    // Gera subconjuntos a partir do �ndice atual
    for (i = index; i < n; i++) {
        subconjunto[tamanhoSubconjunto] = arr[i];
        imprimirSubconjunto(arr, n, i + 1, subconjunto, tamanhoSubconjunto + 1);
    }
}

int main() {
    int n;
    setlocale(LC_ALL,"");
    printf("Digite um n�mero inteiro positivo para a sequ�ncia de Collatz: ");
    scanf("%d", &n);

    // Calcula a sequ�ncia de Collatz
    int collatzResultado[100];
    int collatzTamanho = 0;
    collatz(n, collatzResultado, &collatzTamanho);

    // Imprime os subconjuntos do resultado da sequ�ncia de Collatz
    printf("\t Subconjuntos da sequ�ncia de Collatz:\n");
    imprimirSubconjunto(collatzResultado, collatzTamanho, 0, (int[100]){}, 0);

    return 0;
}

