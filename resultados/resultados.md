# Resultados dos experimentos

Ambiente: Linux em nuvem, Intel Xeon 2,1 GHz, 2 nucleos, 8 GB de RAM, gcc 13.3 -O2. Base completa (10.000.000 registros), execucao total de 8 minutos.

Tempos em milissegundos, medidos com clock(). 10000 buscas por etapa. Gerado a partir de resultados.csv.

## Volume: 100000 registros (TABLE_SIZE = 200003)

### Etapa 30% (insercao dos registros 1 a 30000)

| Estrutura | Insercao (ms) | 10000 buscas (ms) | Encontradas | Colisoes no intervalo | Fator de carga |
| --- | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 1.01 | 271.09 | 10000 | - | - |
| Hash aberto + multiplicacao | 1.40 | 0.15 | 10000 | 2176 | 0.150 |
| Hash aberto + meio-quadrado | 1.57 | 0.17 | 10000 | 2150 | 0.150 |
| Hash encadeado + multiplicacao | 2.84 | 0.22 | 10000 | 2176 | 0.150 |
| Hash encadeado + meio-quadrado | 2.30 | 0.15 | 10000 | 2150 | 0.150 |

### Etapa 60% (insercao dos registros 30001 a 60000)

| Estrutura | Insercao (ms) | 10000 buscas (ms) | Encontradas | Colisoes no intervalo | Fator de carga |
| --- | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 1.37 | 540.17 | 10000 | - | - |
| Hash aberto + multiplicacao | 1.50 | 0.22 | 10000 | 6073 | 0.300 |
| Hash aberto + meio-quadrado | 1.58 | 0.28 | 10000 | 6011 | 0.300 |
| Hash encadeado + multiplicacao | 2.49 | 0.27 | 10000 | 6073 | 0.300 |
| Hash encadeado + meio-quadrado | 2.88 | 0.35 | 10000 | 6011 | 0.300 |

### Etapa 100% (insercao dos registros 60001 a 100000)

| Estrutura | Insercao (ms) | 10000 buscas (ms) | Encontradas | Colisoes no intervalo | Fator de carga |
| --- | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 1.45 | 940.08 | 10000 | - | - |
| Hash aberto + multiplicacao | 2.39 | 0.24 | 10000 | 13215 | 0.500 |
| Hash aberto + meio-quadrado | 2.25 | 0.26 | 10000 | 13199 | 0.500 |
| Hash encadeado + multiplicacao | 2.90 | 0.34 | 10000 | 13215 | 0.500 |
| Hash encadeado + meio-quadrado | 3.84 | 0.39 | 10000 | 13199 | 0.500 |

## Volume: 1000000 registros (TABLE_SIZE = 2000003)

### Etapa 30% (insercao dos registros 1 a 300000)

| Estrutura | Insercao (ms) | 10000 buscas (ms) | Encontradas | Colisoes no intervalo | Fator de carga |
| --- | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 8.66 | 2882.00 | 10000 | - | - |
| Hash aberto + multiplicacao | 17.64 | 0.19 | 10000 | 20941 | 0.150 |
| Hash aberto + meio-quadrado | 18.73 | 0.34 | 10000 | 21567 | 0.150 |
| Hash encadeado + multiplicacao | 41.63 | 0.50 | 10000 | 20941 | 0.150 |
| Hash encadeado + meio-quadrado | 37.89 | 0.51 | 10000 | 21567 | 0.150 |

### Etapa 60% (insercao dos registros 300001 a 600000)

| Estrutura | Insercao (ms) | 10000 buscas (ms) | Encontradas | Colisoes no intervalo | Fator de carga |
| --- | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 14.59 | 5889.58 | 10000 | - | - |
| Hash aberto + multiplicacao | 15.60 | 0.28 | 10000 | 59356 | 0.300 |
| Hash aberto + meio-quadrado | 16.75 | 0.33 | 10000 | 60371 | 0.300 |
| Hash encadeado + multiplicacao | 28.02 | 0.33 | 10000 | 59356 | 0.300 |
| Hash encadeado + meio-quadrado | 28.68 | 0.36 | 10000 | 60371 | 0.300 |

### Etapa 100% (insercao dos registros 600001 a 1000000)

| Estrutura | Insercao (ms) | 10000 buscas (ms) | Encontradas | Colisoes no intervalo | Fator de carga |
| --- | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 16.08 | 10141.99 | 10000 | - | - |
| Hash aberto + multiplicacao | 24.54 | 0.28 | 10000 | 129980 | 0.500 |
| Hash aberto + meio-quadrado | 25.87 | 0.32 | 10000 | 131855 | 0.500 |
| Hash encadeado + multiplicacao | 37.84 | 0.41 | 10000 | 129980 | 0.500 |
| Hash encadeado + meio-quadrado | 40.20 | 0.50 | 10000 | 131855 | 0.500 |

## Volume: 10000000 registros (TABLE_SIZE = 20000003)

### Etapa 30% (insercao dos registros 1 a 3000000)

| Estrutura | Insercao (ms) | 10000 buscas (ms) | Encontradas | Colisoes no intervalo | Fator de carga |
| --- | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 101.79 | 34800.41 | 10000 | - | - |
| Hash aberto + multiplicacao | 179.81 | 0.30 | 10000 | 174126 | 0.150 |
| Hash aberto + meio-quadrado | 203.14 | 0.43 | 10000 | 214004 | 0.150 |
| Hash encadeado + multiplicacao | 399.45 | 0.45 | 10000 | 174126 | 0.150 |
| Hash encadeado + meio-quadrado | 372.41 | 0.83 | 10000 | 214004 | 0.150 |

### Etapa 60% (insercao dos registros 3000001 a 6000000)

| Estrutura | Insercao (ms) | 10000 buscas (ms) | Encontradas | Colisoes no intervalo | Fator de carga |
| --- | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 126.50 | 134635.02 | 10000 | - | - |
| Hash aberto + multiplicacao | 177.11 | 0.34 | 10000 | 502503 | 0.300 |
| Hash aberto + meio-quadrado | 198.75 | 0.41 | 10000 | 601224 | 0.300 |
| Hash encadeado + multiplicacao | 312.55 | 0.62 | 10000 | 502503 | 0.300 |
| Hash encadeado + meio-quadrado | 341.13 | 0.67 | 10000 | 601224 | 0.300 |

### Etapa 100% (insercao dos registros 6000001 a 10000000)

| Estrutura | Insercao (ms) | 10000 buscas (ms) | Encontradas | Colisoes no intervalo | Fator de carga |
| --- | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 156.95 | 273215.66 | 10000 | - | - |
| Hash aberto + multiplicacao | 301.92 | 0.45 | 10000 | 1130579 | 0.500 |
| Hash aberto + meio-quadrado | 352.45 | 0.52 | 10000 | 1313439 | 0.500 |
| Hash encadeado + multiplicacao | 468.72 | 0.74 | 10000 | 1130579 | 0.500 |
| Hash encadeado + meio-quadrado | 495.87 | 0.83 | 10000 | 1313439 | 0.500 |
