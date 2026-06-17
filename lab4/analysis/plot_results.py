#!/usr/bin/env python3
"""Построение таблицы и графиков по замерам lab4 (метод Якоби, OpenMP).

Скрипт запускает бинарь `jacobi_app table <k> <csv>`, который пишет
замеры в CSV, после чего по этим данным строит:
  * results/results.csv      — сырые данные (копия из бинаря);
  * results/results.md       — таблица в формате Markdown;
  * results/time_vs_n.png    — время работы от размера сетки N;
  * results/speedup_vs_n.png — ускорение от размера сетки N.

Можно не запускать бинарь, а взять готовый CSV: --no-run --csv path.

Зависимости: matplotlib (см. requirements.txt). Парсинг CSV — stdlib.
"""

from __future__ import annotations

import argparse
import csv
import subprocess
import sys
from pathlib import Path

import matplotlib

matplotlib.use("Agg")  # без дисплея — пишем сразу в файлы
import matplotlib.pyplot as plt

# lab4/analysis/plot_results.py -> lab4/
LAB_DIR = Path(__file__).resolve().parent.parent


def find_binary(explicit: str | None) -> Path:
    """Найти исполняемый файл jacobi_app."""
    if explicit:
        p = Path(explicit)
        if not p.exists():
            sys.exit(f"Бинарь не найден: {p}")
        return p

    candidates = [
        LAB_DIR / "build" / "jacobi_app",
        LAB_DIR / "build" / "jacobi_app.exe",
        LAB_DIR / "build" / "Release" / "jacobi_app.exe",
    ]
    for c in candidates:
        if c.exists():
            return c
    sys.exit(
        "jacobi_app не найден. Соберите проект (cmake --build build) "
        "или укажите путь через --binary."
    )


def run_benchmark(binary: Path, k: int, csv_path: Path) -> None:
    """Запустить бинарь в режиме table, чтобы он записал CSV."""
    csv_path.parent.mkdir(parents=True, exist_ok=True)
    cmd = [str(binary), "table", str(k), str(csv_path)]
    print("Запуск:", " ".join(cmd))
    subprocess.run(cmd, check=True)


def read_csv(csv_path: Path) -> list[dict]:
    """Прочитать CSV с замерами в список строк (числа приведены к типам)."""
    if not csv_path.exists():
        sys.exit(f"CSV не найден: {csv_path}")
    rows: list[dict] = []
    with csv_path.open(newline="") as f:
        for r in csv.DictReader(f):
            rows.append(
                {
                    "N": int(r["N"]),
                    "itmax": int(r["itmax"]),
                    "serial_s": float(r["serial_s"]),
                    "par2_s": float(r["par2_s"]),
                    "speedup2": float(r["speedup2"]),
                    "par8_s": float(r["par8_s"]),
                    "speedup8": float(r["speedup8"]),
                }
            )
    if not rows:
        sys.exit("CSV пустой — нечего строить.")
    return rows


def write_markdown(rows: list[dict], md_path: Path) -> None:
    """Сформировать таблицу замеров в Markdown."""
    header = (
        "| N | itmax | Последовательный, с | Параллельный, 2 потока, с | "
        "Ускорение | Параллельный, 8 потоков, с | Ускорение |"
    )
    sep = "|---|-------|---------------------|---------------------------|-----------|----------------------------|-----------|"
    lines = [header, sep]
    for r in rows:
        lines.append(
            f"| {r['N']} | {r['itmax']} | {r['serial_s']:.4f} | "
            f"{r['par2_s']:.4f} | {r['speedup2']:.3f} | "
            f"{r['par8_s']:.4f} | {r['speedup8']:.3f} |"
        )
    md_path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print("Таблица:", md_path)


def plot_time(rows: list[dict], png_path: Path) -> None:
    """График: время работы от размера сетки N."""
    n = [r["N"] for r in rows]
    fig, ax = plt.subplots(figsize=(7, 4.5))
    ax.plot(n, [r["serial_s"] for r in rows], "o-", label="Последовательный")
    ax.plot(n, [r["par2_s"] for r in rows], "s-", label="Параллельный, 2 потока")
    ax.plot(n, [r["par8_s"] for r in rows], "^-", label="Параллельный, 8 потоков")
    ax.set_xlabel("Размер сетки N")
    ax.set_ylabel("Время, с")
    ax.set_title("Время работы метода Якоби от размера сетки")
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend()
    fig.tight_layout()
    fig.savefig(png_path, dpi=120)
    plt.close(fig)
    print("График:", png_path)


def plot_speedup(rows: list[dict], png_path: Path) -> None:
    """График: ускорение от размера сетки N."""
    n = [r["N"] for r in rows]
    fig, ax = plt.subplots(figsize=(7, 4.5))
    ax.plot(n, [r["speedup2"] for r in rows], "s-", label="2 потока")
    ax.plot(n, [r["speedup8"] for r in rows], "^-", label="8 потоков")
    ax.axhline(1.0, color="gray", linestyle=":", label="без ускорения (×1)")
    ax.axhline(2.0, color="orange", linestyle=":", label="идеал на 2 потока (×2)")
    ax.axhline(8.0, color="red", linestyle=":", label="идеал на 8 потоков (×8)")
    ax.set_xlabel("Размер сетки N")
    ax.set_ylabel("Ускорение S = T_serial / T_parallel")
    ax.set_title("Ускорение параллельной версии")
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend()
    fig.tight_layout()
    fig.savefig(png_path, dpi=120)
    plt.close(fig)
    print("График:", png_path)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("-k", type=int, default=10,
                        help="вариант по списку группы (по умолчанию 10)")
    parser.add_argument("--binary", default=None,
                        help="путь к jacobi_app (по умолчанию ищется в build/)")
    parser.add_argument("--out", default=str(LAB_DIR / "results"),
                        help="директория для результатов (по умолчанию lab4/results)")
    parser.add_argument("--csv", default=None,
                        help="путь к CSV (по умолчанию <out>/results.csv)")
    parser.add_argument("--no-run", action="store_true",
                        help="не запускать бинарь, строить по существующему CSV")
    args = parser.parse_args()

    out_dir = Path(args.out)
    out_dir.mkdir(parents=True, exist_ok=True)
    csv_path = Path(args.csv) if args.csv else out_dir / "results.csv"

    if not args.no_run:
        binary = find_binary(args.binary)
        run_benchmark(binary, args.k, csv_path)

    rows = read_csv(csv_path)
    write_markdown(rows, out_dir / "results.md")
    plot_time(rows, out_dir / "time_vs_n.png")
    plot_speedup(rows, out_dir / "speedup_vs_n.png")
    print("Готово. Результаты в:", out_dir)


if __name__ == "__main__":
    main()
