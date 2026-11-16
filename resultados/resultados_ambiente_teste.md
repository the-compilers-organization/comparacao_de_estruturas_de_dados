# Resultados dos experimentos

Tempos em milissegundos, medidos com clock(). 10000 buscas por etapa.

## Volume: 100000 registros (TABLE_SIZE = 200003)

### Etapa 30% (insercao dos registros 1 a 30000)

| Estrutura | Insercao (ms) | 10000 buscas (ms) | Colisoes no intervalo | Fator de carga | Sondagens extras no intervalo | Maior lista |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 1.49 | 200.14 | - | - | - | - |
| Hash aberto + multiplicacao | 2.04 | 0.29 | 2176 | 0.150 | 2719 | - |
| Hash aberto + meio-quadrado | 1.36 | 0.19 | 2150 | 0.150 | 2644 | - |
| Hash encadeado + multiplicacao | 3.11 | 0.18 | 2176 | 0.150 | - | 4 |
| Hash encadeado + meio-quadrado | 2.46 | 0.23 | 2150 | 0.150 | - | 4 |

### Etapa 60% (insercao dos registros 30001 a 60000)

| Estrutura | Insercao (ms) | 10000 buscas (ms) | Colisoes no intervalo | Fator de carga | Sondagens extras no intervalo | Maior lista |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 1.47 | 411.93 | - | - | - | - |
| Hash aberto + multiplicacao | 1.26 | 0.25 | 6073 | 0.300 | 10270 | - |
| Hash aberto + meio-quadrado | 1.27 | 0.23 | 6011 | 0.300 | 10231 | - |
| Hash encadeado + multiplicacao | 2.11 | 0.32 | 6073 | 0.300 | - | 5 |
| Hash encadeado + meio-quadrado | 2.37 | 0.33 | 6011 | 0.300 | - | 5 |

### Etapa 100% (insercao dos registros 60001 a 100000)

| Estrutura | Insercao (ms) | 10000 buscas (ms) | Colisoes no intervalo | Fator de carga | Sondagens extras no intervalo | Maior lista |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 1.11 | 858.40 | - | - | - | - |
| Hash aberto + multiplicacao | 2.05 | 0.25 | 13215 | 0.500 | 37433 | - |
| Hash aberto + meio-quadrado | 2.02 | 0.27 | 13199 | 0.500 | 37000 | - |
| Hash encadeado + multiplicacao | 3.50 | 0.39 | 13215 | 0.500 | - | 6 |
| Hash encadeado + meio-quadrado | 2.72 | 0.39 | 13199 | 0.500 | - | 6 |

## Volume: 1000000 registros (TABLE_SIZE = 2000003)

### Etapa 30% (insercao dos registros 1 a 300000)

| Estrutura | Insercao (ms) | 10000 buscas (ms) | Colisoes no intervalo | Fator de carga | Sondagens extras no intervalo | Maior lista |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 5.82 | 2909.36 | - | - | - | - |
| Hash aberto + multiplicacao | 12.39 | 0.26 | 20941 | 0.150 | 25798 | - |
| Hash aberto + meio-quadrado | 13.88 | 0.28 | 21567 | 0.150 | 26696 | - |
| Hash encadeado + multiplicacao | 29.32 | 0.31 | 20941 | 0.150 | - | 4 |
| Hash encadeado + meio-quadrado | 40.04 | 0.39 | 21567 | 0.150 | - | 4 |

### Etapa 60% (insercao dos registros 300001 a 600000)

| Estrutura | Insercao (ms) | 10000 buscas (ms) | Colisoes no intervalo | Fator de carga | Sondagens extras no intervalo | Maior lista |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 10.96 | 5728.40 | - | - | - | - |
| Hash aberto + multiplicacao | 13.69 | 0.29 | 59356 | 0.300 | 100669 | - |
| Hash aberto + meio-quadrado | 13.69 | 0.32 | 60371 | 0.300 | 103107 | - |
| Hash encadeado + multiplicacao | 26.60 | 0.44 | 59356 | 0.300 | - | 6 |
| Hash encadeado + meio-quadrado | 28.34 | 0.42 | 60371 | 0.300 | - | 6 |

### Etapa 100% (insercao dos registros 600001 a 1000000)

| Estrutura | Insercao (ms) | 10000 buscas (ms) | Colisoes no intervalo | Fator de carga | Sondagens extras no intervalo | Maior lista |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 13.58 | 9509.29 | - | - | - | - |
| Hash aberto + multiplicacao | 21.86 | 0.32 | 129980 | 0.500 | 362336 | - |
| Hash aberto + meio-quadrado | 22.40 | 0.36 | 131855 | 0.500 | 373173 | - |
| Hash encadeado + multiplicacao | 34.60 | 0.49 | 129980 | 0.500 | - | 7 |
| Hash encadeado + meio-quadrado | 37.44 | 0.56 | 131855 | 0.500 | - | 7 |

## Volume: 8000000 registros (TABLE_SIZE = 16000057)

### Etapa 30% (insercao dos registros 1 a 2400000)

| Estrutura | Insercao (ms) | 10000 buscas (ms) | Colisoes no intervalo | Fator de carga | Sondagens extras no intervalo | Maior lista |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 59.89 | 26908.88 | - | - | - | - |
| Hash aberto + multiplicacao | 113.20 | 0.32 | 145370 | 0.150 | 176544 | - |
| Hash aberto + meio-quadrado | 120.96 | 0.41 | 171453 | 0.150 | 212078 | - |
| Hash encadeado + multiplicacao | 218.57 | 0.51 | 145370 | 0.150 | - | 4 |
| Hash encadeado + meio-quadrado | 316.67 | 0.55 | 171453 | 0.150 | - | 5 |

### Etapa 60% (insercao dos registros 2400001 a 4800000)

| Estrutura | Insercao (ms) | 10000 buscas (ms) | Colisoes no intervalo | Fator de carga | Sondagens extras no intervalo | Maior lista |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 82.21 | 63178.22 | - | - | - | - |
| Hash aberto + multiplicacao | 121.85 | 0.35 | 417631 | 0.300 | 679634 | - |
| Hash aberto + meio-quadrado | 131.71 | 0.40 | 481988 | 0.300 | 816902 | - |
| Hash encadeado + multiplicacao | 231.12 | 0.59 | 417631 | 0.300 | - | 6 |
| Hash encadeado + meio-quadrado | 246.10 | 0.63 | 481988 | 0.300 | - | 7 |

### Etapa 100% (insercao dos registros 4800001 a 8000000)

| Estrutura | Insercao (ms) | 10000 buscas (ms) | Colisoes no intervalo | Fator de carga | Sondagens extras no intervalo | Maior lista |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| Lista encadeada | 110.33 | 119401.23 | - | - | - | - |
| Hash aberto + multiplicacao | 220.81 | 0.46 | 932051 | 0.500 | 2459049 | - |
| Hash aberto + meio-quadrado | 242.88 | 0.53 | 1049954 | 0.500 | 2969816 | - |
| Hash encadeado + multiplicacao | 323.04 | 0.77 | 932051 | 0.500 | - | 6 |
| Hash encadeado + meio-quadrado | 351.25 | 0.77 | 1049954 | 0.500 | - | 8 |
