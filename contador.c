#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#define MAX_LINHA 1024
/****************************************************************
 * NOME DO PROGRAMA:  CONTADOR                                  *
 * DESCRICAO:                                                   *
 * DATA: --/--/----                                             *
 * INSTITUIÇÃO:                                                 *
 * ALUNO(A):                                                    *
 ****************************************************************/
void validaErroArqv();
void calcularContadores(const char *nomeArquivo, void (*funcao)(const char*, int*, const char**, int), int *contadores, const char *simbolos[], int n);
void calcularSimples(const char *linha, int *contadores, const char *simbolos[], int n);
void calcularNsimples(const char *linha, int *contadores, const char *simbolos[], int n);
void medirTempoExecucao(const char *nomeArquivo, void (*funcao)(const char*, int*, const char**, int), int *contadores, const char *simbolos[], int n);
//
int main(void) {
    setlocale(LC_ALL, "Portuguese");
//
    const char *simbolosSimples[] = {
        " + ", " - ", " * ", " / ", " // ", " /* ", " */ ", " = ", " == ", " != ",
        " -= ", " += ", "if(", "else", "while(", "printf(", "scanf(", "int ", "float ",
        "double ", "long ", " >= ", " <= ", "char ", "%", "&"
    };
    int contadoresSimples[sizeof(simbolosSimples) / sizeof(simbolosSimples[0])] = {0};
//
    medirTempoExecucao("Menor.c", calcularSimples, contadoresSimples, simbolosSimples, sizeof(simbolosSimples) / sizeof(simbolosSimples[0]));
//
    printf("\n======================Divisão de Contadores======================\n\n");
//
    const char *simbolosNsimples[] = {
        "for(", "do{", "}while(", "struct ", "typedef ", "switch ", "memset "
    };
    int contadoresNsimples[sizeof(simbolosNsimples) / sizeof(simbolosNsimples[0])] = {0};
//
    medirTempoExecucao("Menor.c", calcularNsimples, contadoresNsimples, simbolosNsimples, sizeof(simbolosNsimples) / sizeof(simbolosNsimples[0]));
//
    return 0;
}
//
void calcularContadores(const char *nomeArquivo, void (*funcao)(const char*, int*, const char**, int), int *contadores, const char *simbolos[], int n) {
    char linha[MAX_LINHA];
    FILE *arqv = fopen(nomeArquivo, "r");
    int i, total = 0;
    if (!arqv) {
        validaErroArqv();
    }
//
    while (fgets(linha, sizeof(linha), arqv) != NULL) {
        funcao(linha, contadores, simbolos, n);
    }
//
    fclose(arqv);
//
    for (i = 0; i < n; ++i) {
        printf("Contador %s: %d\n", simbolos[i], contadores[i]);
    }
//
    for (i = 0; i < n; ++i) {
        total += contadores[i];
    }
    printf("Total: %d\n", total);
}
//
void calcularSimples(const char *linha, int *contadores, const char *simbolos[], int n) {
	int i;
    for (i = 0; i < n; ++i) {
        if (strstr(linha, simbolos[i]) != NULL) {
            contadores[i]++;
        }
    }
}
//
void calcularNsimples(const char *linha, int *contadores, const char *simbolos[], int n) {
	int i;
    for (i = 0; i < n; ++i) {
        if (strstr(linha, simbolos[i]) != NULL) {
            contadores[i] += 2;
        }
    }
}
//
void validaErroArqv() {
    perror("Erro ao abrir arquivo");
    printf("************** Contador.c *************\n");
    printf("*                                     *\n");
    printf("*       ERRO AO ABRIR ARQUIVO         *\n");
    printf("*                                     *\n");
    printf("*      PROCESSAMENTO  ENCERRADO       *\n");
    printf("*                                     *\n");
    printf("************** Contador.c *************\n");
    exit(EXIT_FAILURE);
}
//
void medirTempoExecucao(const char *nomeArquivo, void (*funcao)(const char*, int*, const char**, int), int *contadores, const char *simbolos[], int n) {
    char nomeArquivoSaida[256];
    snprintf(nomeArquivoSaida, sizeof(nomeArquivoSaida), "%s_resultado.txt", nomeArquivo);
//
    FILE *saida = fopen(nomeArquivoSaida, "w");
    if (!saida) {
        perror("Erro ao criar arquivo de saída");
        exit(EXIT_FAILURE);
    }
//
    printf("\nChamada do contador para o arquivo %s\n", nomeArquivo);
    clock_t begin = clock();
    calcularContadores(nomeArquivo, funcao, contadores, simbolos, n);
    clock_t end = clock();
    double tempoExecucao = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nFim da execução para o arquivo %s!\n", nomeArquivo);
    printf("Tempo de Execucao: %f segundos\n", tempoExecucao);
//
    fprintf(saida, "Chamada do contador para o arquivo %s\n", nomeArquivo);
    fprintf(saida, "Tempo de Execucao: %f segundos\n", tempoExecucao);
//
    fprintf(saida, "\nEstimação da Complexidade Assintótica:\n");
    fprintf(saida, "Big O (Supremo): O(L * n) - L é o número de linhas e n é o número de símbolos.\n");
    fprintf(saida, "Little o (Estritamente menor que): o(L * n) - O número de verificações é estritamente menor do que um cenário cúbico.\n");
    fprintf(saida, "Omega (Inferior): O(L * n) - O mínimo comportamento seria linear com relação ao número total de símbolos verificados.\n");
    fprintf(saida, "Theta (Exatamente): T(L * n) - A complexidade é assintoticamente linear no número de linhas vezes o número de símbolos verificados.\n");
//
    fclose(saida);
}
// para proxima aula apresentação com algoritimo com o metodo de recurção envolvendo sequencia e conjunto
