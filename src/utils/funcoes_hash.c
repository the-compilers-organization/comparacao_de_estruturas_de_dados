#include "funcoes_hash.h"

/* ------------------------------------------------------------------
   1a funcao: METODO DA MULTIPLICACAO (visto em sala)
   h(k) = parte_inteira( TABLE_SIZE * parte_fracionaria(k * A) )
   A = (raiz(5) - 1) / 2 = 0.6180339887..., sugerido por Knuth.
   Usamos double: com float (24 bits de mantissa), k * A passa de
   2^24 para matriculas grandes e a parte fracionaria vira sempre 0.
   ------------------------------------------------------------------ */
int hash_multiplicacao(int chave, int TABLE_SIZE){
    double A = 0.6180339887498949;
    double val = (chave & 0x7FFFFFFF) * A;   /* & tira o sinal, como em sala */
    val = val - (int) val;                    /* fica so a parte fracionaria (k*A cabe em int) */
    return (int) (TABLE_SIZE * val);          /* resultado entre 0 e TABLE_SIZE-1 */
}

/* ------------------------------------------------------------------
   2a funcao: METODO DO MEIO-QUADRADO (mid-square)
   1. eleva a chave ao quadrado;
   2. descarta os 4 digitos da direita (que variam pouco entre chaves
      parecidas e dependem so dos ultimos digitos da chave);
   3. o resto da divisao por TABLE_SIZE pega os digitos seguintes,
      que sao os "digitos do meio" do quadrado.
   Os digitos do meio dependem de TODOS os digitos da chave.
   long long: 99999999^2 tem 16 digitos e NAO cabe em int (maximo
   2.147.483.647). Com int a conta estoura e o indice sai errado ou
   negativo. Por isso so esta funcao usa long long.
   ------------------------------------------------------------------ */
int hash_meio_quadrado(int chave, int TABLE_SIZE){
    long long k = chave & 0x7FFFFFFF;
    long long quadrado = k * k;
    long long meio = quadrado / 10000;            /* tira os 4 digitos da direita */
    return (int) (meio % TABLE_SIZE);             /* o resto e menor que TABLE_SIZE: cabe em int */
}

int calcular_hash(int chave, int TABLE_SIZE, int tipo){
    if(tipo == HASH_MULTIPLICACAO)
        return hash_multiplicacao(chave, TABLE_SIZE);
    return hash_meio_quadrado(chave, TABLE_SIZE);
}

/* Um primo tem exatamente dois divisores: 1 e ele mesmo.
   Roda uma vez por volume, fora das medicoes de tempo. */
int eh_primo(int n){
    if(n <= 1){
        return 0;
    }

    int i;
    int qtd_div = 0;

    for(i = 1; i <= n; i += 1){
        if(n % i == 0){
            qtd_div += 1;
        }
    }

    if(qtd_div == 2){
        return 1;
    }

    return 0;
}

int proximo_primo(int n){
    while(!eh_primo(n)) n++;
    return n;
}
