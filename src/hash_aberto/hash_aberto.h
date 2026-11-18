#ifndef HASH_ABERTO_H
#define HASH_ABERTO_H
#include "../utils/aluno.h"

/* Tabela hash com enderecamento aberto e sondagem linear */
typedef struct hashAberto* HashAberto;

/* funcao = MULTIPLICACAO ou MEIO_QUADRADO (funcoes_hash.h) */
HashAberto criar_hash_aberto(int TABLE_SIZE, int funcao);
int   inserir_hash_aberto(HashAberto ha, Aluno a);         /* 1 = sucesso, 0 = falha */
Aluno buscar_hash_aberto(HashAberto ha, int matricula);    /* NULL se nao encontrar */
int   remover_hash_aberto(HashAberto ha, int matricula);   /* 1 = removeu, 0 = nao achou */
void  destruir_hash_aberto(HashAberto ha);

int    colisoes_hash_aberto(HashAberto ha);
double fator_carga_hash_aberto(HashAberto ha);              /* alfa = qtd / TABLE_SIZE */

#endif
