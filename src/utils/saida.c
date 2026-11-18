#include <stdio.h>
#include <stdarg.h>
#include "saida.h"

/* Arquivo onde a saida e gravada (NULL = so na tela) */
static FILE *arquivo_saida = NULL;

int abrir_saida(char *arquivo){
    arquivo_saida = fopen(arquivo, "w");
    if(arquivo_saida == NULL){
        return 0;
    }
    return 1;
}

/* Os "..." recebem os mesmos valores de um printf.
   va_list (de stdarg.h) e o jeito do C de repassar esses valores:
   usamos uma vez para a tela e outra vez para o arquivo. */
void escrever(const char *formato, ...){
    va_list valores;

    va_start(valores, formato);
    vprintf(formato, valores);
    va_end(valores);
    fflush(stdout);                     /* mostra na hora, sem esperar */

    if(arquivo_saida != NULL){
        va_start(valores, formato);
        vfprintf(arquivo_saida, formato, valores);
        va_end(valores);
        fflush(arquivo_saida);          /* se o programa parar, o que ja saiu fica salvo */
    }
}

void fechar_saida(){
    if(arquivo_saida != NULL){
        fclose(arquivo_saida);
        arquivo_saida = NULL;
    }
}
