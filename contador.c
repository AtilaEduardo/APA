#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINHA 1024
#define MAX_SIMBOLOS 50

void calcularContadores(const char *nomeArquivo, void (*funcao)(const char*, int*, const char**, int), int *contadores, const char *simbolos[], int n);
void calcularSimples(const char *linha, int *contadores, const char *simbolos[], int n);
void medirTempoExecucao(const char *nomeArquivo, void (*funcao)(const char*, int*, const char**, int), int *contadores, const char *simbolos[], int n);
int verificaSimbolo(const char *linha, const char *simbolo);
void analisarComplexidade(const char *nomeArquivo, FILE *saida);
void detectarRecursividade(const char *nomeArquivo, FILE *saida);
void identificarTecnicasProjeto(const char *nomeArquivo, FILE *saida);  // Nova função

int main(void) {
    
    const char *simbolos[] = {
        "//", "/*", "*/", "=", "==", "!=", "-=", "+=", "if", "else", "while", 
        "printf", "scanf", "int", "float", "double", "long", ">=", "<=", "char", "%", "&", "int*", 
        "float*", "double*", "long*", "char*", "for", "fprintf", "fscanf", "fopen", "fclose","switch", 
        "fwrite", "fread", "fseek", "ftell", "rewind", "fflush", "remove", "rename", "case", "default",
         "break", "|"
    };
    int contadoresSimbolos[MAX_SIMBOLOS] = {0};

    medirTempoExecucao("arvoreRepeticao.c", calcularSimples, contadoresSimbolos, simbolos, sizeof(simbolos) / sizeof(simbolos[0]));
    medirTempoExecucao("arvoreRecursiva.c", calcularSimples, contadoresSimbolos, simbolos, sizeof(simbolos) / sizeof(simbolos[0]));
    medirTempoExecucao("Sequencia e Conjunto.c", calcularSimples, contadoresSimbolos, simbolos, sizeof(simbolos) / sizeof(simbolos[0]));
    medirTempoExecucao("boubleSort.c", calcularSimples, contadoresSimbolos, simbolos, sizeof(simbolos) / sizeof(simbolos[0]));
    medirTempoExecucao("probabilidade.c", calcularSimples, contadoresSimbolos, simbolos, sizeof(simbolos) / sizeof(simbolos[0]));

    printf("Processamento realizado com sucesso!");
    return 0;
}

void medirTempoExecucao(const char *nomeArquivo, void (*funcao)(const char*, int*, const char**, int), int *contadores, const char *simbolos[], int n) {
    clock_t inicio, fim;
    double tempoGasto;
    
    inicio = clock();
    int i, total = 0;

    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char nomeSaida[256];
    sprintf(nomeSaida, "saida_%s.txt", nomeArquivo);  
    FILE *saida = fopen(nomeSaida, "a");
    if (saida == NULL) {
        perror("Erro ao abrir o arquivo de saída");
        fclose(arquivo);
        exit(EXIT_FAILURE);
    }

    memset(contadores, 0, sizeof(int) * n);

    char linha[MAX_LINHA];
    while (fgets(linha, MAX_LINHA, arquivo)) {
        funcao(linha, contadores, simbolos, n);
    }

    fprintf(saida, "Chamada do contador para o arquivo %s\n", nomeArquivo);
    for (i = 0; i < n; i++) {
        fprintf(saida, "Contador %s: %d\n", simbolos[i], contadores[i]);
        fflush(saida);
        total += contadores[i];
    }

    fprintf(saida, "Total de ocorrências: %d\n\n", total);

    fim = clock();
    tempoGasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    fprintf(saida, "Tempo de Execucao: %lf segundos\n\n", tempoGasto);

    detectarRecursividade(nomeArquivo, saida);  // Detectar Recursividade
    analisarComplexidade(nomeArquivo, saida);  // Analisar Complexidade Assintótica
    identificarTecnicasProjeto(nomeArquivo, saida);  // Identificar as técnicas de projeto

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

int verificaSimbolo(const char *linha, const char *simbolo) {
    const char *ptr = strstr(linha, simbolo);
    while (ptr != NULL) {
        int pos = ptr - linha;
        if ((pos == 0 || isspace(linha[pos - 1]) || ispunct(linha[pos - 1])) && 
            (isspace(linha[pos + strlen(simbolo)]) || ispunct(linha[pos + strlen(simbolo)]) || linha[pos + strlen(simbolo)] == '\0')) {
            return 1;
        }
        ptr = strstr(ptr + 1, simbolo);
    }
    return 0;
}

void detectarRecursividade(const char *nomeArquivo, FILE *saida) {
    char linha[MAX_LINHA];
    char funcaoAtual[100] = "";
    int dentroFuncao = 0;
    int recursividadeEncontrada = 0;
    int abriuChave = 0;  // Verifica se está dentro do corpo de uma função

    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    while (fgets(linha, MAX_LINHA, arquivo)) {
        char *ptr = linha;
        while (isspace(*ptr)) ptr++;

        // Remover comentários de linha única (//)
        char *comentarioLinha = strstr(ptr, "//");
        if (comentarioLinha) {
            *comentarioLinha = '\0';
        }

        if (!dentroFuncao && (strstr(ptr, "int ") == ptr || strstr(ptr, "void ") == ptr ||
            strstr(ptr, "float ") == ptr || strstr(ptr, "double ") == ptr || strstr(ptr, "char ") == ptr)) {
            
            char *inicioNome = strchr(ptr, ' ') + 1;
            char *fimNome = strchr(inicioNome, '(');
            if (fimNome) {
                strncpy(funcaoAtual, inicioNome, fimNome - inicioNome);
                funcaoAtual[fimNome - inicioNome] = '\0';

                if (strcmp(funcaoAtual, "main") == 0) {
                    dentroFuncao = 0;
                    continue;
                }

                dentroFuncao = 1;
                recursividadeEncontrada = 0;
                abriuChave = 0;
            }
        }

        if (dentroFuncao && strchr(ptr, '{')) {
            abriuChave = 1;
        }

        if (dentroFuncao && abriuChave && strstr(ptr, funcaoAtual) && strchr(ptr, '(')) {
            if (verificaChamadaFuncao(ptr, funcaoAtual)) {
                recursividadeEncontrada = 1;
            }
        }

        if (dentroFuncao && strchr(ptr, '}')) {
            if (recursividadeEncontrada) {
                fprintf(saida, "Recursividade detectada na função: %s\n\n", funcaoAtual);
            } else {
                fprintf(saida, "Função %s não é recursiva\n\n", funcaoAtual);
            }
            dentroFuncao = 0;
            abriuChave = 0;
            strcpy(funcaoAtual, "");
        }
    }

    fclose(arquivo);
}

int verificaChamadaFuncao(char *linha, const char *funcao) {
    char *pos = strstr(linha, funcao);
    while (pos != NULL) {
        if (*(pos + strlen(funcao)) == '(' && 
            (pos == linha || isspace(*(pos - 1)) || *(pos - 1) == ';' || *(pos - 1) == '{')) {
            int dentroString = 0;
            char *c;
            for (c = linha; c < pos; c++) {
                if (*c == '"') {
                    dentroString = !dentroString;
                }
            }
            if (!dentroString) {
                return 1;
            }
        }
        pos = strstr(pos + 1, funcao);
    }
    return 0;
}

void analisarComplexidade(const char *nomeArquivo, FILE *saida) {
    char linha[MAX_LINHA];
    int contagemLoops = 0;
    int profundidadeLoop = 0;
    int contagemRecursao = 0;
    int logaritmica = 0;  // Indicador de complexidade logarítmica
    int dentroFuncao = 0;
    int recursivaComLoop = 0;  // Detectar recursão com loop
    int melhorCasoLogaritmico = 0; // Identificar divisões que indicam melhor caso logaritmico
    int melhorCasoConstante = 0;  // Detectar terminação antecipada
    int piorCaso = 0;

    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    while (fgets(linha, MAX_LINHA, arquivo)) {
        // Verifica a presença de loops e aumenta a profundidade
        if (strstr(linha, "for(") || strstr(linha, "while(") || strstr(linha, "do{")) {
            contagemLoops++;
            profundidadeLoop++;
            if (dentroFuncao && contagemRecursao > 0) {
                recursivaComLoop = 1;  // Função recursiva contém loop
            }
        }

        // Detecta divisão no loop, indicando possível complexidade logarítmica
        if (strstr(linha, "/2") || strstr(linha, ">>1")) {
            logaritmica = 1;  // Indicação de divisão do espaço de busca
        }

        // Detecta terminação antecipada, representando um melhor caso constante
        if (strstr(linha, "if") && strstr(linha, "return")) {
            melhorCasoConstante = 1;  // Melhor caso O(1) encontrado
        }

        // Detecta o fechamento de loops para reduzir a profundidade
        if (strstr(linha, "}")) {
            if (profundidadeLoop > 0) {
                profundidadeLoop--;
            }
        }

        // Verifica a presença de recursividade
        if (dentroFuncao && strstr(linha, "return") && strstr(linha, "(")) {
            contagemRecursao++;
        }

        // Detectar início e fim de funções para analisar recursividade corretamente
        if (strstr(linha, "void ") || strstr(linha, "int ") || strstr(linha, "float ") || strstr(linha, "double ")) {
            dentroFuncao = 1;
        }

        if (dentroFuncao && strstr(linha, "}")) {
            dentroFuncao = 0;
        }
    }

    // Determinação da complexidade com base na profundidade dos loops e padrões
    // Pior caso
    if (recursivaComLoop) {
        fprintf(saida, "Pior caso - Complexidade Assintótica: O(n log n)\n");
        piorCaso = 1;
    } else if (contagemRecursao > 0) {
        fprintf(saida, "Pior caso - Complexidade Assintótica: O(2^n)\n");
        piorCaso = 1;
    } else if (profundidadeLoop > 1) {
        fprintf(saida, "Pior caso - Complexidade Assintótica: O(n^%d)\n", profundidadeLoop);
        piorCaso = 1;
    } else if (profundidadeLoop == 1) {
        fprintf(saida, "Pior caso - Complexidade Assintótica: O(n)\n");
        piorCaso = 1;
    } else {
        fprintf(saida, "Pior e Melhor caso - Complexidade Assintótica: O(1)\n");
    }

    // Melhor caso
    if (logaritmica) {
        fprintf(saida, "Melhor caso - Complexidade Assintótica: O(log n)\n");
    } else if (melhorCasoConstante) {
        fprintf(saida, "Melhor caso - Complexidade Assintótica: O(1)\n");
    } else if (piorCaso && !melhorCasoConstante) {
        fprintf(saida, "Melhor caso - Complexidade Assintótica: O(n)\n");
    }

    fflush(saida);
    fclose(arquivo);
}

//Função para identificar as técnicas de projeto
void identificarTecnicasProjeto(const char *nomeArquivo, FILE *saida) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }

    char linha[MAX_LINHA];
    int recursao = 0, divisaoProblema = 0, tabelaMemoria = 0;

    while (fgets(linha, MAX_LINHA, arquivo)) {
        if (strstr(linha, "return") && strstr(linha, "(")) {
            recursao = 1;
        }

        if (strstr(linha, "/2") || strstr(linha, ">>1")) {
            divisaoProblema = 1;
        }

        if (strstr(linha, "malloc") || strstr(linha, "calloc")) {
            tabelaMemoria = 1;
        }
    }

    if (recursao && divisaoProblema) {
        fprintf(saida, "Técnica de Projeto: Dividir para Conquistar\n");
    } else if (tabelaMemoria) {
        fprintf(saida, "Técnica de Projeto: Programação Dinâmica\n");
    } else if (recursao) {
        fprintf(saida, "Técnica de Projeto: Recursão Simples\n");
    } else {
        fprintf(saida, "Técnica de Projeto: Iterativa\n");
    }

    fclose(arquivo);
}
