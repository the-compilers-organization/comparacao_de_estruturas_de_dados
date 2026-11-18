/* =====================================================================
   Projeto 2o GQ - Comparacao de Estruturas de Dados (EDL)

   Compara 5 estruturas na insercao e na busca por matricula:
     1. Lista unicamente encadeada
     2. Hash com enderecamento aberto + multiplicacao
     3. Hash com enderecamento aberto + meio-quadrado
     4. Hash com encadeamento separado + multiplicacao
     5. Hash com encadeamento separado + meio-quadrado

   Execute a partir da pasta raiz do projeto (onde fica a pasta data).
   ===================================================================== */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils/aluno.h"
#include "utils/leitura.h"
#include "utils/funcoes_hash.h"
#include "lista/lista.h"
#include "hash_aberto/hash_aberto.h"
#include "hash_encadeado/hash_encadeado.h"

#define ARQUIVO        "data/alunos.txt"
#define RESULTADOS     "resultados/resultados.csv"
#define MAX_REGISTROS  10000000
#define NUM_BUSCAS     10000

/* ---------- funcoes auxiliares ---------- */

/* Numero aleatorio entre 0 e limite-1.
   rand() pode ir so ate 32767 (no Windows), entao juntamos dois sorteios
   para alcancar posicoes na casa dos milhoes. */
int sortear(int limite){
    int numero = (rand() % 32768) * 32768 + (rand() % 32768);
    return numero % limite;
}

/* Converte a diferenca de dois clock() em milissegundos */
double milissegundos(clock_t inicio, clock_t fim){
    return (double) (fim - inicio) * 1000.0 / CLOCKS_PER_SEC;
}

/* Mostra uma linha na tela e grava a mesma linha no arquivo CSV.
   Para a lista, colisoes = -1 e alfa = -1 (nao se aplicam). */
void mostrar(FILE *csv, int volume, int etapa, char *estrutura,
             double t_insercao, double t_busca, int encontradas,
             int colisoes, double alfa){
    if(colisoes < 0){
        printf("%-32s %12.2f %12.2f %10s %6s\n", estrutura, t_insercao, t_busca, "-", "-");
    }else{
        printf("%-32s %12.2f %12.2f %10d %6.2f\n", estrutura, t_insercao, t_busca, colisoes, alfa);
    }
    fprintf(csv, "%d,%d,%s,%.3f,%.3f,%d,%d,%.4f\n",
            volume, etapa, estrutura, t_insercao, t_busca, encontradas, colisoes, alfa);

    if(encontradas != NUM_BUSCAS){
        printf("ERRO: %s encontrou so %d de %d matriculas!\n", estrutura, encontradas, NUM_BUSCAS);
    }
}

/* ---------- insercao e busca de cada estrutura ----------
   Cada "inserir_..." insere os alunos de inicio ate fim-1 e devolve o tempo.
   Cada "buscar_..." busca as NUM_BUSCAS matriculas e devolve o tempo;
   em *encontradas fica quantas foram achadas. */

double inserir_na_lista(Lista li, Aluno alunos[], int inicio, int fim){
    clock_t t0 = clock();
    int i;
    for(i = inicio; i < fim; i++){
        inserir_inicio(li, alunos[i]);
    }
    return milissegundos(t0, clock());
}

double buscar_na_lista(Lista li, int chaves[], int *encontradas){
    *encontradas = 0;
    clock_t t0 = clock();
    int k;
    for(k = 0; k < NUM_BUSCAS; k++){
        if(buscar_lista(li, chaves[k]) != NULL){
            *encontradas = *encontradas + 1;
        }
    }
    return milissegundos(t0, clock());
}

double inserir_no_aberto(HashAberto ha, Aluno alunos[], int inicio, int fim){
    clock_t t0 = clock();
    int i;
    for(i = inicio; i < fim; i++){
        inserir_hash_aberto(ha, alunos[i]);
    }
    return milissegundos(t0, clock());
}

double buscar_no_aberto(HashAberto ha, int chaves[], int *encontradas){
    *encontradas = 0;
    clock_t t0 = clock();
    int k;
    for(k = 0; k < NUM_BUSCAS; k++){
        if(buscar_hash_aberto(ha, chaves[k]) != NULL){
            *encontradas = *encontradas + 1;
        }
    }
    return milissegundos(t0, clock());
}

double inserir_no_encadeado(HashEncadeado ha, Aluno alunos[], int inicio, int fim){
    clock_t t0 = clock();
    int i;
    for(i = inicio; i < fim; i++){
        inserir_hash_encadeado(ha, alunos[i]);
    }
    return milissegundos(t0, clock());
}

double buscar_no_encadeado(HashEncadeado ha, int chaves[], int *encontradas){
    *encontradas = 0;
    clock_t t0 = clock();
    int k;
    for(k = 0; k < NUM_BUSCAS; k++){
        if(buscar_hash_encadeado(ha, chaves[k]) != NULL){
            *encontradas = *encontradas + 1;
        }
    }
    return milissegundos(t0, clock());
}

/* ---------- experimento de um volume ---------- */

void experimento(Aluno alunos[], int volume, FILE *csv){
    /* Tamanho da tabela: menor primo >= 2 x volume (tabela termina meio cheia) */
    int m = proximo_primo(2 * volume);
    printf("\n========== VOLUME: %d registros | TABLE_SIZE = %d ==========\n", volume, m);

    /* As cinco estruturas comecam vazias */
    Lista lista = criar_lista();
    HashAberto aberto_mult = criar_hash_aberto(m, MULTIPLICACAO);
    HashAberto aberto_meio = criar_hash_aberto(m, MEIO_QUADRADO);
    HashEncadeado encad_mult = criar_hash_encadeado(m, MULTIPLICACAO);
    HashEncadeado encad_meio = criar_hash_encadeado(m, MEIO_QUADRADO);

    if(lista == NULL || aberto_mult == NULL || aberto_meio == NULL ||
       encad_mult == NULL || encad_meio == NULL){
        printf("Sem memoria para criar as estruturas.\n");
        exit(1);
    }

    int porcentagens[3] = {30, 60, 100};
    int chaves[NUM_BUSCAS];
    int inicio = 0;

    /* Colisoes ate a etapa anterior (para contar so as do intervalo) */
    int antes_aberto_mult = 0, antes_aberto_meio = 0;
    int antes_encad_mult = 0, antes_encad_meio = 0;

    int etapa;
    for(etapa = 0; etapa < 3; etapa++){
        int fim = volume / 100 * porcentagens[etapa];

        /* Sorteia as matriculas ANTES de medir o tempo.
           As mesmas matriculas sao buscadas nas cinco estruturas. */
        int k;
        for(k = 0; k < NUM_BUSCAS; k++){
            chaves[k] = acessar_mat(alunos[sortear(fim)]);
        }

        printf("\n--- Etapa %d%%: registros %d a %d ---\n", porcentagens[etapa], inicio + 1, fim);
        printf("%-32s %12s %12s %10s %6s\n", "Estrutura", "Insercao(ms)", "Busca(ms)", "Colisoes", "Alfa");

        double t_ins, t_busca;
        int encontradas, colisoes;

        /* 1. Lista encadeada */
        t_ins = inserir_na_lista(lista, alunos, inicio, fim);
        t_busca = buscar_na_lista(lista, chaves, &encontradas);
        mostrar(csv, volume, porcentagens[etapa], "Lista encadeada",
                t_ins, t_busca, encontradas, -1, -1);

        /* 2. Hash aberto + multiplicacao */
        t_ins = inserir_no_aberto(aberto_mult, alunos, inicio, fim);
        t_busca = buscar_no_aberto(aberto_mult, chaves, &encontradas);
        colisoes = colisoes_hash_aberto(aberto_mult);
        mostrar(csv, volume, porcentagens[etapa], "Hash aberto + multiplicacao",
                t_ins, t_busca, encontradas, colisoes - antes_aberto_mult,
                fator_carga_hash_aberto(aberto_mult));
        antes_aberto_mult = colisoes;

        /* 3. Hash aberto + meio-quadrado */
        t_ins = inserir_no_aberto(aberto_meio, alunos, inicio, fim);
        t_busca = buscar_no_aberto(aberto_meio, chaves, &encontradas);
        colisoes = colisoes_hash_aberto(aberto_meio);
        mostrar(csv, volume, porcentagens[etapa], "Hash aberto + meio-quadrado",
                t_ins, t_busca, encontradas, colisoes - antes_aberto_meio,
                fator_carga_hash_aberto(aberto_meio));
        antes_aberto_meio = colisoes;

        /* 4. Hash encadeado + multiplicacao */
        t_ins = inserir_no_encadeado(encad_mult, alunos, inicio, fim);
        t_busca = buscar_no_encadeado(encad_mult, chaves, &encontradas);
        colisoes = colisoes_hash_encadeado(encad_mult);
        mostrar(csv, volume, porcentagens[etapa], "Hash encadeado + multiplicacao",
                t_ins, t_busca, encontradas, colisoes - antes_encad_mult,
                fator_carga_hash_encadeado(encad_mult));
        antes_encad_mult = colisoes;

        /* 5. Hash encadeado + meio-quadrado */
        t_ins = inserir_no_encadeado(encad_meio, alunos, inicio, fim);
        t_busca = buscar_no_encadeado(encad_meio, chaves, &encontradas);
        colisoes = colisoes_hash_encadeado(encad_meio);
        mostrar(csv, volume, porcentagens[etapa], "Hash encadeado + meio-quadrado",
                t_ins, t_busca, encontradas, colisoes - antes_encad_meio,
                fator_carga_hash_encadeado(encad_meio));
        antes_encad_meio = colisoes;

        inicio = fim;
    }

    destruir_lista(lista);
    destruir_hash_aberto(aberto_mult);
    destruir_hash_aberto(aberto_meio);
    destruir_hash_encadeado(encad_mult);
    destruir_hash_encadeado(encad_meio);
}

int main(){
    int volumes[3] = {100000, 1000000, 10000000};

    /* 1. Le a base (fora de qualquer medicao de tempo) */
    Aluno *alunos = malloc(MAX_REGISTROS * sizeof(Aluno));
    if(alunos == NULL){
        printf("Sem memoria para o vetor de alunos.\n");
        return 1;
    }
    printf("Lendo %s ...\n", ARQUIVO);
    int total = ler_alunos(ARQUIVO, alunos, MAX_REGISTROS);
    if(total == 0){
        printf("Nao foi possivel ler %s.\n", ARQUIVO);
        printf("Execute o programa a partir da pasta raiz do projeto.\n");
        free(alunos);
        return 1;
    }
    printf("%d alunos carregados.\n", total);

    /* 2. Abre o arquivo de resultados */
    FILE *csv = fopen(RESULTADOS, "w");
    if(csv == NULL){
        printf("Nao foi possivel criar %s.\n", RESULTADOS);
        return 1;
    }
    fprintf(csv, "volume,etapa_pct,estrutura,tempo_insercao_ms,tempo_busca_ms,encontradas,colisoes_intervalo,fator_carga\n");

    /* 3. Roda os experimentos (semente fixa: o sorteio se repete a cada execucao) */
    srand(2026);
    int v;
    for(v = 0; v < 3; v++){
        if(volumes[v] > total){
            printf("\nVolume %d ignorado: o arquivo so tem %d alunos.\n", volumes[v], total);
            continue;
        }
        experimento(alunos, volumes[v], csv);
    }

    /* 4. Libera tudo */
    fclose(csv);
    int i;
    for(i = 0; i < total; i++){
        destruir_aluno(alunos[i]);
    }
    free(alunos);

    printf("\nResultados salvos em %s\n", RESULTADOS);
    return 0;
}
