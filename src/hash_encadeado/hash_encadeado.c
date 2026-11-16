#include <stdlib.h>
#include "hash_encadeado.h"
#include "../utils/funcoes_hash.h"

typedef struct no{
    int chave;               /* matricula (comparada sem acessar o aluno) */
    Aluno aluno;             /* ponteiro para o registro completo */
    struct no* prox;
} No;

struct hashEncadeado {
    int qtd;
    int TABLE_SIZE;
    int tipo_funcao;
    No **tabela;             /* cada posicao = inicio de uma lista encadeada */
    int colisoes;
};

HashEncadeado criar_hash_encadeado(int TABLE_SIZE, int tipo_funcao){
    HashEncadeado ha = malloc(sizeof(struct hashEncadeado));
    if(ha == NULL) return NULL;
    ha->qtd = 0;
    ha->TABLE_SIZE = TABLE_SIZE;
    ha->tipo_funcao = tipo_funcao;
    ha->colisoes = 0;
    ha->tabela = calloc(TABLE_SIZE, sizeof(No*));   /* calloc: todas as listas vazias */
    if(ha->tabela == NULL){
        free(ha);
        return NULL;
    }
    return ha;
}

/* Insere no INICIO da lista da posicao, como no slide: O(1) */
int inserir_hash_encadeado(HashEncadeado ha, Aluno a){
    if(ha == NULL) return 0;
    int mat = acessar_mat(a);
    int pos = calcular_hash(mat, ha->TABLE_SIZE, ha->tipo_funcao);
    No* novo = malloc(sizeof(No));
    if(novo == NULL) return 0;

    /* Sem remocoes, lista nao vazia = indice ja produzido por outra matricula */
    if(ha->tabela[pos] != NULL) ha->colisoes++;

    novo->chave = mat;
    novo->aluno = a;
    novo->prox = ha->tabela[pos];
    ha->tabela[pos] = novo;
    ha->qtd++;
    return 1;
}

/* Percorre so a lista da posicao: O(tamanho dessa lista) */
Aluno buscar_hash_encadeado(HashEncadeado ha, int matricula){
    if(ha == NULL) return NULL;
    int pos = calcular_hash(matricula, ha->TABLE_SIZE, ha->tipo_funcao);
    No* aux = ha->tabela[pos];
    while(aux != NULL){
        if(aux->chave == matricula)
            return aux->aluno;
        aux = aux->prox;
    }
    return NULL;
}

/* Remocao no encadeamento separado: nao precisa de marca.
   "atual" percorre a lista da posicao e "anterior" acompanha o no de tras.
   Ao achar a matricula, refaz o encadeamento ANTES do free(atual). */
int remover_hash_encadeado(HashEncadeado ha, int matricula){
    if(ha == NULL) return 0;
    int pos = calcular_hash(matricula, ha->TABLE_SIZE, ha->tipo_funcao);
    No* atual = ha->tabela[pos];
    No* anterior = NULL;

    while(atual != NULL){
        if(atual->chave == matricula){
            if(anterior == NULL)
                ha->tabela[pos] = atual->prox;   /* era o primeiro da lista */
            else
                anterior->prox = atual->prox;    /* pula o no removido */
            free(atual);                         /* libera so o no, nao o aluno */
            ha->qtd--;
            return 1;
        }
        anterior = atual;
        atual = atual->prox;
    }
    return 0;                                    /* nao encontrou */
}

void destruir_hash_encadeado(HashEncadeado ha){
    if(ha == NULL) return;
    for(int i = 0; i < ha->TABLE_SIZE; i++){
        No* aux = ha->tabela[i];
        while(aux != NULL){
            No* prox = aux->prox;
            free(aux);
            aux = prox;
        }
    }
    free(ha->tabela);
    free(ha);
}

int qtd_hash_encadeado(HashEncadeado ha){ return ha->qtd; }

double fator_carga_hash_encadeado(HashEncadeado ha){
    return (double) ha->qtd / ha->TABLE_SIZE;
}

int colisoes_hash_encadeado(HashEncadeado ha){ return ha->colisoes; }

int maior_lista_hash_encadeado(HashEncadeado ha){
    int maior = 0;
    for(int i = 0; i < ha->TABLE_SIZE; i++){
        int tam = 0;
        for(No* aux = ha->tabela[i]; aux != NULL; aux = aux->prox) tam++;
        if(tam > maior) maior = tam;
    }
    return maior;
}
