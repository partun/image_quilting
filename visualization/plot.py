import argparse
from typing import NamedTuple

import pandas as pd
from matplotlib import pyplot as plt

from ops_cnt import cnt_all


class Revision(NamedTuple):
    name: str
    no: int
    color: str
    style: str


revisions = {
    "baseline": Revision("baseline", 0, "gray", ":"),
    "opt_4": Revision("onroll + no-transpose", 0, "yellow", "--"),
    "opt_5": Revision("non-interleved image", 0, "orange", "--"),
    "opt_6": Revision("vectorized min-cut overlap error", 0, "cornflowerblue", "-"),
    "opt_6a": Revision("vectorized overlap error (i16)", 0, "navy", "-"),
    "opt_6c": Revision("early stopping", 0, "green", "-"),
    "opt_6d": Revision("improved src image locality", 0, "orange", "-"),
    "opt_8": Revision("vectorized overlap error (i32)", 0, "purple", "-"),
    "opt_9": Revision("vectorized overlap error with early stopping", 0, "purple", "-"),
    "opt_10": Revision("color permutation", 0, "black", "-"),
}


def plot_sec(measurements, overlap, image):
    plt.style.use("seaborn-v0_8-darkgrid")
    fig = plt.figure(figsize=(16, 8))
    fig_ax = fig.gca()

    for revi_key, revi in revisions.items():
        filtered = measurements[measurements["revision"] == revi_key].reset_index()
        if len(filtered) == 0:
            continue

        fig_ax.plot(
            filtered["block_size"],
            filtered["runtime_s"],
            label=f"{revi.name} ({revi_key})" if revi_key != "baseline" else revi.name,
            color=revi.color,
            linestyle=revi.style,
            marker="o"
        )

    plt.title(f"Quilting Runtime [i7-8550U @ 1.8 GHz, overlap = {overlap} * block_size]",
              loc="left", fontsize=20, fontweight="bold",
              x=-0.015, y=1.06
              )
    fig_ax.legend(loc="upper right", fontsize=14)
    fig_ax.grid(True, color="lightgray", linestyle="--", linewidth=1)
    fig_ax.set_yscale('log', base=2)
    # fig_ax.set_xscale('log', base=2)
    fig_ax.set_xlabel("block size", fontsize=14)
    fig_ax.set_xticks(filtered["block_size"])

    # fig_ax.set_xticks(arange(100, 1600, 100))
    fig_ax.set_ylabel("[seconds]", loc="top", rotation=0, fontsize=14)
    fig_ax.yaxis.set_label_coords(0.04, 1.02)
    fig_ax.tick_params(labelsize=14)
    # fig_ax.set_yticks(arange(0.5, 4., 0.5))
    # fig_ax.set_yticks(arange(0, 4.5, 0.5))

    # Save plot
    plt.tight_layout()
    overlap_str = f"{overlap}".replace(".", "_")
    plt.savefig(f"visualization/plot_sec_{image}_overlap_{overlap_str}.png")


def plot_speedup(measurements, overlap, image):
    plt.style.use("seaborn-v0_8-darkgrid")
    fig = plt.figure(figsize=(16, 8))
    fig_ax = fig.gca()

    baseline = measurements[measurements["revision"] == "baseline"].reset_index()

    for revi_key, revi in revisions.items():
        filtered = measurements[measurements["revision"] == revi_key].reset_index()
        if len(filtered) == 0:
            continue

        fig_ax.plot(
            filtered["block_size"],
            baseline["number_of_cycles"] / filtered["number_of_cycles"],
            label=f"{revi.name} ({revi_key})" if revi_key != "baseline" else revi.name,
            color=revi.color,
            linestyle=revi.style,
            marker="o"
        )

    plt.title(f"Quilting Speedup [i7-8550U @ 1.8 GHz, overlap = {overlap} * block_size]",
              loc="left", fontsize=20, fontweight="bold",
              x=-0.015, y=1.06
              )
    fig_ax.legend(loc="upper right", fontsize=14)
    fig_ax.grid(True, color="lightgray", linestyle="--", linewidth=1)
    # fig_ax.set_yscale('log', base=2)
    # fig_ax.set_xscale('log', base=2)
    fig_ax.set_xlabel("block size", fontsize=14)
    fig_ax.set_xticks(filtered["block_size"])

    # fig_ax.set_xticks(arange(100, 1600, 100))
    fig_ax.set_ylabel("[speedup]", loc="top", rotation=0, fontsize=14)
    fig_ax.yaxis.set_label_coords(0.04, 1.02)
    fig_ax.tick_params(labelsize=14)
    # fig_ax.set_yticks(arange(0.5, 4., 0.5))
    # fig_ax.set_yticks(arange(0, 4.5, 0.5))

    # Save plot
    plt.tight_layout()
    overlap_str = f"{overlap}".replace(".", "_")
    plt.savefig(f"visualization/plot_speedup_{image}_overlap_{overlap_str}.png")


def plot_perf(measurements, relative_overlap, image, src_width, src_height):
    plt.style.use("seaborn-v0_8-darkgrid")
    fig = plt.figure(figsize=(16, 8))
    fig_ax = fig.gca()

    baseline = measurements[measurements["revision"] == "baseline"].reset_index()

    for revi_key, revi in revisions.items():
        filtered = measurements[measurements["revision"] == revi_key].reset_index()
        if len(filtered) == 0:
            continue

        y_values = []
        for idx, data in filtered.iterrows():
            ops = cnt_all(
                src_width, src_height, data["block_size"], data["overlap_size"],
                data["number_of_blocks_in_output_image"]
            )
            y_values.append(ops / data["number_of_cycles"])

        fig_ax.plot(
            filtered["block_size"],
            y_values,
            label=f"{revi.name} ({revi_key})" if revi_key != "baseline" else revi.name,
            color=revi.color,
            linestyle=revi.style,
            marker="o"
        )

    plt.title(f"Quilting Performance [i7-8550U @ 1.8 GHz, overlap = {relative_overlap} * block_size]",
              loc="left", fontsize=20, fontweight="bold",
              x=-0.015, y=1.06
              )
    fig_ax.legend(loc="upper right", fontsize=14)
    fig_ax.grid(True, color="lightgray", linestyle="--", linewidth=1)
    # fig_ax.set_yscale('log', base=2)
    # fig_ax.set_xscale('log', base=2)
    fig_ax.set_xlabel("block size", fontsize=14)
    fig_ax.set_xticks(filtered["block_size"])

    # fig_ax.set_xticks(arange(100, 1600, 100))
    fig_ax.set_ylabel("[ops/cycle]", loc="top", rotation=0, fontsize=14)
    fig_ax.yaxis.set_label_coords(0.04, 1.02)
    fig_ax.tick_params(labelsize=14)
    # fig_ax.set_yticks(arange(0.5, 4., 0.5))
    # fig_ax.set_yticks(arange(0, 4.5, 0.5))

    # Save plot
    plt.tight_layout()
    overlap_str = f"{relative_overlap}".replace(".", "_")
    plt.savefig(f"visualization/plot_perf_{image}_overlap_{overlap_str}.png")


def parse_data(data_path: str):
    measurements = pd.read_csv(data_path, sep=',', header=0)

    measurements["output_size"] = (measurements['block_size'] - measurements["overlap_size"]) * (
            measurements["number_of_blocks_in_output_image"] - 1) + measurements['block_size']

    return measurements


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        'data_path', help="path to the timings csv file created by the c timing function")
    args = parser.parse_args()

    print(args.data_path)
    measurements = parse_data(args.data_path)

    for relative_overlap in [0.25, 0.375, 0.5]:
        data = measurements[measurements["overlap_size"] == measurements["block_size"] * relative_overlap]
        data = data.reset_index()

        plot_sec(data, relative_overlap, "leaf")
        plot_speedup(data, relative_overlap, "leaf")
        plot_perf(data, relative_overlap, "leaf", 129, 164)


if __name__ == "__main__":
    main()
