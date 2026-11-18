"""
Gera graficos comparativos a partir do CSV produzido pelo programa em C.

Uso (a partir da pasta raiz do projeto):
    python scripts/gerar_graficos.py
    python scripts/gerar_graficos.py outro.csv outra_pasta

Sem argumentos, le resultados/resultados.csv e grava os PNGs em resultados/graficos/.
Requisito: Python 3 com matplotlib (pip install matplotlib).
"""
import csv
import math
import os
import sys

try:
    import matplotlib
    matplotlib.use("Agg")                      # gera arquivos, sem abrir janela
    import matplotlib.pyplot as plt
    from matplotlib.ticker import FuncFormatter, LogLocator
except ImportError:
    print("matplotlib nao encontrado. Instale com:  pip install matplotlib")
    sys.exit(1)

# ---------------------------------------------------------------- estilo ---
SUPERFICIE = "#fcfcfb"
TEXTO = "#0b0b0b"
TEXTO_2 = "#52514e"
GRADE = "#e4e3df"

LISTA = "Lista encadeada"
HASHES = ["Hash aberto + multiplicacao", "Hash aberto + meio-quadrado",
          "Hash encadeado + multiplicacao", "Hash encadeado + meio-quadrado"]
ESTRUTURAS = [LISTA] + HASHES

# Cor fixa por estrutura (a mesma em todos os graficos).
COR = {
    LISTA: "#52514e",
    HASHES[0]: "#2a78d6",
    HASHES[1]: "#eb6834",
    HASHES[2]: "#1baf7a",
    HASHES[3]: "#eda100",
}
# Segunda codificacao, para nao depender so da cor:
# encadeamento = hachura nas barras e marcador quadrado nas linhas.
HACHURA = {LISTA: "", HASHES[0]: "", HASHES[1]: "", HASHES[2]: "////", HASHES[3]: "////"}
MARCADOR = {LISTA: "D", HASHES[0]: "o", HASHES[1]: "o", HASHES[2]: "s", HASHES[3]: "s"}

plt.rcParams.update({
    "figure.facecolor": SUPERFICIE, "axes.facecolor": SUPERFICIE,
    "savefig.facecolor": SUPERFICIE, "font.size": 10,
    "text.color": TEXTO, "axes.labelcolor": TEXTO_2,
    "xtick.color": TEXTO_2, "ytick.color": TEXTO_2,
    "axes.edgecolor": GRADE, "axes.linewidth": 0.8,
    "axes.grid": True, "grid.color": GRADE, "grid.linewidth": 0.6,
    "axes.axisbelow": True, "axes.spines.top": False, "axes.spines.right": False,
    "hatch.color": SUPERFICIE, "hatch.linewidth": 1.2,
    "legend.frameon": False, "axes.titlesize": 11, "axes.titleweight": "bold",
})


def br(valor, casas=2):
    """Numero no formato brasileiro: 1.234,56"""
    texto = f"{valor:,.{casas}f}"
    return texto.replace(",", "X").replace(".", ",").replace("X", ".")


def nome_volume(v):
    if v >= 1000000:
        return f"{br(v / 1000000, 0)} milhão" if v == 1000000 else f"{br(v / 1000000, 0)} milhões"
    return f"{br(v / 1000, 0)} mil"


def proximo_primo(n):
    def primo(x):
        if x < 2:
            return False
        d = 2
        while d * d <= x:
            if x % d == 0:
                return False
            d += 1
        return True
    while not primo(n):
        n += 1
    return n


# ----------------------------------------------------------------- dados ---
def ler(caminho):
    dados = {}
    with open(caminho, encoding="utf-8") as f:
        for r in csv.DictReader(f):
            chave = (int(r["volume"]), int(r["etapa_pct"]), r["estrutura"])
            dados[chave] = {
                "ins": float(r["tempo_insercao_ms"]),
                "busca": float(r["tempo_busca_ms"]),
                "col": int(r["colisoes_intervalo"]),
                "alfa": float(r["fator_carga"]),
            }
    volumes = sorted({k[0] for k in dados})
    etapas = sorted({k[1] for k in dados})
    return dados, volumes, etapas


def barras_agrupadas(ax, grupos, series, valor, rotulo_grupo):
    """Barras agrupadas finas, com 2 px de folga entre elas."""
    n = len(series)
    largura = 0.8 / n
    for i, s in enumerate(series):
        xs = [g + (i - (n - 1) / 2) * largura for g in range(len(grupos))]
        ys = [valor(grupo, s) for grupo in grupos]
        ax.bar(xs, ys, width=largura * 0.92, color=COR[s], hatch=HACHURA[s],
               edgecolor=SUPERFICIE, linewidth=0.8, label=s)
    ax.set_xticks(range(len(grupos)))
    ax.set_xticklabels([rotulo_grupo(g) for g in grupos])
    ax.grid(axis="x", visible=False)


def legenda_abaixo(fig, ax, ncol=3):
    handles, labels = ax.get_legend_handles_labels()
    fig.legend(handles, labels, loc="lower center", ncol=ncol, fontsize=9,
               bbox_to_anchor=(0.5, -0.01), handlelength=1.6)


# Tempos iguais a 0 (abaixo da resolucao do clock(), cerca de 1 ms no Windows)
# nao aparecem nos graficos de busca; esta nota explica a ausencia.
NOTA_ZERO = ("Barras ou pontos ausentes: o tempo medido foi 0 ms, abaixo da resolução do "
             "clock() (cerca de 1 ms no Windows).")
tem_zero = False


def nota_zero(fig):
    if tem_zero:
        fig.text(0.01, -0.06, NOTA_ZERO, ha="left", fontsize=8, color=TEXTO_2)


def eixo_log_decadas(ax, valores):
    """Eixo log de uma potencia de 10 ate outra, com uma marca em cada potencia."""
    positivos = [x for x in valores if x == x and x > 0]   # sem nan e sem 0
    if not positivos:
        return
    baixo = 10 ** math.floor(math.log10(min(positivos)))
    alto = 10 ** math.ceil(math.log10(max(positivos)))
    if alto == baixo:
        alto = baixo * 10
    ax.set_ylim(baixo, alto)
    ax.set_yticks([10 ** k for k in range(round(math.log10(baixo)), round(math.log10(alto)) + 1)])


def salvar(fig, pasta, nome, criados):
    caminho = os.path.join(pasta, nome)
    fig.savefig(caminho, dpi=150, bbox_inches="tight")
    plt.close(fig)
    criados.append(caminho)


def _fmt_eixo(y, _):
    if y == 0 or y == int(y):
        return br(y, 0)
    if y < 1:
        return br(y, 2).rstrip("0")      # 0,10 -> 0,1
    return br(y, 1)


fmt_ms = FuncFormatter(_fmt_eixo)


# --------------------------------------------------------------- graficos ---
def grafico_busca_por_volume(d, volumes, etapas, pasta, criados):
    """1. Tempo de 10.000 buscas apos a ultima etapa, por volume (escala log)."""
    ultima = etapas[-1]
    fig, ax = plt.subplots(figsize=(8, 5))
    for s in ESTRUTURAS:
        ys = [d[(v, ultima, s)]["busca"] for v in volumes]
        estilo = "--" if "meio" in s else "-"
        ax.plot(volumes, ys, estilo, color=COR[s], marker=MARCADOR[s], markersize=7,
                linewidth=2, label=s, markeredgecolor=SUPERFICIE, markeredgewidth=1.5)
    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xticks(volumes)
    ax.set_xticklabels([nome_volume(v) for v in volumes])
    ax.minorticks_off()
    eixo_log_decadas(ax, [d[(v, ultima, s)]["busca"] for v in volumes for s in ESTRUTURAS])
    ax.yaxis.set_major_formatter(fmt_ms)
    ax.set_xlabel("Registros na estrutura")
    ax.set_ylabel("Tempo de 10.000 buscas (ms, escala log)")
    ax.set_title(f"Busca após {ultima}% da inserção: lista × tabelas hash", loc="left")
    # rotulo direto so na lista, no ultimo ponto
    v = volumes[-1]
    t = d[(v, ultima, LISTA)]["busca"]
    ax.annotate(f"{br(t / 1000, 1)} s", (v, t), xytext=(-10, -14), textcoords="offset points",
                ha="right", va="center", color=TEXTO, fontsize=9)
    legenda_abaixo(fig, ax)
    fig.subplots_adjust(bottom=0.24)
    nota_zero(fig)
    salvar(fig, pasta, "1_busca_por_volume.png", criados)


def grafico_busca_hash_por_etapa(d, volumes, etapas, pasta, criados):
    """2. Busca nas quatro tabelas hash, por etapa, um painel por volume."""
    fig, axes = plt.subplots(1, len(volumes), figsize=(4 * len(volumes), 4.2), sharey=True)
    if len(volumes) == 1:
        axes = [axes]
    for ax, v in zip(axes, volumes):
        barras_agrupadas(ax, etapas, HASHES, lambda e, s: d[(v, e, s)]["busca"],
                         lambda e: f"após {e}%")
        ax.set_title(nome_volume(v), loc="left")
    axes[0].set_ylabel("Tempo de 10.000 buscas (ms)")
    axes[0].yaxis.set_major_formatter(FuncFormatter(lambda y, _: br(y, 2)))
    fig.suptitle("Busca nas tabelas hash (a lista fica de fora: é milhares de vezes mais lenta)",
                 x=0.01, ha="left", fontsize=11, fontweight="bold")
    legenda_abaixo(fig, axes[0], ncol=2)
    fig.subplots_adjust(bottom=0.25, top=0.82)
    nota_zero(fig)
    salvar(fig, pasta, "2_busca_hash_por_etapa.png", criados)


def grafico_insercao_por_etapa(d, volumes, etapas, pasta, criados):
    """3. Tempo de insercao de cada intervalo, um painel por volume."""
    fig, axes = plt.subplots(1, len(volumes), figsize=(4 * len(volumes), 4.2))
    if len(volumes) == 1:
        axes = [axes]
    rotulos, anterior = {}, 0
    for e in etapas:
        rotulos[e] = f"{anterior}–{e}%"
        anterior = e
    for ax, v in zip(axes, volumes):
        barras_agrupadas(ax, etapas, ESTRUTURAS, lambda e, s: d[(v, e, s)]["ins"],
                         lambda e: rotulos[e])
        ax.set_title(nome_volume(v), loc="left")
        ax.yaxis.set_major_formatter(fmt_ms)
    axes[0].set_ylabel("Tempo de inserção do intervalo (ms)")
    fig.suptitle("Inserção por etapa (cada painel tem sua própria escala)",
                 x=0.01, ha="left", fontsize=11, fontweight="bold")
    legenda_abaixo(fig, axes[0], ncol=3)
    fig.subplots_adjust(bottom=0.25, top=0.82, wspace=0.25)
    salvar(fig, pasta, "3_insercao_por_etapa.png", criados)


def grafico_insercao_por_volume(d, volumes, etapas, pasta, criados):
    """8. Tempo total de insercao (soma das etapas), por volume (escala log)."""
    def total(v, s):
        return sum(d[(v, e, s)]["ins"] for e in etapas)

    fig, ax = plt.subplots(figsize=(8, 5))
    for s in ESTRUTURAS:
        ys = [total(v, s) for v in volumes]
        estilo = "--" if "meio" in s else "-"
        ax.plot(volumes, ys, estilo, color=COR[s], marker=MARCADOR[s], markersize=7,
                linewidth=2, label=s, markeredgecolor=SUPERFICIE, markeredgewidth=1.5)
    ax.set_xscale("log")
    ax.set_yscale("log")
    ax.set_xticks(volumes)
    ax.set_xticklabels([nome_volume(v) for v in volumes])
    ax.minorticks_off()
    eixo_log_decadas(ax, [total(v, s) for v in volumes for s in ESTRUTURAS])
    ax.yaxis.set_major_formatter(fmt_ms)
    ax.set_xlabel("Registros inseridos")
    ax.set_ylabel("Tempo total de inserção (ms, escala log)")
    ax.set_title("Inserção de todos os registros (soma das etapas)", loc="left")
    # rotulos diretos no ultimo ponto: a mais rapida e a mais lenta
    v = volumes[-1]
    rapida = min(ESTRUTURAS, key=lambda s: total(v, s))
    lenta = max(ESTRUTURAS, key=lambda s: total(v, s))
    for s, dy in [(rapida, -14), (lenta, 14)]:
        t = total(v, s)
        texto = f"{br(t / 1000, 2)} s" if t >= 1000 else f"{br(t, 0)} ms"
        ax.annotate(texto, (v, t), xytext=(-10, dy), textcoords="offset points",
                    ha="right", va="center", color=TEXTO, fontsize=9)
    legenda_abaixo(fig, ax)
    fig.subplots_adjust(bottom=0.24)
    salvar(fig, pasta, "8_insercao_por_volume.png", criados)


def grafico_busca_por_etapa(d, volumes, etapas, pasta, criados):
    """7. Busca nas cinco estruturas, por etapa, um painel por volume (escala log)."""
    fig, axes = plt.subplots(1, len(volumes), figsize=(4 * len(volumes), 4.2), sharey=True)
    if len(volumes) == 1:
        axes = [axes]

    def tempo(v, e, s):
        t = d[(v, e, s)]["busca"]
        return t if t > 0 else float("nan")   # 0 ms nao cabe na escala log

    for ax, v in zip(axes, volumes):
        barras_agrupadas(ax, etapas, ESTRUTURAS, lambda e, s: tempo(v, e, s),
                         lambda e: f"após {e}%")
        ax.set_title(nome_volume(v), loc="left")
        ax.set_yscale("log")
        ax.minorticks_off()
    eixo_log_decadas(axes[0], [tempo(v, e, s) for v in volumes for e in etapas for s in ESTRUTURAS])
    axes[0].yaxis.set_major_formatter(fmt_ms)
    axes[0].set_ylabel("Tempo de 10.000 buscas (ms, escala log)")
    fig.suptitle("Busca por etapa (escala log: cada linha da grade é 10 vezes a de baixo)",
                 x=0.01, ha="left", fontsize=11, fontweight="bold")
    legenda_abaixo(fig, axes[0], ncol=3)
    fig.subplots_adjust(bottom=0.25, top=0.82)
    nota_zero(fig)
    salvar(fig, pasta, "7_busca_por_etapa.png", criados)


def grafico_vezes_mais_rapido(d, volumes, etapas, pasta, criados):
    """4. Quantas vezes cada tabela hash foi mais rapida que a lista na busca."""
    ultima = etapas[-1]
    fig, ax = plt.subplots(figsize=(8, 4.6))

    def razao(v, s):
        t = d[(v, ultima, s)]["busca"]
        return d[(v, ultima, LISTA)]["busca"] / t if t > 0 else float("nan")

    barras_agrupadas(ax, volumes, HASHES, razao, nome_volume)
    ax.set_yscale("log")
    ax.minorticks_off()
    eixo_log_decadas(ax, [razao(v, s) for v in volumes for s in HASHES])
    ax.yaxis.set_major_formatter(FuncFormatter(lambda y, _: br(y, 0)))
    ax.set_ylabel("Vezes mais rápida que a lista (escala log)")
    ax.set_title(f"Busca após {ultima}%: tempo da lista ÷ tempo da tabela hash", loc="left")
    # rotulo direto so no maior valor de cada volume
    n = len(HASHES)
    for g, v in enumerate(volumes):
        valores = [razao(v, s) for s in HASHES]
        validos = [k for k in range(n) if valores[k] == valores[k]]   # tira os nan
        if not validos:
            continue
        i = max(validos, key=lambda k: valores[k])
        x = g + (i - (n - 1) / 2) * (0.8 / n)
        ax.annotate(br(valores[i], 0) + "×", (x, valores[i]), xytext=(0, 3),
                    textcoords="offset points", ha="center", fontsize=8.5, color=TEXTO)
    legenda_abaixo(fig, ax, ncol=2)
    fig.subplots_adjust(bottom=0.24)
    nota_zero(fig)
    salvar(fig, pasta, "4_vezes_mais_rapido_que_lista.png", criados)


def grafico_colisoes(d, volumes, etapas, pasta, criados):
    """5. Colisoes (% das insercoes) de cada funcao contra uma funcao ideal."""
    funcoes = [("multiplicação", HASHES[0]), ("meio-quadrado", HASHES[1])]
    fig, ax = plt.subplots(figsize=(8, 4.6))
    n = len(funcoes) + 1
    largura = 0.8 / n
    for g, v in enumerate(volumes):
        m = proximo_primo(2 * v)
        esperado = v - m * (1 - (1 - 1 / m) ** v)
        valores = [sum(d[(v, e, s)]["col"] for e in etapas) / v * 100 for _, s in funcoes]
        valores.append(esperado / v * 100)
        for i, val in enumerate(valores):
            x = g + (i - (n - 1) / 2) * largura
            if i < len(funcoes):
                cor, hach, borda, rotulo = COR[funcoes[i][1]], "", SUPERFICIE, funcoes[i][0]
            else:
                cor, hach, borda, rotulo = SUPERFICIE, "", TEXTO_2, "esperado (função ideal)"
            ax.bar(x, val, width=largura * 0.92, color=cor, hatch=hach, edgecolor=borda,
                   linewidth=1.0, label=rotulo if g == 0 else None)
            ax.annotate(br(val, 1) + "%", (x, val), xytext=(0, 3), textcoords="offset points",
                        ha="center", fontsize=8.5, color=TEXTO)
    ax.set_xticks(range(len(volumes)))
    ax.set_xticklabels([nome_volume(v) for v in volumes])
    ax.grid(axis="x", visible=False)
    ax.set_ylabel("Colisões (% das inserções)")
    ax.set_ylim(0, max(25, ax.get_ylim()[1]))
    ax.set_title("Colisões com α = 0,5 (iguais no endereçamento aberto e no encadeamento)",
                 loc="left")
    legenda_abaixo(fig, ax, ncol=3)
    fig.subplots_adjust(bottom=0.2)
    salvar(fig, pasta, "5_colisoes_vs_esperado.png", criados)


def grafico_custo_por_no(d, volumes, etapas, pasta, criados):
    """6. Custo medio por no visitado na busca da lista (efeito da memoria cache)."""
    fig, ax = plt.subplots(figsize=(8, 4.2))
    rotulos, valores = [], []
    for v in volumes:
        for e in etapas:
            n = v // 100 * e
            visitados = 10000 * n / 2              # cada busca percorre, em media, metade
            valores.append(d[(v, e, LISTA)]["busca"] * 1e6 / visitados)
            rotulos.append(f"{nome_volume(v)}\napós {e}%")
    xs = range(len(valores))
    ax.bar(xs, valores, width=0.6, color=COR[LISTA], edgecolor=SUPERFICIE)
    for x, val in zip(xs, valores):
        ax.annotate(br(val, 1), (x, val), xytext=(0, 3), textcoords="offset points",
                    ha="center", fontsize=8.5, color=TEXTO)
    ax.set_xticks(list(xs))
    ax.set_xticklabels(rotulos, fontsize=8)
    ax.grid(axis="x", visible=False)
    ax.set_ylabel("Nanossegundos por nó visitado")
    ax.set_title("Lista encadeada: custo de cada nó visitado na busca", loc="left")
    salvar(fig, pasta, "6_lista_custo_por_no.png", criados)


def main():
    entrada = sys.argv[1] if len(sys.argv) > 1 else os.path.join("resultados", "resultados.csv")
    pasta = sys.argv[2] if len(sys.argv) > 2 else os.path.join("resultados", "graficos")
    if not os.path.exists(entrada):
        print(f"Arquivo {entrada} nao encontrado. Rode o programa em C antes,")
        print("a partir da pasta raiz do projeto.")
        sys.exit(1)
    os.makedirs(pasta, exist_ok=True)

    d, volumes, etapas = ler(entrada)
    global tem_zero
    tem_zero = any(r["busca"] == 0 for r in d.values())
    if tem_zero:
        print("Aviso: ha buscas medidas como 0 ms (abaixo da resolucao do clock()).")
        print("       Elas ficam de fora dos graficos de busca.")
    criados = []
    grafico_busca_por_volume(d, volumes, etapas, pasta, criados)
    grafico_busca_hash_por_etapa(d, volumes, etapas, pasta, criados)
    grafico_insercao_por_etapa(d, volumes, etapas, pasta, criados)
    grafico_vezes_mais_rapido(d, volumes, etapas, pasta, criados)
    grafico_colisoes(d, volumes, etapas, pasta, criados)
    grafico_custo_por_no(d, volumes, etapas, pasta, criados)
    grafico_busca_por_etapa(d, volumes, etapas, pasta, criados)
    grafico_insercao_por_volume(d, volumes, etapas, pasta, criados)

    print(f"Graficos gerados a partir de {entrada}:")
    for c in criados:
        print("  " + c)


if __name__ == "__main__":
    main()
