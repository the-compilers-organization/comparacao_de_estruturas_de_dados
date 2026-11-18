#ifndef SAIDA_H
#define SAIDA_H

/* Saida do programa: tudo o que e escrito com escrever() aparece na tela
   E e gravado no arquivo aberto com abrir_saida(), sem precisar copiar
   e colar do terminal. */

/* Abre (ou recria) o arquivo de saida. Devolve 1 = sucesso, 0 = falha.
   Se falhar, escrever() continua mostrando na tela. */
int  abrir_saida(char *arquivo);

/* Igual ao printf: escrever("%d alunos\n", n); */
void escrever(const char *formato, ...);

void fechar_saida();

#endif
