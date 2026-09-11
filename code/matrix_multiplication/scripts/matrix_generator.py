# INF-221 Tarea 1 2026-2 | Nombre: Tomás San Martín | Rol: 202473565-9
# Genera los casos de prueba de matrices en data/matrix_input/
# Referencias: enunciado Tarea 1 INF-221 2026-2 (Anexo A).
import os
import random

OUT = os.path.join(os.path.dirname(__file__), "..", "data", "matrix_input")
os.makedirs(OUT, exist_ok=True)

NS = [16, 64, 256, 1024]
TIPOS = ["dispersa", "diagonal", "densa"]
DIGITOS = {"D0": 1, "D10": 10}
MUESTRAS = ["a", "b", "c"]

def valor(d):
    return random.randint(0, 9) if d == 1 else random.randint(10**(d-1), 10**d - 1)

def matriz(n, tipo, d):
    M = []
    for i in range(n):
        fila = []
        for j in range(n):
            if tipo == "densa":
                fila.append(valor(d))
            elif tipo == "diagonal":
                fila.append(valor(d) if i == j else 0)
            else:  # dispersa: ~10% no nulos
                fila.append(valor(d) if random.random() < 0.1 else 0)
        M.append(fila)
    return M

def guardar(M, ruta):
    with open(ruta, "w") as f:
        f.write(str(len(M)) + "\n")
        for fila in M:
            f.write(" ".join(map(str, fila)) + "\n")

for n in NS:
    for t in TIPOS:
        for dom, d in DIGITOS.items():
            for m in MUESTRAS:
                for k in (1, 2):
                    guardar(matriz(n, t, d), os.path.join(OUT, f"{n}_{t}_{dom}_{m}_{k}.txt"))
print("Todas las matrices han sido generadas.")