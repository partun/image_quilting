import argparse

import pandas as pd
from matplotlib import pyplot as plt


def plot0(measurements, block_size):
    plt.style.use("seaborn-v0_8-darkgrid")
    fig = plt.figure(figsize=(16, 8))
    fig_ax = fig.gca()

    for overlap in range(1, 6):
        filtered = measurements[measurements["overlap_size"] == overlap][measurements["block_size"] == block_size]
        output_size = 8

        perf = []
        for output_size, cycles in zip(filtered["number_of_blocks_in_output_image"], filtered["number_of_cycles"]):
            ops = (output_size ** 2) * (164 * 129)
            ops += (output_size ** 2) * (164 * 129) * 9 * overlap * block_size
            perf.append(ops / cycles)

        fig_ax.plot(
            filtered["number_of_blocks_in_output_image"],
            perf,
            label=f"overlap = {overlap}, blocksize={block_size}",
            linestyle="-",
            marker="o"
        )

    plt.title("Quilting Performance [i7-8550U @ 1.8 GHz]",
              loc="left", fontsize=20, fontweight="bold",
              x=-0.008, y=1.06
              )
    fig_ax.legend(loc="lower right", fontsize=14)
    fig_ax.grid(True, color="lightgray", linestyle="--", linewidth=1)
    # fig_ax.set_xscale('log', base=2)
    fig_ax.set_xlabel("output blocks", fontsize=14)
    # fig_ax.set_xticks([2 ** i for i in range(2, 24)])
    # fig_ax.set_xticks(arange(100, 1600, 100))
    fig_ax.set_ylabel("[ops/cycles]", loc="top", rotation=0, fontsize=14)
    fig_ax.yaxis.set_label_coords(0.06, 1.02)
    fig_ax.tick_params(labelsize=14)
    # fig_ax.set_yticks(arange(0.5, 4., 0.5))
    # fig_ax.set_yticks(arange(0, 4.5, 0.5))

    # Save plot
    plt.tight_layout()
    plt.savefig(f"visualization/plot_0_blocksize_{block_size}.png")


def parse_data(data_path: str):
    measurements = pd.read_csv(data_path, sep=',', header=0)
    return measurements


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('data_path', help="path to the timings csv file created by the c timing function")
    args = parser.parse_args()

    print(args.data_path)
    measurements = parse_data(args.data_path)

    for b in range(8, 31, 2):
        plot0(measurements, b)


if __name__ == "__main__":
    main()
