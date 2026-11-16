#ifndef HASH_ABERTO_H
#define HASH_ABERTO_H
#include "../utils/aluno.h"

/* TAD Tabela Hash com enderecamento aberto e SONDAGEM LINEAR */
typedef struct hashAberto* HashAberto;

/* tipo_funcao = HASH_MULTIPLICACAO ou HASH_MEIO_QUADRADO (funcoes_hash.h) */
HashAberto criar_hash_aberto(int TABLE_SIZE, int tipo_funcao);
int   inserir_hash_aberto(HashAberto ha, Aluno a);        /* 1 = sucesso, 0 = falha */
Aluno buscar_hash_aberto(HashAberto ha, int matricula);   /* NULL se nao encontrar */
int   remover_hash_aberto(HashAberto ha, int matricula);  /* estado vira REMOVIDO; 1 = sucesso, 0 = nao achou */
void  destruir_hash_aberto(HashAberto ha);                /* nao libera os alunos */

/* Metricas do experimento */
int       qtd_hash_aberto(HashAberto ha);
double    fator_carga_hash_aberto(HashAberto ha);  /* alfa = n / m */
int       colisoes_hash_aberto(HashAberto ha);     /* indice inicial ja produzido antes */
int       sondagens_extras_hash_aberto(HashAberto ha); /* posicoes extras testadas nas insercoes */

#endif
