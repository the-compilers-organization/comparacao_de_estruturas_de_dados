#ifndef LEITURA_H
#define LEITURA_H
#include "aluno.h"

/* Le ate max_registros linhas "matricula;nome;curso" do arquivo,
   NA ORDEM do arquivo, e cria um Aluno para cada uma.
   Devolve quantos registros foram lidos (ou -1 se nao abrir o arquivo).
   *vetor recebe um vetor alocado dinamicamente com os alunos. */
int ler_alunos(const char *caminho, int max_registros, Aluno **vetor);

/* Libera os alunos e o vetor */
void liberar_alunos(Aluno *vetor, int n);

#endif
