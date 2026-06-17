#!/usr/bin/env python3
"""Построение таблицы и графиков по замерам lab5 (вариант 10, OpenMP).

Запускает бинарь `setrel_app bench <Nmax> <csv>`, который пишет замеры
в CSV (формат «длинный»: одна строка на пару N×threads), и по этим
данным строит:
  * results/results.csv        — сырые данные (копия из бинаря);
  * results/results.md         — сводная таблица в Markdown;
  * results/time_vs_n.png      — время работы от мощности множеств N;
  * results/speedup_vs_n.png   — ускорение S(p) от N;
  * results/efficiency_vs_n.png — эффективность E(p)=S(p)/p от N.

Можно не запускать бинарь, а взять готовый CSV: --no-run --csv path.

Зависимости: matplotlib (см. requirements.txt). Парсинг CSV — stdlib.
"""

from __future__ import annotations

import argparse
import csv
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt

LAB_DIR = Path(__file__).resolve().parent.parent


def find_binary(explicit: str | None) -> Path:
    """Найти исполняемый файл setrel_app."""
    if explicit:
        p = Path(explicit)
        if not p.exists():
            sys.exit(f"Бинарь не найден: {p}")
        return p

    candidates = [
        LAB_DIR / "build" / "setrel_app",
        LAB_DIR / "build" / "setrel_app.exe",
        LAB_DIR / "build" / "Release" / "setrel_app.exe",
    ]
    for c in candidates:
        if c.exists():
            return c
    sys.exit(
        "setrel_app не найден. Соберите проект (cmake --build build) "
        "или укажите путь через --binary."
    )


def run_benchmark(binary: Path, nmax: int, csv_path: Path) -> None:
    """Запустить бинарь в режиме bench, чтобы он записал CSV."""
    csv_path.parent.mkdir(parents=True, exist_ok=True)
    cmd = [str(binary), "bench", str(nmax), str(csv_path)]
    print("Запуск:", " ".join(cmd))
    subprocess.run(cmd, check=True)


def read_csv(csv_path: Path) -> list[dict]:
    """Прочитать «длинный» CSV в список строк с приведением типов."""
    if not csv_path.exists():
        sys.exit(f"CSV не найден: {csv_path}")
    rows: list[dict] = []
    with csv_path.open(newline="") as f:
        for r in csv.DictReader(f):
            rows.append(
                {
                    "N": int(r["N"]),
                    "threads": int(r["threads"]),
                    "serial_ms": float(r["serial_ms"]),
                    "parallel_ms": float(r["parallel_ms"]),
                    "speedup": float(r["speedup"]),
                    "efficiency": float(r["efficiency"]),
                }
            )
    if not rows:
        sys.exit("CSV пустой — нечего строить.")
    return rows


def by_threads(rows: list[dict]) -> dict[int, list[dict]]:
    """Сгруппировать строки по числу потоков, отсортировав по N."""
    groups: dict[int, list[dict]] = defaultdict(list)
    for r in rows:
        groups[r["threads"]].append(r)
    for g in groups.values():
        g.sort(key=lambda x: x["N"])
    return dict(sorted(groups.items()))


def sizes_sorted(rows: list[dict]) -> list[int]:
    return sorted({r["N"] for r in rows})


def serial_by_n(rows: list[dict]) -> dict[int, float]:
    """Время последовательной версии для каждого N (одинаково по строкам)."""
    return {r["N"]: r["serial_ms"] for r in rows}


def write_markdown(rows: list[dict], md_path: Path) -> None:
    """Сводная таблица: строки — N, колонки — потоки (время и ускорение)."""
    groups = by_threads(rows)
    ns = sizes_sorted(rows)
    serial = serial_by_n(rows)
    threads = list(groups.keys())

    header = "| N | Serial, мс |"
    sep = "|---|------------|"
    for t in threads:
        header += f" {t} потоков, мс | S({t}) | E({t}) |"
        sep += "------------|-------|-------|"
    lines = [header, sep]

    idx = {(r["threads"], r["N"]): r for r in rows}
    for n in ns:
        row = f"| {n} | {serial[n]:.3f} |"
        for t in threads:
            r = idx[(t, n)]
            row += f" {r['parallel_ms']:.3f} | {r['speedup']:.2f} | {r['efficiency']:.2f} |"
        lines.append(row)

    md_path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    print("Таблица:", md_path)


def plot_time(rows: list[dict], png_path: Path) -> None:
    """График: время работы от мощности множеств N."""
    groups = by_threads(rows)
    ns = sizes_sorted(rows)
    serial = serial_by_n(rows)

    fig, ax = plt.subplots(figsize=(7, 4.5))
    ax.plot(ns, [serial[n] for n in ns], "o-", label="Последовательно")
    for t, g in groups.items():
        ax.plot([r["N"] for r in g], [r["parallel_ms"] for r in g],
                "s-", label=f"{t} потоков")
    ax.set_xlabel("Мощность множеств N")
    ax.set_ylabel("Время, мс")
    ax.set_title("Время проверки отношений от размера задачи")
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend()
    fig.tight_layout()
    fig.savefig(png_path, dpi=120)
    plt.close(fig)
    print("График:", png_path)


def plot_speedup(rows: list[dict], png_path: Path) -> None:
    """График: ускорение S(p) от N."""
    groups = by_threads(rows)

    fig, ax = plt.subplots(figsize=(7, 4.5))
    for t, g in groups.items():
        ax.plot([r["N"] for r in g], [r["speedup"] for r in g],
                "s-", label=f"{t} потоков")
        ax.axhline(t, color="gray", linestyle=":", alpha=0.4)
    ax.set_xlabel("Мощность множеств N")
    ax.set_ylabel("Ускорение S(p) = T1 / Tp")
    ax.set_title("Ускорение параллельной версии (пунктир — идеал ×p)")
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend()
    fig.tight_layout()
    fig.savefig(png_path, dpi=120)
    plt.close(fig)
    print("График:", png_path)


def plot_efficiency(rows: list[dict], png_path: Path) -> None:
    """График: эффективность E(p) = S(p)/p от N."""
    groups = by_threads(rows)

    fig, ax = plt.subplots(figsize=(7, 4.5))
    for t, g in groups.items():
        ax.plot([r["N"] for r in g], [r["efficiency"] for r in g],
                "s-", label=f"{t} потоков")
    ax.axhline(1.0, color="red", linestyle=":", label="идеал (E=1)")
    ax.set_xlabel("Мощность множеств N")
    ax.set_ylabel("Эффективность E(p) = S(p) / p")
    ax.set_title("Эффективность параллельной версии")
    ax.grid(True, linestyle="--", alpha=0.5)
    ax.legend()
    fig.tight_layout()
    fig.savefig(png_path, dpi=120)
    plt.close(fig)
    print("График:", png_path)


def main() -> None:
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("-n", "--nmax", type=int, default=1600000,
                        help="максимальная мощность множеств (по умолчанию 1.6M)")
    parser.add_argument("--binary", default=None,
                        help="путь к setrel_app (по умолчанию ищется в build/)")
    parser.add_argument("--out", default=str(LAB_DIR / "results"),
                        help="директория для результатов (по умолчанию lab5/results)")
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
        run_benchmark(binary, args.nmax, csv_path)

    rows = read_csv(csv_path)
    write_markdown(rows, out_dir / "results.md")
    plot_time(rows, out_dir / "time_vs_n.png")
    plot_speedup(rows, out_dir / "speedup_vs_n.png")
    plot_efficiency(rows, out_dir / "efficiency_vs_n.png")
    print("Готово. Результаты в:", out_dir)


if __name__ == "__main__":
    main()
