#include <stdlib.h>
#include "hash_aberto.h"
#include "../utils/funcoes_hash.h"

/* Estado de cada posicao da tabela */
#define VAZIO    0   /* nunca foi usada: a busca pode parar aqui */
#define OCUPADO  1   /* guarda um aluno */
#define REMOVIDO 2   /* havia um aluno, mas ele foi removido: a busca continua */

struct item {
    int chave;       /* matricula (comparada direto no vetor, sem acessar o aluno) */
    int estado;      /* VAZIO, OCUPADO ou REMOVIDO */
    Aluno aluno;     /* ponteiro para o registro completo */
};

struct hashAberto {
    int qtd;
    int TABLE_SIZE;
    int tipo_funcao;
    struct item *itens;      /* vetor de TABLE_SIZE itens */
    char *indice_usado;      /* indice_usado[i] = 1 se alguma matricula ja teve i como indice inicial */
    int colisoes;
    int sondagens_extras;
};

HashAberto criar_hash_aberto(int TABLE_SIZE, int tipo_funcao){
    HashAberto ha = malloc(sizeof(struct hashAberto));
    if(ha == NULL) return NULL;
    ha->qtd = 0;
    ha->TABLE_SIZE = TABLE_SIZE;
    ha->tipo_funcao = tipo_funcao;
    ha->colisoes = 0;
    ha->sondagens_extras = 0;
    ha->itens = malloc(TABLE_SIZE * sizeof(struct item));
    ha->indice_usado = calloc(TABLE_SIZE, sizeof(char));   /* calloc: tudo 0 */
    if(ha->itens == NULL || ha->indice_usado == NULL){
        free(ha->itens);
        free(ha->indice_usado);
        free(ha);
        return NULL;
    }
    /* todos os estados comecam VAZIO */
    for(int i = 0; i < TABLE_SIZE; i++){
        ha->itens[i].chave = 0;
        ha->itens[i].estado = VAZIO;
        ha->itens[i].aluno = NULL;
    }
    return ha;
}

/* Sondagem linear, como no slide: (pos + i) % TABLE_SIZE */
static int sondagem_linear(int pos, int i, int TABLE_SIZE){
    return (pos + i) % TABLE_SIZE;
}

int inserir_hash_aberto(HashAberto ha, Aluno a){
    if(ha == NULL || ha->qtd == ha->TABLE_SIZE) return 0;   /* tabela cheia */
    int mat = acessar_mat(a);
    int pos = calcular_hash(mat, ha->TABLE_SIZE, ha->tipo_funcao);

    /* colisao (definicao do enunciado): o indice inicial ja foi produzido */
    if(ha->indice_usado[pos]) ha->colisoes++;
    else ha->indice_usado[pos] = 1;

    for(int i = 0; i < ha->TABLE_SIZE; i++){       /* no maximo uma volta */
        int nova = sondagem_linear(pos, i, ha->TABLE_SIZE);
        /* posicao livre: VAZIO ou REMOVIDO (a posicao removida e reutilizada).
           A base tem matriculas unicas, entao nao ha risco de inserir uma repetida. */
        if(ha->itens[nova].estado != OCUPADO){
            ha->itens[nova].chave = mat;
            ha->itens[nova].aluno = a;
            ha->itens[nova].estado = OCUPADO;
            ha->qtd++;
            ha->sondagens_extras += i;               /* i = posicoes ocupadas puladas */
            return 1;
        }
    }
    return 0;
}

Aluno buscar_hash_aberto(HashAberto ha, int matricula){
    if(ha == NULL) return NULL;
    int pos = calcular_hash(matricula, ha->TABLE_SIZE, ha->tipo_funcao);
    for(int i = 0; i < ha->TABLE_SIZE; i++){
        int nova = sondagem_linear(pos, i, ha->TABLE_SIZE);
        if(ha->itens[nova].estado == VAZIO)
            return NULL;                             /* vazia: a matricula nao existe */
        if(ha->itens[nova].estado == OCUPADO && ha->itens[nova].chave == matricula)
            return ha->itens[nova].aluno;            /* encontrada */
        /* REMOVIDO ou outra matricula: continua sondando */
    }
    return NULL;
}

/* Remocao com marcacao especial:
   se a posicao voltasse a ser VAZIO, a busca pararia nela e nao acharia
   as matriculas que colidiram e foram guardadas depois. Com REMOVIDO,
   a busca sabe que "havia um elemento" e continua sondando. */
int remover_hash_aberto(HashAberto ha, int matricula){
    if(ha == NULL || ha->qtd == 0) return 0;
    int pos = calcular_hash(matricula, ha->TABLE_SIZE, ha->tipo_funcao);
    for(int i = 0; i < ha->TABLE_SIZE; i++){
        int nova = sondagem_linear(pos, i, ha->TABLE_SIZE);
        if(ha->itens[nova].estado == VAZIO)
            return 0;                                /* nao existe: nada a remover */
        if(ha->itens[nova].estado == OCUPADO && ha->itens[nova].chave == matricula){
            ha->itens[nova].estado = REMOVIDO;       /* marca, NAO volta para VAZIO */
            ha->itens[nova].aluno = NULL;            /* o aluno e liberado por quem o criou */
            ha->qtd--;
            return 1;
        }
    }
    return 0;
}

void destruir_hash_aberto(HashAberto ha){
    if(ha == NULL) return;
    free(ha->itens);          /* os alunos sao liberados por quem os criou */
    free(ha->indice_usado);
    free(ha);
}

int qtd_hash_aberto(HashAberto ha){ return ha->qtd; }

double fator_carga_hash_aberto(HashAberto ha){
    return (double) ha->qtd / ha->TABLE_SIZE;
}

int colisoes_hash_aberto(HashAberto ha){ return ha->colisoes; }

int sondagens_extras_hash_aberto(HashAberto ha){ return ha->sondagens_extras; }
