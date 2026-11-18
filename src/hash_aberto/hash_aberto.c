#include <stdlib.h>
#include "hash_aberto.h"
#include "../utils/funcoes_hash.h"

/* Estado de cada posicao da tabela */
#define VAZIO    0
#define OCUPADO  1
#define REMOVIDO 2

struct item{
    int chave;           /* matricula */
    int estado;          /* VAZIO, OCUPADO ou REMOVIDO */
    Aluno aluno;         /* ponteiro para o aluno */
};

struct hashAberto{
    int qtd;
    int TABLE_SIZE;
    int funcao;          /* MULTIPLICACAO ou MEIO_QUADRADO */
    struct item *itens;  /* vetor com TABLE_SIZE posicoes */
    int *ja_usado;       /* ja_usado[i] = 1 se alguma matricula ja teve i como posicao inicial */
    int colisoes;
};

HashAberto criar_hash_aberto(int TABLE_SIZE, int funcao){
    HashAberto ha = malloc(sizeof(struct hashAberto));
    if(ha == NULL){
        return NULL;
    }
    ha->itens = malloc(TABLE_SIZE * sizeof(struct item));
    ha->ja_usado = malloc(TABLE_SIZE * sizeof(int));
    if(ha->itens == NULL || ha->ja_usado == NULL){
        free(ha->itens);
        free(ha->ja_usado);
        free(ha);
        return NULL;
    }
    ha->qtd = 0;
    ha->TABLE_SIZE = TABLE_SIZE;
    ha->funcao = funcao;
    ha->colisoes = 0;

    int i;
    for(i = 0; i < TABLE_SIZE; i++){
        ha->itens[i].estado = VAZIO;
        ha->itens[i].aluno = NULL;
        ha->ja_usado[i] = 0;
    }
    return ha;
}

int inserir_hash_aberto(HashAberto ha, Aluno a){
    if(ha == NULL || ha->qtd == ha->TABLE_SIZE){
        return 0;                                    /* tabela cheia */
    }
    int mat = acessar_mat(a);
    int pos = calcular_hash(mat, ha->TABLE_SIZE, ha->funcao);

    /* Colisao (definicao do enunciado): a posicao inicial ja apareceu antes */
    if(ha->ja_usado[pos] == 1){
        ha->colisoes++;
    }else{
        ha->ja_usado[pos] = 1;
    }

    /* Sondagem linear: testa pos, pos+1, pos+2... ate achar uma posicao livre */
    int i;
    for(i = 0; i < ha->TABLE_SIZE; i++){
        int nova = (pos + i) % ha->TABLE_SIZE;
        if(ha->itens[nova].estado != OCUPADO){      /* VAZIO ou REMOVIDO */
            ha->itens[nova].chave = mat;
            ha->itens[nova].aluno = a;
            ha->itens[nova].estado = OCUPADO;
            ha->qtd++;
            return 1;
        }
    }
    return 0;
}

Aluno buscar_hash_aberto(HashAberto ha, int matricula){
    if(ha == NULL){
        return NULL;
    }
    int pos = calcular_hash(matricula, ha->TABLE_SIZE, ha->funcao);
    int i;
    for(i = 0; i < ha->TABLE_SIZE; i++){
        int nova = (pos + i) % ha->TABLE_SIZE;
        if(ha->itens[nova].estado == VAZIO){
            return NULL;                             /* nao existe */
        }
        if(ha->itens[nova].estado == OCUPADO && ha->itens[nova].chave == matricula){
            return ha->itens[nova].aluno;            /* encontrou */
        }
        /* REMOVIDO ou outra matricula: continua procurando */
    }
    return NULL;
}

/* Remocao: a posicao vira REMOVIDO, e nao VAZIO.
   Se virasse VAZIO, a busca pararia ali e nao acharia
   as matriculas que colidiram e foram guardadas depois. */
int remover_hash_aberto(HashAberto ha, int matricula){
    if(ha == NULL){
        return 0;
    }
    int pos = calcular_hash(matricula, ha->TABLE_SIZE, ha->funcao);
    int i;
    for(i = 0; i < ha->TABLE_SIZE; i++){
        int nova = (pos + i) % ha->TABLE_SIZE;
        if(ha->itens[nova].estado == VAZIO){
            return 0;                                /* nao existe */
        }
        if(ha->itens[nova].estado == OCUPADO && ha->itens[nova].chave == matricula){
            ha->itens[nova].estado = REMOVIDO;
            ha->itens[nova].aluno = NULL;
            ha->qtd--;
            return 1;
        }
    }
    return 0;
}

void destruir_hash_aberto(HashAberto ha){
    if(ha == NULL){
        return;
    }
    free(ha->itens);
    free(ha->ja_usado);
    free(ha);
}

int colisoes_hash_aberto(HashAberto ha){
    return ha->colisoes;
}

double fator_carga_hash_aberto(HashAberto ha){
    return (double) ha->qtd / ha->TABLE_SIZE;
}
