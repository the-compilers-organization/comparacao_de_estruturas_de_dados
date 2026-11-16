#ifndef HASH_ENCADEADO_H
#define HASH_ENCADEADO_H
#include "../utils/aluno.h"

/* TAD Tabela Hash com ENCADEAMENTO SEPARADO */
typedef struct hashEncadeado* HashEncadeado;

HashEncadeado criar_hash_encadeado(int TABLE_SIZE, int tipo_funcao);
int   inserir_hash_encadeado(HashEncadeado ha, Aluno a);       /* 1 = sucesso, 0 = falha */
Aluno buscar_hash_encadeado(HashEncadeado ha, int matricula);  /* NULL se nao encontrar */
int   remover_hash_encadeado(HashEncadeado ha, int matricula); /* tira o no da lista; 1 = sucesso, 0 = nao achou */
void  destruir_hash_encadeado(HashEncadeado ha);               /* nao libera os alunos */

/* Metricas do experimento */
int       qtd_hash_encadeado(HashEncadeado ha);
double    fator_carga_hash_encadeado(HashEncadeado ha);   /* alfa = n / m, pode passar de 1 */
int       colisoes_hash_encadeado(HashEncadeado ha);
int       maior_lista_hash_encadeado(HashEncadeado ha);   /* percorre a tabela: O(m) */

#endif
