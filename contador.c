#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
//
#define MAX_LINHA 1024
#define MAX_SIMBOLOS 50
//
void calcularContadores(const char *nomeArquivo, void (*funcao)(const char*, int*, const char**, int), int *contadores, const char *simbolos[], int n);
void calcularSimples(const char *linha, int *contadores, const char *simbolos[], int n);
void medirTempoExecucao(const char *nomeArquivo, void (*funcao)(const char*, int*, const char**, int), int *contadores, const char *simbolos[], int n);
int verificaSimbolo(const char *linha, const char *simbolo);
void analisarComplexidade(const char *nomeArquivo, FILE *saida);
void detectarRecursividade(const char *nomeArquivo, FILE *saida);
//
int main(void) {
//	
        const char *simbolos[] = {
        "+", "-", "*", "/", "//", "/*", "*/", "=", "==", "!=", "-=", "+=", "if", "else", "while", 
        "printf", "scanf", "int", "float", "double", "long", ">=", "<=", "char", "%", "&", "int*", 
        "float*", "double*", "long*", "char*", "for", "fprintf", "fscanf", "fopen", "fclose","switch", 
		"fwrite", "fread", "fseek", "ftell", "rewind", "fflush", "remove", "rename", "case", "default",
		 "break", "|" 
    };
    int contadoresSimbolos[MAX_SIMBOLOS] = {0};
//
    medirTempoExecucao("arvoreRepeticao.c", calcularSimples, contadoresSimbolos, simbolos, sizeof(simbolos) / sizeof(simbolos[0]));
    medirTempoExecucao("arvoreRecursiva.c", calcularSimples, contadoresSimbolos, simbolos, sizeof(simbolos) / sizeof(simbolos[0]));
    medirTempoExecucao("Sequencia e Conjunto.c", calcularSimples, contadoresSimbolos, simbolos, sizeof(simbolos) / sizeof(simbolos[0]));
    medirTempoExecucao("boubleSort.c", calcularSimples, contadoresSimbolos, simbolos, sizeof(simbolos) / sizeof(simbolos[0]));
    medirTempoExecucao("grafocod.c", calcularSimples, contadoresSimbolos, simbolos, sizeof(simbolos) / sizeof(simbolos[0]));
//
	printf("Processamento realizado com sucesso!");
    return 0;
}
//
void medirTempoExecucao(const char *nomeArquivo, void (*funcao)(const char*, int*, const char**, int), int *contadores, const char *simbolos[], int n) {
    clock_t inicio, fim;
    double tempoGasto;
    inicio = clock();
    int i, total = 0;
//
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }
//
    char nomeSaida[256];
    sprintf(nomeSaida, "saida_%s.txt", nomeArquivo);  
    FILE *saida = fopen(nomeSaida, "a");
    if (saida == NULL) {
        perror("Erro ao abrir o arquivo de saída");
        fclose(arquivo);
        exit(EXIT_FAILURE);
    }
//
    memset(contadores, 0, sizeof(int) * n);
//
    char linha[MAX_LINHA];
    while (fgets(linha, MAX_LINHA, arquivo)) {
        funcao(linha, contadores, simbolos, n);
    }
//
    fprintf(saida, "Chamada do contador para o arquivo %s\n", nomeArquivo);
    for (i = 0; i < n; i++) {
        fprintf(saida, "Contador %s: %d\n", simbolos[i], contadores[i]);
        fflush(saida);
        total += contadores[i];
    }
//
    fprintf(saida, "Total de ocorrências: %d\n\n", total);
//
    fim = clock();
    tempoGasto = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
//
    fprintf(saida, "Tempo de Execucao: %lf segundos\n\n", tempoGasto);
//
    // Análise de complexidade e recursividade
    detectarRecursividade(nomeArquivo, saida);
    analisarComplexidade(nomeArquivo, saida);
//
    fclose(arquivo);
    fclose(saida);
}
//
void calcularSimples(const char *linha, int *contadores, const char *simbolos[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (verificaSimbolo(linha, simbolos[i])) {
            contadores[i]++;
        }
    }
}
//
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
//
void detectarRecursividade(const char *nomeArquivo, FILE *saida) {
    char linha[MAX_LINHA];
    char funcaoAtual[100] = "";
    int dentroFuncao = 0;
    int recursividadeEncontrada = 0;
    int abriuChave = 0;  // Verifica se estamos dentro do corpo de uma função
    int comentarioAtivo = 0;  // Indica se estamos dentro de um comentário de bloco (/* */)
//
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }
//
    while (fgets(linha, MAX_LINHA, arquivo)) {
        char *ptr = linha;
        while (isspace(*ptr)) ptr++;  // Ignorar espaços em branco
//
        // Remover comentários de linha única (//)
        char *comentarioLinha = strstr(ptr, "//");
        if (comentarioLinha) {
            *comentarioLinha = '\0';  // Truncar a linha ao encontrar "//"
        }
//
        // Verificar início e fim de comentários de bloco (/* */)
        char *inicioComentarioBloco = strstr(ptr, "/*");
        char *fimComentarioBloco = strstr(ptr, "*/");
        if (inicioComentarioBloco && !fimComentarioBloco) {
            comentarioAtivo = 1;
        }
        if (comentarioAtivo && fimComentarioBloco) {
            comentarioAtivo = 0;
            continue;  // Ignorar a linha enquanto estamos em um comentário de bloco
        }
        if (comentarioAtivo) {
            continue;  // Pular linhas dentro de comentários de bloco
        }
//
        // Verifica se estamos no início de uma definição de função, exceto para a função 'main'
        if (!dentroFuncao && (strstr(ptr, "int ") == ptr || strstr(ptr, "void ") == ptr ||
            strstr(ptr, "float ") == ptr || strstr(ptr, "double ") == ptr || strstr(ptr, "char ") == ptr)) {
//        
            // Captura o nome da função
            char *inicioNome = strchr(ptr, ' ') + 1;
            char *fimNome = strchr(inicioNome, '(');
            if (fimNome) {
                strncpy(funcaoAtual, inicioNome, fimNome - inicioNome);
                funcaoAtual[fimNome - inicioNome] = '\0';
//              
                // Ignora a função 'main'
                if (strcmp(funcaoAtual, "main") == 0) {
                    dentroFuncao = 0;  // Não entra no modo de análise para a função main
                    continue;
                }
//
                dentroFuncao = 1;
                recursividadeEncontrada = 0;
                abriuChave = 0; // Marcar que ainda não entramos no corpo da função
            }
        }
//
        // Verifica a abertura da chave (início do corpo da função)
        if (dentroFuncao && strchr(ptr, '{')) {
            abriuChave = 1;  // Entramos no corpo da função
        }
//
        // Detecta chamadas recursivas no corpo da função
        if (dentroFuncao && abriuChave && strstr(ptr, funcaoAtual) && strchr(ptr, '(')) {
            // Verifica se é uma chamada de função e não uma declaração, comentário ou string
            if (verificaChamadaFuncao(ptr, funcaoAtual)) {
                recursividadeEncontrada = 1;
            }
        }
//
        // Verifica se chegamos ao final da função
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
//
    fclose(arquivo);
}
//
int verificaChamadaFuncao(char *linha, const char *funcao) {
    char *pos = strstr(linha, funcao);
    while (pos != NULL) {
        // Verifica se o nome da função é seguido por '(' e precedido por algo válido (espaço, '{', ';', ou nada)
        if (*(pos + strlen(funcao)) == '(' && 
            (pos == linha || isspace(*(pos - 1)) || *(pos - 1) == ';' || *(pos - 1) == '{')) {
            // Verifica se não está dentro de uma string (evitar strings contendo o nome da função)
            int dentroString = 0;
            char *c;  // Declaração da variável fora do loop
            for (c = linha; c < pos; c++) {
                if (*c == '"') {
                    dentroString = !dentroString;  // Inverter o estado de estar dentro ou fora de uma string
                }
            }
            if (!dentroString) {
                return 1;  // Chamada de função válida
            }
        }
        pos = strstr(pos + 1, funcao);  // Continuar procurando outras ocorrências
    }
    return 0;
}
//
void analisarComplexidade(const char *nomeArquivo, FILE *saida) {
//
    char linha[MAX_LINHA];
    int contagemLoops = 0;
    int profundidadeLoop = 0;
    int contagemRecursao = 0;
    int logaritmica = 0;  // Indicador de complexidade logarítmica
    int dentroFuncao = 0;
    int recursivaComLoop = 0;  // Detectar recursão com loop
//
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        exit(EXIT_FAILURE);
    }
//
    while (fgets(linha, MAX_LINHA, arquivo)) {
        // Verifica a presença de loops e aumenta a profundidade
        if (strstr(linha, "for(") || strstr(linha, "while(") || strstr(linha, "do{")) {
            contagemLoops++;
            profundidadeLoop++;
            if (dentroFuncao && contagemRecursao > 0) {
                recursivaComLoop = 1;  // Função recursiva contém loop
            }
        }
//
        // Detecta divisão no loop, indicando possível complexidade logarítmica
        if (strstr(linha, "/2") || strstr(linha, ">>1")) {
            logaritmica = 1;  // Indicação de divisão do espaço de busca
        }
//
        // Detecta o fechamento de loops para reduzir a profundidade
        if (strstr(linha, "}")) {
            if (profundidadeLoop > 0) {
                profundidadeLoop--;
            }
        }
//
        // Verifica a presença de recursividade
        if (dentroFuncao && strstr(linha, "return") && strstr(linha, "(")) {
            contagemRecursao++;
        }
//
        // Detectar início e fim de funções para analisar recursividade corretamente
        if (strstr(linha, "void ") || strstr(linha, "int ") || strstr(linha, "float ") || strstr(linha, "double ")) {
            dentroFuncao = 1;
        }
//
        if (dentroFuncao && strstr(linha, "}")) {
            dentroFuncao = 0;
        }
    }
//
    // Determinação da complexidade com base na profundidade dos loops e padrões
    if (recursivaComLoop) {
        fprintf(saida, "Complexidade Assintótica: O(n log n)\n");
    } else if (contagemRecursao > 0) {
        fprintf(saida, "Complexidade Assintótica: O(2^n) - Recursão\n");
    } else if (logaritmica) {
        fprintf(saida, "Complexidade Assintótica: O(log n)\n");
    } else if (profundidadeLoop == 1) {
        fprintf(saida, "Complexidade Assintótica: O(n)\n");
    } else if (profundidadeLoop == 2) {
        fprintf(saida, "Complexidade Assintótica: O(n^2)\n");
    } else if (profundidadeLoop > 2) {
        fprintf(saida, "Complexidade Assintótica: O(n^%d)\n", profundidadeLoop);
    } else {
        fprintf(saida, "Complexidade Assintótica: O(1)\n");
    }
//
    fflush(saida);
    fclose(arquivo);
}
