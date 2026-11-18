#ifndef LEITURA_H
#define LEITURA_H
#include "aluno.h"

/* Le ate "max" linhas do arquivo (matricula;nome;curso), NA ORDEM do
   arquivo, e guarda um Aluno em cada posicao do vetor "alunos".
   Devolve quantos alunos foram lidos (0 se nao abrir o arquivo). */
int ler_alunos(char *arquivo, Aluno alunos[], int max);

#endif
