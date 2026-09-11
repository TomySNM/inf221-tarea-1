# INF-221 Tarea 1 2026-2 | Nombre: Tomás San Martín | Rol: 202473565-9
# Lee los CSV de data/measurements/ y genera PNG en data/plots/ (matrices).
# Referencias:
#  [1] https://matplotlib.org/stable/
#  [2] https://pandas.pydata.org/docs/
import pathlib
import pandas as pd # type: ignore
import matplotlib # type: ignore
matplotlib.use("Agg")
import matplotlib.pyplot as plt # type: ignore

BASE = pathlib.Path(__file__).resolve().parents[1]   # code/matrix_multiplication
MEAS = BASE / "data" / "measurements"
OUT  = BASE / "data" / "plots"
OUT.mkdir(parents=True, exist_ok=True)
TIPOS = ["dispersa", "diagonal", "densa"]

dfs = []
for f in sorted(MEAS.glob("*.csv")):
    df = pd.read_csv(f)
    df["algorithm"] = f.stem
    dfs.append(df)
df = pd.concat(dfs, ignore_index=True)

agg = df.groupby(["algorithm", "type", "n"]).agg(
    time_ms=("time_ms", "mean"), mem_kb=("mem_kb", "mean")
).reset_index()

for metric, ylabel, fname in [("time_ms", "Tiempo (ms)", "tiempos"),
                              ("mem_kb", "Memoria extra (KB)", "memoria")]:
    fig, axes = plt.subplots(1, 3, figsize=(16, 4.6), sharey=True)
    for ax, t in zip(axes, TIPOS):
        sub = agg[agg["type"] == t]
        for alg, g in sub.groupby("algorithm"):
            g = g.sort_values("n")
            ax.plot(g["n"], g[metric], marker="o", label=alg)
        ax.set_xscale("log", base=2)
        if metric == "time_ms":
            ax.set_yscale("log")
        ax.set_title(f"Matriz {t}")
        ax.set_xlabel("n")
        ax.grid(True, which="both", ls=":", alpha=0.6)
    axes[0].set_ylabel(ylabel)
    axes[-1].legend(fontsize=9)
    fig.suptitle(f"Multiplicación de matrices: {ylabel} vs n (promedio sobre dominios y muestras)")
    fig.tight_layout()
    fig.savefig(OUT / f"{fname}.png", dpi=150)
    plt.close(fig)
print("Gráficos matrices guardados en", OUT)