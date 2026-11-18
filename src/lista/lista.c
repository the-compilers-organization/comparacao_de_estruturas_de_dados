#include <stdlib.h>
#include "lista.h"

struct elemento{
    int chave;                 /* matricula */
    Aluno aluno;               /* ponteiro para o aluno */
    struct elemento *prox;
};
typedef struct elemento Elem;

struct lista{                  /* no descritor */
    Elem *inicio;
    int qtd;
};

Lista criar_lista(){
    Lista li = malloc(sizeof(struct lista));
    if(li != NULL){
        li->inicio = NULL;
        li->qtd = 0;
    }
    return li;
}

/* Insere no INICIO: O(1).
   No final seria O(n) a cada insercao, porque teria que achar o ultimo. */
int inserir_inicio(Lista li, Aluno a){
    if(li == NULL){
        return 0;
    }
    Elem *no = malloc(sizeof(Elem));
    if(no == NULL){
        return 0;
    }
    no->chave = acessar_mat(a);
    no->aluno = a;
    no->prox = li->inicio;
    li->inicio = no;
    li->qtd++;
    return 1;
}

/* Busca sequencial: O(n) */
Aluno buscar_lista(Lista li, int matricula){
    if(li == NULL){
        return NULL;
    }
    Elem *aux = li->inicio;
    while(aux != NULL){
        if(aux->chave == matricula){
            return aux->aluno;
        }
        aux = aux->prox;
    }
    return NULL;
}

/* Libera os nos (os alunos sao liberados no main) */
void destruir_lista(Lista li){
    if(li == NULL){
        return;
    }
    Elem *no = li->inicio;
    while(no != NULL){
        Elem *prox = no->prox;   /* guarda o proximo antes do free */
        free(no);
        no = prox;
    }
    free(li);
}
