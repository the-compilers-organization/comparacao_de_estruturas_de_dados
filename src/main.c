/* =====================================================================
   Projeto 2o GQ - Comparacao de Estruturas de Dados (EDL)
   Compara 5 implementacoes na insercao e na busca por matricula:
     1. Lista unicamente encadeada
     2. Hash, enderecamento aberto (sondagem linear) + multiplicacao
     3. Hash, enderecamento aberto (sondagem linear) + meio-quadrado
     4. Hash, encadeamento separado + multiplicacao
     5. Hash, encadeamento separado + meio-quadrado
   Uso: ./projeto [arquivo] [volume1 volume2 ...]
        sem arquivo: data/alunos.txt
        sem volumes: 100000 1000000 10000000
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

#define NUM_BUSCAS     10000
#define NUM_ESTRUTURAS 5
#define NUM_ETAPAS     3

enum { LISTA, ABERTO_MULT, ABERTO_MEIO, ENCAD_MULT, ENCAD_MEIO };

const char *NOMES[NUM_ESTRUTURAS] = {
    "Lista encadeada",
    "Hash aberto + multiplicacao",
    "Hash aberto + meio-quadrado",
    "Hash encadeado + multiplicacao",
    "Hash encadeado + meio-quadrado"
};
const int PORCENTAGENS[NUM_ETAPAS] = {30, 60, 100};

/* As cinco estruturas de um experimento */
typedef struct {
    Lista lista;
    HashAberto aberto_mult, aberto_meio;
    HashEncadeado encad_mult, encad_meio;
} Estruturas;

/* ---------- gerador aleatorio proprio ----------
   rand() do Windows so vai ate 32767, o que nao alcanca 10 milhoes de
   posicoes. Este gerador (congruencial linear de 64 bits) da a mesma
   sequencia em qualquer computador, com a mesma semente.
   Precisa de unsigned long long: a conta usa numeros de 64 bits, e o
   "estouro" de um unsigned e definido em C (volta a contar do zero),
   o que faz parte do calculo. Com int o estouro e erro. */
static unsigned long long semente = 2026;
static int aleatorio(int limite){            /* numero entre 0 e limite-1 */
    semente = semente * 6364136223846793005ULL + 1442695040888963407ULL;
    return (int) ((semente >> 33) % (unsigned long long) limite);
}

static double milissegundos(clock_t inicio, clock_t fim){
    return (double) (fim - inicio) * 1000.0 / CLOCKS_PER_SEC;
}

/* Insere v[ini..fim-1] em UMA estrutura e devolve o tempo em ms.
   O switch fica fora do laco: so a insercao entra na medicao. */
static double medir_insercao(Estruturas *e, int qual, Aluno *v, int ini, int fim){
    clock_t t0 = clock();
    switch(qual){
        case LISTA:       for(int i = ini; i < fim; i++) inserir_inicio(e->lista, v[i]); break;
        case ABERTO_MULT: for(int i = ini; i < fim; i++) inserir_hash_aberto(e->aberto_mult, v[i]); break;
        case ABERTO_MEIO: for(int i = ini; i < fim; i++) inserir_hash_aberto(e->aberto_meio, v[i]); break;
        case ENCAD_MULT:  for(int i = ini; i < fim; i++) inserir_hash_encadeado(e->encad_mult, v[i]); break;
        case ENCAD_MEIO:  for(int i = ini; i < fim; i++) inserir_hash_encadeado(e->encad_meio, v[i]); break;
    }
    return milissegundos(t0, clock());
}

/* Faz as NUM_BUSCAS buscas e devolve o tempo em ms; *achou = quantas encontrou */
static double medir_busca(Estruturas *e, int qual, int *chaves, int *achou){
    int encontrados = 0;
    clock_t t0 = clock();
    switch(qual){
        case LISTA:       for(int k = 0; k < NUM_BUSCAS; k++) if(buscar_lista(e->lista, chaves[k]) != NULL) encontrados++; break;
        case ABERTO_MULT: for(int k = 0; k < NUM_BUSCAS; k++) if(buscar_hash_aberto(e->aberto_mult, chaves[k]) != NULL) encontrados++; break;
        case ABERTO_MEIO: for(int k = 0; k < NUM_BUSCAS; k++) if(buscar_hash_aberto(e->aberto_meio, chaves[k]) != NULL) encontrados++; break;
        case ENCAD_MULT:  for(int k = 0; k < NUM_BUSCAS; k++) if(buscar_hash_encadeado(e->encad_mult, chaves[k]) != NULL) encontrados++; break;
        case ENCAD_MEIO:  for(int k = 0; k < NUM_BUSCAS; k++) if(buscar_hash_encadeado(e->encad_meio, chaves[k]) != NULL) encontrados++; break;
    }
    double tempo = milissegundos(t0, clock());
    *achou = encontrados;
    return tempo;
}

/* Metricas das tabelas hash (a lista nao tem: devolve -1) */
static int colisoes_de(Estruturas *e, int qual){
    switch(qual){
        case ABERTO_MULT: return colisoes_hash_aberto(e->aberto_mult);
        case ABERTO_MEIO: return colisoes_hash_aberto(e->aberto_meio);
        case ENCAD_MULT:  return colisoes_hash_encadeado(e->encad_mult);
        case ENCAD_MEIO:  return colisoes_hash_encadeado(e->encad_meio);
    }
    return -1;
}
static double fator_carga_de(Estruturas *e, int qual){
    switch(qual){
        case ABERTO_MULT: return fator_carga_hash_aberto(e->aberto_mult);
        case ABERTO_MEIO: return fator_carga_hash_aberto(e->aberto_meio);
        case ENCAD_MULT:  return fator_carga_hash_encadeado(e->encad_mult);
        case ENCAD_MEIO:  return fator_carga_hash_encadeado(e->encad_meio);
    }
    return -1;
}
static int sondagens_de(Estruturas *e, int qual){
    if(qual == ABERTO_MULT) return sondagens_extras_hash_aberto(e->aberto_mult);
    if(qual == ABERTO_MEIO) return sondagens_extras_hash_aberto(e->aberto_meio);
    return -1;
}
static int maior_lista_de(Estruturas *e, int qual){
    if(qual == ENCAD_MULT) return maior_lista_hash_encadeado(e->encad_mult);
    if(qual == ENCAD_MEIO) return maior_lista_hash_encadeado(e->encad_meio);
    return -1;
}

static void experimento(Aluno *alunos, int volume, FILE *csv, FILE *md){
    /* Tamanho da tabela: menor primo >= 2 x volume (alfa final = 0,5).
       Fixo e igual nas quatro tabelas hash. */
    int m = proximo_primo(2 * volume);
    printf("\n========== VOLUME %d registros | TABLE_SIZE = %d ==========\n", volume, m);
    fprintf(md, "\n## Volume: %d registros (TABLE_SIZE = %d)\n", volume, m);

    Estruturas e;
    e.lista       = criar_lista();
    e.aberto_mult = criar_hash_aberto(m, HASH_MULTIPLICACAO);
    e.aberto_meio = criar_hash_aberto(m, HASH_MEIO_QUADRADO);
    e.encad_mult  = criar_hash_encadeado(m, HASH_MULTIPLICACAO);
    e.encad_meio  = criar_hash_encadeado(m, HASH_MEIO_QUADRADO);
    if(!e.lista || !e.aberto_mult || !e.aberto_meio || !e.encad_mult || !e.encad_meio){
        printf("Sem memoria para criar as estruturas.\n");
        exit(1);
    }

    int *chaves = malloc(NUM_BUSCAS * sizeof(int));
    int colisoes_antes[NUM_ESTRUTURAS] = {0};
    int sondagens_antes[NUM_ESTRUTURAS] = {0};
    int ini = 0;

    for(int etapa = 0; etapa < NUM_ETAPAS; etapa++){
        int fim = volume * PORCENTAGENS[etapa] / 100;   /* 10.000.000 x 100 cabe em int */

        /* Sorteia as matriculas ANTES de medir (fora do tempo).
           As mesmas chaves, na mesma ordem, para as cinco estruturas. */
        for(int k = 0; k < NUM_BUSCAS; k++)
            chaves[k] = acessar_mat(alunos[aleatorio(fim)]);

        printf("\n--- Etapa %d%% (registros %d a %d) ---\n", PORCENTAGENS[etapa], ini + 1, fim);
        printf("%-32s %13s %12s %10s %8s\n", "Estrutura", "Insercao(ms)", "Busca(ms)", "Colisoes", "Alfa");
        fprintf(md, "\n### Etapa %d%% (insercao dos registros %d a %d)\n\n", PORCENTAGENS[etapa], ini + 1, fim);
        fprintf(md, "| Estrutura | Insercao (ms) | %d buscas (ms) | Colisoes no intervalo | Fator de carga | Sondagens extras no intervalo | Maior lista |\n", NUM_BUSCAS);
        fprintf(md, "| --- | ---: | ---: | ---: | ---: | ---: | ---: |\n");

        for(int s = 0; s < NUM_ESTRUTURAS; s++){
            int achou;
            double t_ins   = medir_insercao(&e, s, alunos, ini, fim);
            double t_busca = medir_busca(&e, s, chaves, &achou);
            if(achou != NUM_BUSCAS)
                printf("ERRO: %s encontrou so %d de %d matriculas!\n", NOMES[s], achou, NUM_BUSCAS);

            int col = colisoes_de(&e, s);
            int col_intervalo = (col < 0) ? -1 : col - colisoes_antes[s];
            if(col >= 0) colisoes_antes[s] = col;
            int sond = sondagens_de(&e, s);
            int sond_intervalo = (sond < 0) ? -1 : sond - sondagens_antes[s];
            if(sond >= 0) sondagens_antes[s] = sond;
            double alfa = fator_carga_de(&e, s);
            int maior = maior_lista_de(&e, s);

            if(s == LISTA){
                printf("%-32s %13.2f %12.2f %10s %8s\n", NOMES[s], t_ins, t_busca, "-", "-");
                fprintf(md, "| %s | %.2f | %.2f | - | - | - | - |\n", NOMES[s], t_ins, t_busca);
            }else{
                printf("%-32s %13.2f %12.2f %10d %8.3f\n", NOMES[s], t_ins, t_busca, col_intervalo, alfa);
                fprintf(md, "| %s | %.2f | %.2f | %d | %.3f | ", NOMES[s], t_ins, t_busca, col_intervalo, alfa);
                if(sond_intervalo >= 0) fprintf(md, "%d | - |\n", sond_intervalo);
                else fprintf(md, "- | %d |\n", maior);
            }
            fprintf(csv, "%d,%d,%d,%s,%d,%.3f,%.3f,%d,%d,%d,%.4f,%d,%d\n",
                    volume, m, PORCENTAGENS[etapa], NOMES[s], fim, t_ins, t_busca, achou,
                    col_intervalo, col, alfa, sond_intervalo, maior);
            fflush(csv);
        }
        fflush(stdout);
        ini = fim;
    }

    free(chaves);
    destruir_lista(e.lista);
    destruir_hash_aberto(e.aberto_mult);
    destruir_hash_aberto(e.aberto_meio);
    destruir_hash_encadeado(e.encad_mult);
    destruir_hash_encadeado(e.encad_meio);
}

int main(int argc, char *argv[]){
    /* Arquivo: 1o argumento ou, se nao for informado, data/alunos.txt */
    const char *arquivo = (argc >= 2) ? argv[1] : "data/alunos.txt";

    int volumes[16] = {100000, 1000000, 10000000};
    int num_volumes = 3;
    if(argc > 2){
        num_volumes = 0;
        for(int i = 2; i < argc && num_volumes < 16; i++)
            volumes[num_volumes++] = atoi(argv[i]);
    }
    int maior_volume = 0;
    for(int i = 0; i < num_volumes; i++)
        if(volumes[i] > maior_volume) maior_volume = volumes[i];

    /* Leitura do arquivo: fora de qualquer medicao */
    printf("Lendo ate %d registros de %s ...\n", maior_volume, arquivo);
    Aluno *alunos = NULL;
    int total = ler_alunos(arquivo, maior_volume, &alunos);
    if(total < 0){
        printf("Nao foi possivel abrir o arquivo %s.\n", arquivo);
        printf("Coloque a base em data/alunos.txt ou informe o caminho: ./projeto caminho/alunos.txt\n");
        return 1;
    }
    printf("%d registros carregados.\n", total);

    FILE *csv = fopen("resultados.csv", "w");
    FILE *md  = fopen("resultados.md", "w");
    if(csv == NULL || md == NULL){
        printf("Nao foi possivel criar os arquivos de resultado.\n");
        return 1;
    }
    fprintf(csv, "volume,table_size,etapa_pct,estrutura,registros_inseridos,tempo_insercao_ms,tempo_busca_ms,buscas_encontradas,colisoes_intervalo,colisoes_acumuladas,fator_carga,sondagens_extras_intervalo,maior_lista\n");
    fprintf(md, "# Resultados dos experimentos\n\nTempos em milissegundos, medidos com clock(). %d buscas por etapa.\n", NUM_BUSCAS);

    for(int i = 0; i < num_volumes; i++){
        if(volumes[i] > total){
            printf("\nVolume %d ignorado: o arquivo so tem %d registros validos.\n", volumes[i], total);
            fprintf(md, "\n## Volume %d: nao executado (arquivo com apenas %d registros)\n", volumes[i], total);
            continue;
        }
        experimento(alunos, volumes[i], csv, md);
    }

    fclose(csv);
    fclose(md);
    liberar_alunos(alunos, total);
    printf("\nResultados salvos em resultados.csv e resultados.md\n");
    return 0;
}
