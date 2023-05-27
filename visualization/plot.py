import argparse
from typing import NamedTuple

import pandas as pd
from matplotlib import pyplot as plt


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
    "opt_8": Revision("vectorized overlap error (i32)", 0, "purple", "-")
}


def plot0(measurements, overlap, image):
    plt.style.use("seaborn-v0_8-darkgrid")
    fig = plt.figure(figsize=(16, 8))
    fig_ax = fig.gca()

    for revi_key, revi in revisions.items():
        filtered = measurements[measurements["revision"] == revi_key].reset_index()

        fig_ax.plot(
            filtered["block_size"],
            filtered["runtime_s"],
            label=f"{revi.name} ({revi_key})" if revi_key != "baseline" else revi.name,
            color=revi.color,
            linestyle=revi.style,
            marker="o"
        )

    plt.title("Quilting Runtime [i7-8550U @ 1.8 GHz]",
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
    plt.savefig(f"visualization/plot_{image}_overlap_{overlap}.png")


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

    for overlap in sorted(set(measurements["overlap_size"])):
        data = measurements[measurements["overlap_size"] == overlap]
        data = data.reset_index()

        plot0(data, overlap, "leaf")


if __name__ == "__main__":
    main()
