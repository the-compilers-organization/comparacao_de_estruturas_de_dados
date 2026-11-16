# Projeto 2º GQ: Comparação de Estruturas de Dados

Disciplina: Estrutura de Dados Lineares (INF1923), UNICAP, 2026.2.

Integrantes: _(preencher)_

Este projeto compara cinco implementações em C nas operações de **inserção** e **busca por matrícula**, usando a base do Cadastro Nacional de Estudantes (`matricula;nome;curso`):

| # | Estrutura | Tratamento de colisão | Função hash |
| --- | --- | --- | --- |
| 1 | Lista unicamente encadeada | (não se aplica) | (não se aplica) |
| 2 | Tabela hash | Endereçamento aberto, sondagem linear | Multiplicação |
| 3 | Tabela hash | Endereçamento aberto, sondagem linear | Meio-quadrado |
| 4 | Tabela hash | Encadeamento separado | Multiplicação |
| 5 | Tabela hash | Encadeamento separado | Meio-quadrado |

## Organização do código (TAD)

Cada estrutura é um Tipo Abstrato de Dados: um `.h` com o tipo opaco (`typedef struct x* X`) e as operações, e um `.c` com a implementação. Toda a memória é alocada dinamicamente.

```
projeto-hash-edl/
├── data/                      base de dados (alunos.txt)
├── resultados/                resultados dos experimentos documentados
├── src/
│   ├── lista/                 lista unicamente encadeada
│   │   ├── lista.h
│   │   └── lista.c
│   ├── hash_aberto/           tabela hash com endereçamento aberto
│   │   ├── hash_aberto.h
│   │   └── hash_aberto.c
│   ├── hash_encadeado/        tabela hash com encadeamento separado
│   │   ├── hash_encadeado.h
│   │   └── hash_encadeado.c
│   ├── utils/                 módulos de apoio, usados pelas estruturas e pelo main
│   │   ├── aluno.h
│   │   ├── aluno.c
│   │   ├── funcoes_hash.h
│   │   ├── funcoes_hash.c
│   │   ├── leitura.h
│   │   └── leitura.c
│   └── main.c                 experimentos
|
└── README.md
```

Cada estrutura tem sua pasta em `src/`. Os módulos de apoio ficam em `src/utils/`, e as pastas das estruturas os incluem com `#include "../utils/aluno.h"`. O `main.c` fica na raiz de `src/` e inclui tudo pelo caminho da pasta, como `#include "lista/lista.h"`.

| Módulo | Conteúdo |
| --- | --- |
| `src/lista/` | TAD `Lista`: lista unicamente encadeada com nó descritor (`inicio`, `qtd`) |
| `src/hash_aberto/` | TAD `HashAberto`: endereçamento aberto com sondagem linear |
| `src/hash_encadeado/` | TAD `HashEncadeado`: encadeamento separado |
| `src/utils/aluno.h/.c` | TAD `Aluno`: matrícula, nome e curso |
| `src/utils/funcoes_hash.h/.c` | Métodos da multiplicação e do meio-quadrado; `proximo_primo` |
| `src/utils/leitura.h/.c` | Leitura do arquivo na ordem original |
| `src/main.c` | Experimentos: etapas, medições, arquivos de resultado |
| `testes/teste_remocao.c` | Teste da remoção nas duas tabelas hash (34 verificações) |

Convenção de retorno: as funções de inserção devolvem **1 = sucesso, 0 = falha** (como no `1_tabela_hash.c` da disciplina). As buscas devolvem o `Aluno` encontrado ou `NULL`.

## Como compilar e executar

Requisito: `gcc`. Baixe a base do Teams da disciplina e salve como **`data/alunos.txt`**. **A base não vai para o repositório**: ela tem mais de 300 MB e o GitHub recusa arquivos acima de 100 MB. O `.gitignore` envia só a pasta `data/` vazia (com um `.gitkeep`).

Os comandos abaixo são executados **na pasta raiz do projeto** (onde está o `README.md`).

**Compilar** (o mesmo comando no Linux, no macOS e no Windows; no Windows, troque `-o projeto` por `-o projeto.exe`):

```bash
gcc -Wall -Wextra -O2 -std=c99 src/main.c src/utils/aluno.c src/utils/leitura.c src/utils/funcoes_hash.c src/lista/lista.c src/hash_aberto/hash_aberto.c src/hash_encadeado/hash_encadeado.c -o projeto
```

O comando lista todos os `.c` do projeto: os da raiz de `src/` e um de cada pasta de estrutura. Os `.h` não entram no comando, porque são incluídos pelos próprios `.c` com `#include`.

No Windows, também dá para compilar com dois cliques em `compilar.bat`, que executa esse mesmo comando e gera o `projeto.exe`.

**Executar:**

```bash
./projeto                                     # lê data/alunos.txt; volumes 100000 1000000 10000000
./projeto data/alunos.txt 100000 1000000      # escolhendo os volumes
./projeto outra_pasta/base.txt                # outro arquivo
```

No Windows, use `projeto.exe` no lugar de `./projeto`.

**Teste da remoção** nas duas tabelas hash:

```bash
gcc -Wall -Wextra -std=c99 testes/teste_remocao.c src/utils/aluno.c src/utils/funcoes_hash.c src/hash_aberto/hash_aberto.c src/hash_encadeado/hash_encadeado.c -o teste_remocao
./teste_remocao
```

O teste tem duas partes, rodadas com as duas funções hash. No endereçamento aberto, ele reproduz o slide 43: encontra matrículas que caem no mesmo índice inicial (B, F, G e A), insere as quatro, remove G e confere que A continua sendo encontrado; depois insere uma quinta matrícula (H) e confere que ela reutiliza a posição `REMOVIDO`. No encadeamento, coloca três matrículas na mesma lista e remove a do meio, a primeira e a única que sobrou. Ao final, mostra quantos testes passaram (34 de 34).

Para escolher os volumes, informe também o arquivo, porque o primeiro argumento é sempre o caminho da base: `./projeto data/alunos.txt 100000`.

O programa grava dois arquivos na pasta onde foi executado:

- `resultados.csv`: uma linha por estrutura e etapa (para gerar gráficos em planilha).
- `resultados.md`: as mesmas medições já formatadas em tabelas Markdown.

Se o arquivo tiver menos registros que um volume pedido, aquele volume é pulado com um aviso. Linhas incompletas são ignoradas e contadas.

**Memória:** com 10 milhões de registros, os registros e as cinco estruturas juntos ocupam cerca de 3,3 GB de RAM (cada aluno ocupa 104 bytes com `TAM 50`, e cada posição do endereçamento aberto, 16 bytes). Feche outros programas antes de rodar.

**Tempo:** a lista encadeada domina o tempo total, porque cada uma das 10.000 buscas percorre, em média, metade da lista. No ambiente de teste, os volumes de 100 mil, 1 milhão e 8 milhões levaram 4 minutos no total; com 10 milhões, conte com 6 a 8 minutos.

## Como o experimento funciona

Para cada volume (100 mil, 1 milhão e 10 milhões):

1. **Tamanho da tabela:** `m = proximo_primo(2 × volume)`, ou seja 200.003, 2.000.003 e 20.000.003. É o mesmo nas quatro tabelas hash e fica fixo durante o experimento.
2. **Cinco estruturas vazias** são criadas.
3. **Três etapas de inserção**, preservando a ordem do arquivo: registros de 0 a 30%, de 30% a 60% e de 60% a 100%. O tempo de cada intervalo é medido separadamente, para cada estrutura.
4. **Após cada etapa:** 10.000 matrículas são sorteadas entre os registros já inseridos. As **mesmas** matrículas, na **mesma** ordem, são buscadas nas cinco estruturas, e o programa confere se todas foram encontradas.
5. **Métricas das tabelas hash, por etapa:** colisões no intervalo, fator de carga α = n/m e, como métricas extras, as sondagens extras (endereçamento aberto) e a maior lista (encadeamento).

Ficam **fora** das medições a leitura do arquivo, o sorteio das matrículas e o cálculo das métricas extras.

## Decisões de projeto e justificativas

### 1. Lista unicamente encadeada com inserção no início

A lista tem nó descritor com `inicio` e `qtd`, como nas aulas. A inserção é feita no **início**, em O(1). Inserir no final custaria O(n) por inserção, porque o descritor não guarda o último nó, e O(n²) para carregar a base inteira: com 10 milhões de registros, seriam cerca de 5 × 10¹³ passos. Isso não muda o resultado da busca, que é sequencial e O(n) em qualquer caso.

A base não tem matrículas repetidas (verificamos isso no arquivo), por isso a inserção não procura duplicatas. Procurar duplicatas tornaria a inserção na lista O(n).

### 2. Tamanho da tabela: primo ≥ 2 × volume (α final = 0,5)

- **Por que um primo:** é a recomendação vista em sala para quebrar padrões aritméticos das chaves. No meio-quadrado, o resto da divisão por `m` é a etapa final, então o primo também ajuda ali.
- **Por que 2 × volume:** no endereçamento aberto, o número de posições testadas por busca depende principalmente da fração da tabela ocupada e cresce muito quando a tabela se aproxima de cheia. Isso foi observado já no trabalho que introduziu o endereçamento aberto (PETERSON, 1957). Pela análise clássica da sondagem linear (KNUTH, 1998), com α = 0,5 uma busca bem-sucedida testa em média ½(1 + 1/(1 − α)) = **1,5** posições, e uma inserção testa ½(1 + 1/(1 − α)²) = **2,5**. Com α = 0,9, esses valores sobem para 5,5 e 50,5.
- **Por que não maior:** Richter, Alvarez e Dittrich (2015) mostram que a sondagem linear é a melhor opção abaixo de 50% de ocupação quando a maioria das buscas encontra a chave, como no nosso caso, em que todas encontram. Acima de 60%, ela passa a sofrer com agrupamento primário. Uma tabela maior gastaria mais memória sem ganho relevante.
- **No encadeamento separado**, o mesmo `m` dá listas com 0,5 elemento em média, e a busca custa Θ(1 + α) (CORMEN et al., 2009).

### 3. Endereçamento aberto com sondagem linear

Das três sondagens vistas em sala, a linear é a mais simples e a de melhor localidade de memória: as posições testadas são vizinhas, então ficam na mesma linha de cache. Richter, Alvarez e Dittrich (2015) a apontam como a mais rápida em inserções com fator de carga baixo. O ponto fraco é o agrupamento primário: Heileman e Luo (2005) mostram que o desempenho da sondagem linear varia muito com a qualidade da função hash e pode ficar muito pior que o do duplo hash no pior caso. Por isso mantivemos α ≤ 0,5 e medimos as **sondagens extras**, para verificar se houve agrupamento com cada função.

### 4. Primeira função: método da multiplicação

`h(k) = ⌊ m × frac(k × A) ⌋`, com A = (√5 − 1)/2 ≈ 0,6180339887, a constante sugerida por Knuth (1998). Richter, Alvarez e Dittrich (2015) mostram que o hashing multiplicativo é barato (uma multiplicação) e tem boa qualidade para chaves inteiras, mas com mais variação entre distribuições de dados que funções mais elaboradas.

**Usamos `double`, não `float`.** Um `float` tem 24 bits de mantissa: para uma matrícula de 8 dígitos, `k × A` passa de 2²⁴ ≈ 16,7 milhões e o `float` não guarda mais casas decimais. Testamos: com `float`, a parte fracionária de 86694336 × A dá 0, e **todas** as matrículas grandes cairiam no índice 0. Com `double` (53 bits), a precisão é suficiente para tabelas de 20 milhões de posições.

### 5. Segunda função: método do meio-quadrado (mid-square)

1. Eleva a matrícula ao quadrado em `long long`. 99.999.999² tem 16 dígitos e não cabe em `int` (máximo 2.147.483.647): com `int`, a conta estoura e o índice sai errado ou até negativo. Testamos: a matrícula 86694336 daria o índice −140.567, fora da tabela.
2. Descarta os 4 dígitos da direita.
3. Pega os dígitos seguintes com `% m`.

O resultado é formado pelos **dígitos do meio** do quadrado, que dependem de todos os dígitos da chave. Os da direita dependem só dos últimos dígitos da chave. É um dos métodos clássicos de transformação de chave em endereço, descrito no levantamento de Maurer e Lewis (1975) e comparado com outros cinco métodos em arquivos reais grandes por Lum, Yuen e Dodd (1971). Escolhemos esse método porque usa só operações vistas no curso (multiplicação, divisão inteira e resto) e é diferente da multiplicação por uma constante.

### 6. Remoção nas tabelas hash

O enunciado mede só inserção e busca, mas as duas tabelas hash também têm remoção. A forma de remover depende da estratégia de colisão.

**Endereçamento aberto: estado `REMOVIDO`** (`remover_hash_aberto`, slides 40 a 43). Cada posição do vetor é uma `struct item` com três campos: `chave` (a matrícula), `estado` e `aluno` (ponteiro para o registro). O estado é um de três valores, definidos com `#define`:

| Estado | Significado | O que a busca faz |
| --- | --- | --- |
| `VAZIO` | a posição nunca foi usada | para: a matrícula não existe |
| `OCUPADO` | guarda um aluno | compara a chave; se for diferente, continua |
| `REMOVIDO` | havia um aluno, mas ele foi removido | continua sondando |

Ao remover, a posição **não volta a ser `VAZIO`**: ela passa a `REMOVIDO`. Se voltasse a ser `VAZIO`, a busca pararia ali e diria que não existem as matrículas que colidiram e foram guardadas depois, como A no slide 43. Na inserção, uma posição `REMOVIDO` é reutilizada, como na rotina de inserção de Cormen et al. (2009, seção 11.4). Sem isso, as posições removidas nunca mais seriam usadas. A tabela é criada com todos os estados em `VAZIO`.

**Encadeamento separado: desencadear e liberar o nó** (`remover_hash_encadeado`, slides 51 a 53). Não há marca. O ponteiro `atual` percorre a lista da posição, e `anterior` acompanha o nó de trás. Ao achar a matrícula, a lista é refeita **antes** do `free(atual)`:

- se o nó é o primeiro da lista, o vetor passa a apontar para o seguinte (`ha->tabela[pos] = atual->prox`);
- senão, o anterior pula o nó removido (`anterior->prox = atual->prox`).

| Aspecto | Endereçamento aberto | Encadeamento separado |
| --- | --- | --- |
| Onde ficam os elementos | No próprio vetor | Em listas, uma por posição |
| Como remove | Estado vira `REMOVIDO` | Desencadeia e libera o nó |
| Busca após a remoção | Continua sondando após `REMOVIDO` | Percorre o restante da lista |
| Custo esperado | O(1), com boa dispersão | O(1), com boa dispersão |
| Pior caso | O(n) | O(n) |

Nos dois casos, só a posição ou o nó são liberados. O `Aluno` não é liberado, porque pertence ao vetor criado na leitura.

### 7. Contagem de colisões

Seguimos a definição do enunciado: conta-se uma colisão quando uma nova matrícula produz um **índice inicial** que já foi produzido por outra.

- **Endereçamento aberto:** um vetor `indice_usado[m]` marca os índices iniciais já produzidos. Não basta olhar se a posição está ocupada, porque ela pode estar ocupada por um elemento deslocado de outra posição pela sondagem.
- **Encadeamento:** como não há remoções, a lista da posição não estar vazia significa exatamente que o índice já foi produzido.

Como as duas estratégias usam a mesma função para o índice inicial, **a mesma função dá o mesmo número de colisões nas duas estratégias**. Isso serve de verificação de que a contagem está correta. A diferença de desempenho entre as estratégias aparece nos tempos e nas sondagens extras.

Para comparação, se a função espalhasse as chaves de forma perfeitamente aleatória, o número esperado de colisões após inserir n chaves seria n − m(1 − (1 − 1/m)ⁿ). Com α = 0,5, isso dá cerca de 21% das inserções.

### 8. Outras decisões

- **As estruturas guardam a matrícula e um ponteiro para `Aluno`.** Cada registro é criado uma vez na leitura, e as cinco estruturas apontam para ele, sem copiar 10 milhões de registros cinco vezes. A matrícula também fica no próprio nó ou item (`chave`), como na `struct elem` vista em aula. Assim a busca compara a chave direto, sem ir até o registro a cada passo, e as cinco estruturas comparam do mesmo jeito. O nó continua com 32 bytes na memória, por isso esse campo não aumenta o consumo.
- **Medição com `clock()`**, de `time.h`. No Linux a resolução é de microssegundos. No Windows ela é de cerca de 1 ms, por isso tempos muito curtos (buscas nas tabelas hash com 100 mil registros) podem aparecer como 0 ou 1 ms.
- **Gerador aleatório próprio** (congruencial linear de 64 bits, semente fixa 2026). O `rand()` do Windows só vai até 32.767 e não alcançaria posições na casa dos milhões. Com semente fixa, o sorteio é o mesmo em qualquer computador.
- **Tipos inteiros:** o projeto usa `int` em tudo, exceto em dois lugares que precisam de 64 bits: o quadrado do meio-quadrado (`long long`) e o gerador aleatório (`unsigned long long`, cuja conta depende do estouro controlado de um `unsigned`). Os contadores cabem em `int`: com 10 milhões de registros, colisões e sondagens extras ficam na casa dos milhões, e o limite do `int` é 2,1 bilhões.

## Resultados

Os resultados abaixo foram obtidos num ambiente de teste (Linux em nuvem, processador Intel Xeon, 2 núcleos, 8 GB de RAM, `gcc 13.3 -O2`). **Antes de entregar, rode o programa na máquina da equipe** e substitua estas tabelas pelas do seu `resultados.md`: os valores absolutos mudam de computador para computador, mas as conclusões devem se manter.

**Base usada nestes testes:** o arquivo recebido tinha 8.282.764 registros completos (a última linha estava cortada), por isso o terceiro volume foi rodado com **8 milhões**, como prévia do de 10 milhões. Com a base completa, o comando padrão roda os três volumes do enunciado.

Arquivos completos: [`resultados/resultados_ambiente_teste.md`](resultados/resultados_ambiente_teste.md) e [`resultados/resultados_ambiente_teste.csv`](resultados/resultados_ambiente_teste.csv). Em todas as etapas, as cinco estruturas encontraram as 10.000 matrículas buscadas.

### 100.000 registros (TABLE_SIZE = 200.003)

Tempos em milissegundos. Inserção = tempo do intervalo; busca = 10.000 buscas após a etapa.

| Estrutura | Inserção 0–30% | Inserção 30–60% | Inserção 60–100% | Busca 30% | Busca 60% | Busca 100% |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 1,49 | 1,47 | 1,11 | 200,14 | 411,93 | 858,40 |
| Hash aberto + multiplicacao | 2,04 | 1,26 | 2,05 | 0,29 | 0,25 | 0,25 |
| Hash aberto + meio-quadrado | 1,36 | 1,27 | 2,02 | 0,19 | 0,23 | 0,27 |
| Hash encadeado + multiplicacao | 3,11 | 2,11 | 3,50 | 0,18 | 0,32 | 0,39 |
| Hash encadeado + meio-quadrado | 2,46 | 2,37 | 2,72 | 0,23 | 0,33 | 0,39 |

| Tabela hash | Colisões 0–30% | Colisões 30–60% | Colisões 60–100% | α após 30% / 60% / 100% | Sondagens extras (total) | Maior lista (100%) |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Hash aberto + multiplicacao | 2.176 | 6.073 | 13.215 | 0,15 / 0,30 / 0,50 | 50.422 | - |
| Hash aberto + meio-quadrado | 2.150 | 6.011 | 13.199 | 0,15 / 0,30 / 0,50 | 49.875 | - |
| Hash encadeado + multiplicacao | 2.176 | 6.073 | 13.215 | 0,15 / 0,30 / 0,50 | - | 6 |
| Hash encadeado + meio-quadrado | 2.150 | 6.011 | 13.199 | 0,15 / 0,30 / 0,50 | - | 6 |

### 1.000.000 registros (TABLE_SIZE = 2.000.003)

Tempos em milissegundos. Inserção = tempo do intervalo; busca = 10.000 buscas após a etapa.

| Estrutura | Inserção 0–30% | Inserção 30–60% | Inserção 60–100% | Busca 30% | Busca 60% | Busca 100% |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 5,82 | 10,96 | 13,58 | 2.909 | 5.728 | 9.509 |
| Hash aberto + multiplicacao | 12,39 | 13,69 | 21,86 | 0,26 | 0,29 | 0,32 |
| Hash aberto + meio-quadrado | 13,88 | 13,69 | 22,40 | 0,28 | 0,32 | 0,36 |
| Hash encadeado + multiplicacao | 29,32 | 26,60 | 34,60 | 0,31 | 0,44 | 0,49 |
| Hash encadeado + meio-quadrado | 40,04 | 28,34 | 37,44 | 0,39 | 0,42 | 0,56 |

| Tabela hash | Colisões 0–30% | Colisões 30–60% | Colisões 60–100% | α após 30% / 60% / 100% | Sondagens extras (total) | Maior lista (100%) |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Hash aberto + multiplicacao | 20.941 | 59.356 | 129.980 | 0,15 / 0,30 / 0,50 | 488.803 | - |
| Hash aberto + meio-quadrado | 21.567 | 60.371 | 131.855 | 0,15 / 0,30 / 0,50 | 502.976 | - |
| Hash encadeado + multiplicacao | 20.941 | 59.356 | 129.980 | 0,15 / 0,30 / 0,50 | - | 7 |
| Hash encadeado + meio-quadrado | 21.567 | 60.371 | 131.855 | 0,15 / 0,30 / 0,50 | - | 7 |

### 8.000.000 registros (TABLE_SIZE = 16.000.057)

Tempos em milissegundos. Inserção = tempo do intervalo; busca = 10.000 buscas após a etapa.

| Estrutura | Inserção 0–30% | Inserção 30–60% | Inserção 60–100% | Busca 30% | Busca 60% | Busca 100% |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 59,89 | 82,21 | 110,33 | 26.909 | 63.178 | 119.401 |
| Hash aberto + multiplicacao | 113,20 | 121,85 | 220,81 | 0,32 | 0,35 | 0,46 |
| Hash aberto + meio-quadrado | 120,96 | 131,71 | 242,88 | 0,41 | 0,40 | 0,53 |
| Hash encadeado + multiplicacao | 218,57 | 231,12 | 323,04 | 0,51 | 0,59 | 0,77 |
| Hash encadeado + meio-quadrado | 316,67 | 246,10 | 351,25 | 0,55 | 0,63 | 0,77 |

| Tabela hash | Colisões 0–30% | Colisões 30–60% | Colisões 60–100% | α após 30% / 60% / 100% | Sondagens extras (total) | Maior lista (100%) |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Hash aberto + multiplicacao | 145.370 | 417.631 | 932.051 | 0,15 / 0,30 / 0,50 | 3.315.227 | - |
| Hash aberto + meio-quadrado | 171.453 | 481.988 | 1.049.954 | 0,15 / 0,30 / 0,50 | 3.998.796 | - |
| Hash encadeado + multiplicacao | 145.370 | 417.631 | 932.051 | 0,15 / 0,30 / 0,50 | - | 6 |
| Hash encadeado + meio-quadrado | 171.453 | 481.988 | 1.049.954 | 0,15 / 0,30 / 0,50 | - | 8 |

## Análise dos resultados

### 1. Busca: as tabelas hash são centenas de milhares de vezes mais rápidas que a lista

Com 8 milhões de registros, as 10.000 buscas levaram **119 segundos** na lista e **menos de 1 milissegundo** em qualquer tabela hash (de 0,46 ms a 0,77 ms), uma diferença de 150 mil a 260 mil vezes. A lista faz busca sequencial, O(n): cada busca percorre em média metade dos nós. A tabela hash calcula a posição e testa, em média, 1 a 2 posições, O(1) no caso médio.

O tempo da lista também cresce **mais rápido que n**. De 100 mil para 8 milhões de registros (80 vezes mais dados), a busca ficou 139 vezes mais lenta. O custo por nó visitado subiu de 1,7 ns para 3,0 ns, porque a lista grande não cabe na memória cache do processador, e os nós passam a vir da memória RAM. Nas tabelas hash, o tempo de busca quase não muda entre volumes (0,25 ms, 0,32 ms e 0,46 ms no endereçamento aberto com multiplicação, após 100%), como prevê o custo O(1).

O acesso à memória pesa muito nesse resultado. Numa versão anterior do projeto, o nó guardava só o ponteiro para o `Aluno`, e a busca precisava ir até o registro para ler a matrícula a cada nó. Com 1 milhão de registros, a busca na lista levava 30,5 s. Guardando a matrícula no próprio nó, caiu para 9,5 s, sem mudar a complexidade, que continua O(n).

### 2. Inserção: a lista é a mais rápida, e o encadeamento é o mais lento

A inserção no início da lista é O(1) e só faz um `malloc` e algumas atribuições, por isso é a mais rápida com 1 milhão e 8 milhões de registros. Com 100 mil, todos os tempos ficam entre 1 e 4 ms e as diferenças são pequenas. O endereçamento aberto vem em seguida: calcula o hash e preenche um item de um vetor já alocado. O encadeamento separado é o mais lento para inserir, em geral cerca de 2 vezes o endereçamento aberto (entre 1,3 e 2,9 vezes, conforme a etapa), porque faz um `malloc` por elemento. Isso concorda com Richter, Alvarez e Dittrich (2015), que recomendam evitar o encadeamento em cargas com muitas escritas.

No encadeamento, a primeira etapa às vezes é mais lenta que a segunda (por exemplo, 316,67 ms contra 246,10 ms com meio-quadrado e 8 milhões). O `calloc` reserva a tabela, mas o sistema operacional só entrega as páginas de memória quando elas são usadas pela primeira vez, e esse custo cai dentro da primeira etapa. No endereçamento aberto isso não acontece, porque a criação da tabela já percorre todas as posições para marcá-las como `VAZIO`.

### 3. Endereçamento aberto contra encadeamento separado

Com o mesmo fator de carga (α = 0,5), o endereçamento aberto também foi mais rápido na busca após a última etapa, em todos os volumes (por exemplo, 0,46 ms contra 0,77 ms com 8 milhões). Nas etapas iniciais de 100 mil registros, as buscas levam menos de 0,4 ms e as diferenças se confundem com a variação da medição. Ele compara a matrícula direto no vetor, e as posições vizinhas da sondagem linear ficam na mesma linha de cache. No encadeamento, cada elemento está num nó alocado separadamente, e é preciso seguir um ponteiro a mais até ele.

O agrupamento primário da sondagem linear ficou sob controle. A média de **sondagens extras por inserção** foi 0,50 com o meio-quadrado e de 0,41 a 0,50 com a multiplicação, exatamente o que a análise clássica prevê para encher uma tabela até α = 0,5 (KNUTH, 1998): a média de ½(1 + 1/(1 − α)²) − 1 entre α = 0 e α = 0,5 é 0,5. Com uma tabela mais cheia, esse número cresceria rapidamente, como observaram Peterson (1957) e Heileman e Luo (2005).

### 4. Multiplicação contra meio-quadrado

As duas funções espalharam bem as matrículas: a maior lista do encadeamento teve só 6 a 8 elementos, com média de 0,5.

- **Meio-quadrado:** as colisões ficaram praticamente iguais às esperadas para uma função ideal, que espalhasse as chaves ao acaso: 1.703.395 contra 1.704.485 esperadas com 8 milhões, cerca de 21,3% das inserções.
- **Multiplicação:** ficou igual ao esperado com 100 mil e 1 milhão de registros, e **12% abaixo** com 8 milhões (1.495.052 colisões). Os valores de frac(k × A) com A = (√5 − 1)/2 ficam bem distribuídos entre 0 e 1 (KNUTH, 1998), e o efeito aparece quando a base ocupa uma parte maior do espaço de matrículas.

Com menos colisões, a multiplicação também teve menos sondagens extras (0,41 por inserção) e, em todas as medições do volume grande, tempos de inserção e busca iguais ou um pouco menores. A diferença de tempo entre as duas funções, porém, é pequena perto da diferença entre as estruturas.

### Conclusão

- **Para buscar por matrícula:** qualquer configuração de tabela hash é a escolha certa, e a melhor foi **endereçamento aberto com sondagem linear e método da multiplicação**, com α ≤ 0,5.
- **Para só inserir**, a lista é mais rápida, mas inviável para buscas: 10.000 buscas levaram cerca de 2 minutos com 8 milhões de registros.
- **Encadeamento separado:** é uma boa alternativa quando não se sabe quantos registros virão, porque a tabela nunca enche e α pode passar de 1. A remoção também é mais simples, porque não precisa de marca. O preço é a inserção mais lenta e mais memória para os nós.

## Referências

CORMEN, T. H.; LEISERSON, C. E.; RIVEST, R. L.; STEIN, C. **Introduction to Algorithms**. 3. ed. Cambridge: MIT Press, 2009. Cap. 11: Hash Tables.

HEILEMAN, G. L.; LUO, W. How caching affects hashing. In: WORKSHOP ON ALGORITHM ENGINEERING AND EXPERIMENTS (ALENEX), 7., 2005, Vancouver. **Proceedings** [...]. Philadelphia: SIAM, 2005. p. 141–154.

KNUTH, D. E. **The Art of Computer Programming**. v. 3: Sorting and Searching. 2. ed. Reading: Addison-Wesley, 1998. Seção 6.4: Hashing.

LUM, V. Y.; YUEN, P. S. T.; DODD, M. Key-to-address transform techniques: a fundamental performance study on large existing formatted files. **Communications of the ACM**, v. 14, n. 4, p. 228–239, 1971.

MAURER, W. D.; LEWIS, T. G. Hash table methods. **ACM Computing Surveys**, v. 7, n. 1, p. 5–19, 1975. DOI: 10.1145/356643.356645.

PETERSON, W. W. Addressing for random-access storage. **IBM Journal of Research and Development**, v. 1, n. 2, p. 130–146, 1957. DOI: 10.1147/rd.12.0130.

RICHTER, S.; ALVAREZ, V.; DITTRICH, J. A seven-dimensional analysis of hashing methods and its implications on query processing. **Proceedings of the VLDB Endowment**, v. 9, n. 3, p. 96–107, 2015.
