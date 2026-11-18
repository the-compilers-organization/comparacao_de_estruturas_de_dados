#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leitura.h"

int ler_alunos(char *arquivo, Aluno alunos[], int max){
    FILE *arq = fopen(arquivo, "r");
    if(arq == NULL){
        return 0;
    }

    char linha[256];
    int n = 0;
    int ignoradas = 0;

    while(n < max && fgets(linha, 256, arq) != NULL){
        /* separa a linha nos ";" */
        char *matricula = strtok(linha, ";");
        char *nome      = strtok(NULL, ";");
        char *curso     = strtok(NULL, ";\r\n");   /* tira tambem a quebra de linha */

        if(matricula == NULL || nome == NULL || curso == NULL){
            ignoradas++;                            /* linha incompleta */
            continue;
        }

        Aluno a = criar_aluno(atoi(matricula), nome, curso);
        if(a == NULL){
            ignoradas++;                            /* campo grande demais */
            continue;
        }

        alunos[n] = a;
        n++;
    }

    fclose(arq);
    if(ignoradas > 0){
        printf("Aviso: %d linha(s) ignorada(s).\n", ignoradas);
    }
    return n;
}
