#ifndef FUNCOES_HASH_H
#define FUNCOES_HASH_H

/* Qual funcao de hashing a tabela vai usar */
#define MULTIPLICACAO 1
#define MEIO_QUADRADO 2

int hash_multiplicacao(int chave, int TABLE_SIZE);
int hash_meio_quadrado(int chave, int TABLE_SIZE);

/* Chama a funcao escolhida: MULTIPLICACAO ou MEIO_QUADRADO */
int calcular_hash(int chave, int TABLE_SIZE, int funcao);

int eh_primo(int n);
int proximo_primo(int n);

#endif
