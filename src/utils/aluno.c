#include <stdlib.h>
#include <string.h>
#include "aluno.h"

struct aluno{
    int matricula;
    char nome[TAM];
    char curso[TAM];
};

Aluno criar_aluno(int matricula, char nome[TAM], char curso[TAM]){
    if(strlen(nome) >= TAM || strlen(curso) >= TAM){
        return NULL;
    }

    Aluno a = malloc(sizeof(struct aluno));

    if(a != NULL){
        a -> matricula = matricula;
        strcpy(a -> nome, nome);
        strcpy(a -> curso, curso);
    }

    return a;
}


int acessar_mat(Aluno a){
    if(a != NULL){
        return a -> matricula;
    }

    return -1;
}

char *acessar_nome(Aluno a){
    if(a != NULL){
        return a -> nome;
    }

    return NULL;
}


char *acessar_curso(Aluno a){
    if(a != NULL){
        return a -> curso;
    }

    return NULL;
}


void  destruir_aluno(Aluno a){
    if(a != NULL){
        free(a);
    }
}
