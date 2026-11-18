#ifndef HASH_ENCADEADO_H
#define HASH_ENCADEADO_H
#include "../utils/aluno.h"

/* Tabela hash com encadeamento separado */
typedef struct hashEncadeado* HashEncadeado;

/* funcao = MULTIPLICACAO ou MEIO_QUADRADO (funcoes_hash.h) */
HashEncadeado criar_hash_encadeado(int TABLE_SIZE, int funcao);
int   inserir_hash_encadeado(HashEncadeado ha, Aluno a);         /* 1 = sucesso, 0 = falha */
Aluno buscar_hash_encadeado(HashEncadeado ha, int matricula);    /* NULL se nao encontrar */
int   remover_hash_encadeado(HashEncadeado ha, int matricula);   /* 1 = removeu, 0 = nao achou */
void  destruir_hash_encadeado(HashEncadeado ha);

int    colisoes_hash_encadeado(HashEncadeado ha);
double fator_carga_hash_encadeado(HashEncadeado ha);              /* alfa = qtd / TABLE_SIZE */

#endif
