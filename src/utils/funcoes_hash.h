#ifndef FUNCOES_HASH_H
#define FUNCOES_HASH_H

/* Qual funcao de hashing a tabela usa */
#define HASH_MULTIPLICACAO 0
#define HASH_MEIO_QUADRADO 1

int hash_multiplicacao(int chave, int TABLE_SIZE);
int hash_meio_quadrado(int chave, int TABLE_SIZE);

/* Chama a funcao escolhida (tipo = HASH_MULTIPLICACAO ou HASH_MEIO_QUADRADO) */
int calcular_hash(int chave, int TABLE_SIZE, int tipo);

/* 1 se n e primo, 0 se nao e */
int eh_primo(int n);

/* Menor numero primo maior ou igual a n (usado para escolher o tamanho da tabela) */
int proximo_primo(int n);

#endif
