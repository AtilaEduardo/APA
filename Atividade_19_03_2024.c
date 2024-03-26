#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//
#define LIMITE_SUPERIOR 101
//
void formatarDadosEntrada();
//
int main(void) {
//
        formatarDadosEntrada();
//
    return 0;
}
//
void formatarDadosEntrada() {
    int data[100], i, n, step, temp;
//
    printf("Digite o número de elementos que deseja ordenar: ");
    scanf("%d", &n);
//
    srand(time(NULL));
//
    for(i = 0; i < n; ++i) {
        data[i] = rand() % LIMITE_SUPERIOR;
    }
//
    for(step = 0; step < n - 1; ++step) {
        for(i = 0; i < n - step - 1; ++i) {
            if(data[i] > data[i + 1]) {
                temp = data[i];
                data[i] = data[i + 1];
                data[i + 1] = temp;
            }
        }
    }
//
    printf("Em ordem crescente: ");
    for(i = 0; i < n; ++i) {
        printf("%d  ", data[i]);
    }
}
