#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitura.h"

int ler_alunos(const char *caminho, int max_registros, Aluno **vetor){
    FILE *arq = fopen(caminho, "r");
    if(arq == NULL) return -1;

    Aluno *v = malloc((size_t) max_registros * sizeof(Aluno));
    if(v == NULL){ fclose(arq); return -1; }

    char linha[256];
    int n = 0, ignoradas = 0;
    while(n < max_registros && fgets(linha, sizeof(linha), arq) != NULL){
        char *mat   = strtok(linha, ";");
        char *nome  = strtok(NULL, ";");
        char *curso = strtok(NULL, ";\r\n");      /* tira a quebra de linha */
        if(mat == NULL || nome == NULL || curso == NULL){
            ignoradas++;                          /* linha incompleta */
            continue;
        }
        Aluno a = criar_aluno(atoi(mat), nome, curso);
        if(a == NULL){
            ignoradas++;                          /* campo longo demais (ou sem memoria) */
            continue;
        }
        v[n++] = a;
    }
    fclose(arq);
    if(ignoradas > 0)
        printf("Aviso: %d linha(s) ignorada(s): incompleta(s) ou com nome/curso maior que %d caracteres.\n", ignoradas, TAM - 1);
    *vetor = v;
    return n;
}

void liberar_alunos(Aluno *vetor, int n){
    if(vetor == NULL) return;
    for(int i = 0; i < n; i++) destruir_aluno(vetor[i]);
    free(vetor);
}
