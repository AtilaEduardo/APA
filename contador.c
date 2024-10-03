#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#define MAX_LINHA 1024
#define MAX_SIMBOLOS 50 // Definindo uma constante para o número máximo de símbolos

void validaErroArqv();
void calcularContadores(const char *nomeArquivo, void (*funcao)(const char*, int*, const char**, int), int *contadores, const char *simbolos[], int n);
void calcularSimples(const char *linha, int *contadores, const char *simbolos[], int n);
void calcularNsimples(const char *linha, int *contadores, const char *simbolos[], int n);
void medirTempoExecucao(const char *nomeArquivo, void (*funcao)(const char*, int*, const char**, int), int *contadores, const char *simbolos[], int n);
int verificaSimbolo(const char *linha, const char *simbolo);
void detectarRecursividade(const char *nomeArquivo);

int main(void) {
    setlocale(LC_ALL, "Portuguese");

    const char *simbolosSimples[] = {
        "+", "-", "*", "/", "//", "/*", "*/", "=", "==", "!=", "-=", "+=", "if(", "else", "while(", 
        "printf(", "scanf(", "int ", "float ", "double ", "long ", ">=", "<=", "char ", "%", "&"
    };
    int contadoresSimples[MAX_SIMBOLOS] = {0};

    medirTempoExecucao("Sequencia e Conjunto.c", calcularSimples, contadoresSimples, simbolosSimples, sizeof(simbolosSimples) / sizeof(simbolosSimples[0]));

    printf("\n======================Processamento Executado com Sucesso======================\n\n");

    const char *simbolosNsimples[] = {
        "for(", "do{", "}while(", "struct ", "typedef ", "switch ", "memset("
    };
    int contadoresNsimples[MAX_SIMBOLOS] = {0};

    medirTempoExecucao("Sequencia e Conjunto.c", calcularNsimples, contadoresNsimples, simbolosNsimples, sizeof(simbolosNsimples) / sizeof(simbolosNsimples[0]));

    // Detectar recursividade no código analisado
    detectarRecursividade("Sequencia e Conjunto.c");

    return 0;
}

void medirTempoExecucao(const char *nomeArquivo, void (*funcao)(const char*, int*, const char**, int), int *contadores, const char *simbolos[], int n) {
    clock_t inicio, fim;
    double tempoGasto;
    inicio = clock();
    int i;

    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    FILE *saida = fopen("saida.txt", "a");
    if (saida == NULL) {
        perror("Erro ao abrir o arquivo de saída");
        fclose(arquivo);
        exit(EXIT_FAILURE);
    }

    // Zera os contadores antes de processar o arquivo
    memset(contadores, 0, sizeof(int) * n);

    char linha[MAX_LINHA];
    while (fgets(linha, MAX_LINHA, arquivo)) {
        funcao(linha, contadores, simbolos, n);
    }

    // Escreve os resultados no arquivo
    fprintf(saida, "\nChamada do contador para o arquivo %s\n", nomeArquivo);
    for (i = 0; i < n; i++) {
        fprintf(saida, "Contador %s: %d\n", simbolos[i], contadores[i]);
    }

    fim = clock();
    tempoGasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    fprintf(saida, "Tempo de Execucao: %lf segundos\n", tempoGasto);

    fclose(arquivo);
    fclose(saida);
}

void calcularSimples(const char *linha, int *contadores, const char *simbolos[], int n) {
    int i;
	for (i = 0; i < n; i++) {
        if (verificaSimbolo(linha, simbolos[i])) {
            contadores[i]++;
        }
    }
}

void calcularNsimples(const char *linha, int *contadores, const char *simbolos[], int n) {
    int i;
	for (i = 0; i < n; i++) {
        if (verificaSimbolo(linha, simbolos[i])) {
            contadores[i]++;
        }
    }
}

int verificaSimbolo(const char *linha, const char *simbolo) {
    // Verifica a presença do símbolo como uma substring e também se ele está em limites de palavra
    const char *ptr = strstr(linha, simbolo);
    if (ptr != NULL) {
        // Verifica se o símbolo está no início ou em limites de palavra
        int pos = ptr - linha;
        if ((pos == 0 || isspace(linha[pos - 1])) && (isspace(linha[pos + strlen(simbolo)]) || linha[pos + strlen(simbolo)] == '\0')) {
            return 1;
        }
    }
    return 0;
}

// Função para detectar recursividade
void detectarRecursividade(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char linha[MAX_LINHA];
    char funcaoAtual[100] = ""; // Para armazenar o nome da função atual
    int dentroFuncao = 0; // Para indicar se estamos dentro de uma função

    while (fgets(linha, MAX_LINHA, arquivo)) {
        // Remover espaços no início da linha
        char *ptr = linha;
        while (isspace(*ptr)) ptr++;

        // Verifica se estamos no início de uma definição de função
        if (!dentroFuncao && (strstr(ptr, "int ") == ptr || strstr(ptr, "void ") == ptr ||
            strstr(ptr, "float ") == ptr || strstr(ptr, "double ") == ptr || strstr(ptr, "char ") == ptr)) {

            // Captura o nome da função
            char *inicioNome = strchr(ptr, ' ') + 1;
            char *fimNome = strchr(inicioNome, '(');
            if (fimNome) {
                strncpy(funcaoAtual, inicioNome, fimNome - inicioNome);
                funcaoAtual[fimNome - inicioNome] = '\0';
                dentroFuncao = 1; // Agora estamos dentro de uma função
            }
        }

        // Verifica se estamos no corpo da função e se há uma chamada recursiva
        if (dentroFuncao && strstr(ptr, funcaoAtual)) {
            printf("Recursividade detectada na função: %s\n", funcaoAtual);
        }

        // Verifica se chegamos ao final da função (usualmente pelo fechamento do bloco de chaves)
        if (dentroFuncao && strchr(ptr, '}')) {
            dentroFuncao = 0; // Saímos do corpo da função
            strcpy(funcaoAtual, ""); // Limpa o nome da função
        }
    }

    fclose(arquivo);
}
