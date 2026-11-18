#ifndef ALUNO_H
#define ALUNO_H

#define TAM 50

typedef struct aluno *Aluno;

Aluno criar_aluno(int matricula, char nome[TAM], char curso[TAM]);

int acessar_mat(Aluno a);

char *acessar_nome(Aluno a);

char *acessar_curso(Aluno a);

void  destruir_aluno(Aluno a);

#endif
