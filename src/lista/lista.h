#ifndef LISTA_H
#define LISTA_H
#include "../utils/aluno.h"

/* TAD Lista unicamente encadeada com no descritor (inicio e qtd) */
typedef struct lista* Lista;

Lista criar_lista();
int   inserir_inicio(Lista li, Aluno a);   /* 1 = sucesso, 0 = falha */
Aluno buscar_lista(Lista li, int matricula); /* NULL se nao encontrar */
int   tamanho_lista(Lista li);
void  destruir_lista(Lista li);            /* libera os nos, nao os alunos */

#endif
