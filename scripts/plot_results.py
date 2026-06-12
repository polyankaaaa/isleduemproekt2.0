#!/usr/bin/env python3
"""Построение графиков по CSV из results/."""

from __future__ import annotations

import sys
from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd

RESULTS_DIR = Path(__file__).resolve().parent.parent / "results"
PLOTS_DIR = RESULTS_DIR / "plots"
CSV_1D = RESULTS_DIR / "benchmark_1d.csv"
CSV_2D = RESULTS_DIR / "benchmark_2d.csv"

METRICS = [
    ("build_ops", "Операции построения"),
    ("avg_query_ops", "Среднее операций на запрос"),
    ("avg_update_ops", "Среднее операций на обновление"),
]


def plot_1d(df: pd.DataFrame) -> None:
    structures = sorted(df["structure"].unique())
    for metric, title in METRICS:
        if metric == "avg_update_ops" and df[metric].max() == 0:
            continue

        plt.figure(figsize=(10, 6))
        for name in structures:
            part = df[df["structure"] == name].sort_values("n")
            y = part[metric]
            if metric == "avg_update_ops" and y.max() == 0:
                continue
            plt.plot(part["n"], y, marker="o", label=name)

        plt.xlabel("n")
        plt.ylabel(metric)
        plt.title(f"1D benchmark: {title}")
        plt.xscale("log")
        if metric != "build_ops":
            plt.yscale("log")
        plt.grid(True, which="both", linestyle="--", alpha=0.4)
        plt.legend(fontsize=8)
        plt.tight_layout()
        plt.savefig(PLOTS_DIR / f"1d_{metric}.png", dpi=150)
        plt.close()


def plot_2d(df: pd.DataFrame) -> None:
    part = df.assign(size=df["n"] * df["m"]).sort_values("size")
    structure_name = part["structure"].iloc[0]

    for metric, title in METRICS:
        if metric == "avg_update_ops" and df[metric].max() == 0:
            continue

        plt.figure(figsize=(8, 5))
        plt.plot(part["size"], part[metric], marker="o", label=structure_name)
        plt.xlabel("n * m")
        plt.ylabel(metric)
        plt.title(f"2D benchmark: {title}")
        plt.xscale("log")
        if metric != "build_ops":
            plt.yscale("log")
        plt.grid(True, which="both", linestyle="--", alpha=0.4)
        plt.legend()
        plt.tight_layout()
        plt.savefig(PLOTS_DIR / f"2d_{metric}.png", dpi=150)
        plt.close()


def main() -> int:
    missing = [p for p in (CSV_1D, CSV_2D) if not p.is_file()]
    if missing:
        print("Файлы benchmark ещё не созданы. Сначала выполните:", file=sys.stderr)
        print("  ./build/semga67_cli benchmark", file=sys.stderr)
        for path in missing:
            print(f"  отсутствует: {path}", file=sys.stderr)
        return 1

    PLOTS_DIR.mkdir(parents=True, exist_ok=True)

    df_1d = pd.read_csv(CSV_1D)
    df_2d = pd.read_csv(CSV_2D)

    plot_1d(df_1d)
    plot_2d(df_2d)

    print(f"Графики сохранены в {PLOTS_DIR}")
    for path in sorted(PLOTS_DIR.glob("*.png")):
        print(f"  {path.name}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
