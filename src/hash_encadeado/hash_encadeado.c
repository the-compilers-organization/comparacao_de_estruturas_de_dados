#include <stdlib.h>
#include "hash_encadeado.h"
#include "../utils/funcoes_hash.h"

struct no{
    int chave;           /* matricula */
    Aluno aluno;         /* ponteiro para o aluno */
    struct no *prox;
};
typedef struct no No;

struct hashEncadeado{
    int qtd;
    int TABLE_SIZE;
    int funcao;          /* MULTIPLICACAO ou MEIO_QUADRADO */
    No **tabela;         /* tabela[pos] = inicio da lista daquela posicao */
    int colisoes;
};

HashEncadeado criar_hash_encadeado(int TABLE_SIZE, int funcao){
    HashEncadeado ha = malloc(sizeof(struct hashEncadeado));
    if(ha == NULL){
        return NULL;
    }
    ha->tabela = malloc(TABLE_SIZE * sizeof(No*));
    if(ha->tabela == NULL){
        free(ha);
        return NULL;
    }
    ha->qtd = 0;
    ha->TABLE_SIZE = TABLE_SIZE;
    ha->funcao = funcao;
    ha->colisoes = 0;

    int i;
    for(i = 0; i < TABLE_SIZE; i++){
        ha->tabela[i] = NULL;        /* todas as listas comecam vazias */
    }
    return ha;
}

/* Insere no INICIO da lista da posicao: O(1).
   tabela[pos] faz o papel do "inicio" de uma lista com descritor. */
int inserir_hash_encadeado(HashEncadeado ha, Aluno a){
    if(ha == NULL){
        return 0;
    }
    int mat = acessar_mat(a);
    int pos = calcular_hash(mat, ha->TABLE_SIZE, ha->funcao);

    No *novo = malloc(sizeof(No));
    if(novo == NULL){
        return 0;
    }

    /* Colisao: a lista ja tinha alguem = a posicao ja tinha aparecido antes */
    if(ha->tabela[pos] != NULL){
        ha->colisoes++;
    }

    novo->chave = mat;
    novo->aluno = a;
    novo->prox = ha->tabela[pos];    /* o novo aponta para o antigo inicio */
    ha->tabela[pos] = novo;          /* o novo vira o inicio */
    ha->qtd++;
    return 1;
}

/* Percorre so a lista daquela posicao */
Aluno buscar_hash_encadeado(HashEncadeado ha, int matricula){
    if(ha == NULL){
        return NULL;
    }
    int pos = calcular_hash(matricula, ha->TABLE_SIZE, ha->funcao);
    No *aux = ha->tabela[pos];
    while(aux != NULL){
        if(aux->chave == matricula){
            return aux->aluno;
        }
        aux = aux->prox;
    }
    return NULL;
}

/* Remocao: tira o no da lista, sem marca.
   "anterior" acompanha o no de tras de "atual". */
int remover_hash_encadeado(HashEncadeado ha, int matricula){
    if(ha == NULL){
        return 0;
    }
    int pos = calcular_hash(matricula, ha->TABLE_SIZE, ha->funcao);
    No *atual = ha->tabela[pos];
    No *anterior = NULL;

    while(atual != NULL){
        if(atual->chave == matricula){
            if(anterior == NULL){
                ha->tabela[pos] = atual->prox;   /* era o primeiro da lista */
            }else{
                anterior->prox = atual->prox;    /* pula o no removido */
            }
            free(atual);
            ha->qtd--;
            return 1;
        }
        anterior = atual;
        atual = atual->prox;
    }
    return 0;
}

void destruir_hash_encadeado(HashEncadeado ha){
    if(ha == NULL){
        return;
    }
    int i;
    for(i = 0; i < ha->TABLE_SIZE; i++){
        No *aux = ha->tabela[i];
        while(aux != NULL){
            No *prox = aux->prox;
            free(aux);
            aux = prox;
        }
    }
    free(ha->tabela);
    free(ha);
}

int colisoes_hash_encadeado(HashEncadeado ha){
    return ha->colisoes;
}

double fator_carga_hash_encadeado(HashEncadeado ha){
    return (double) ha->qtd / ha->TABLE_SIZE;
}
