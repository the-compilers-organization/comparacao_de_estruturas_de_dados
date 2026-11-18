#include "funcoes_hash.h"

/* 1a funcao: METODO DA MULTIPLICACAO (visto em sala)
   h(k) = parte inteira de ( TABLE_SIZE * parte fracionaria de (k * A) )
   A = (raiz de 5 - 1) / 2 = 0,618..., sugerido por Knuth.
   Usamos double: com float as casas decimais de k * A se perdem
   para matriculas de 8 digitos e todas cairiam no indice 0. */
int hash_multiplicacao(int chave, int TABLE_SIZE){
    double A = 0.6180339887498949;
    double valor = chave * A;
    valor = valor - (int) valor;          /* fica so a parte fracionaria */
    return (int) (TABLE_SIZE * valor);
}

/* 2a funcao: METODO DO MEIO-QUADRADO
   1. eleva a chave ao quadrado;
   2. tira os 4 digitos da direita;
   3. o resto da divisao por TABLE_SIZE pega os digitos do meio.
   long long: o quadrado de uma matricula de 8 digitos tem 16 digitos
   e nao cabe em int (o maior int e 2.147.483.647). */
int hash_meio_quadrado(int chave, int TABLE_SIZE){
    long long quadrado = (long long) chave * chave;
    long long meio = quadrado / 10000;
    return (int) (meio % TABLE_SIZE);
}

int calcular_hash(int chave, int TABLE_SIZE, int funcao){
    if(funcao == MULTIPLICACAO){
        return hash_multiplicacao(chave, TABLE_SIZE);
    }
    return hash_meio_quadrado(chave, TABLE_SIZE);
}

/* Um primo tem exatamente dois divisores: 1 e ele mesmo */
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

/* Menor primo maior ou igual a n (tamanho da tabela hash) */
int proximo_primo(int n){
    while(eh_primo(n) == 0){
        n++;
    }
    return n;
}
