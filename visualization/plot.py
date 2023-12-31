from typing import NamedTuple
import numpy as np
import pandas as pd
from matplotlib import pyplot as plt
from ops_cnt import cnt_all


class Revision(NamedTuple):
    name: str
    no: int
    color: str
    style: str
    marker: str = 'o'


def set_ymargin(ax, left=0.0, right=0.3):
    ax.set_ymargin(0)
    ax.autoscale_view()
    lim = ax.get_ylim()
    delta = np.diff(lim)
    left = lim[0] - delta * left
    right = lim[1] + delta * right
    ax.set_ylim(left, right)


revisions = {
    "baseline": Revision("baseline", 0, "gray", ":"),
    "opt_4": Revision("unroll + no-transpose", 0, "yellow", ":"),
    "opt_5": Revision("non-interleaved image", 0, "orange", "--"),

    "opt_6": Revision("vectorized min-cut", 0, "tomato", "--", 'x'),
    "opt_6a": Revision("overlap error (vec)", 0, "maroon", "--", 'x'),

    "opt_6b": Revision("early stopping combined color", 0, "lightgreen", "-"),
    "opt_6c": Revision("early stopping rgb order", 0, "darkolivegreen", "-"),

    "opt_6d": Revision("improved src image locality", 0, "teal", "dashdot"),

    # "opt_8": Revision("vectorized overlap error (i32)", 0, "purple", "-"),
    "opt_9": Revision("early stopping (vec)", 0, "purple", "-", "x"),
    "opt_10": Revision("color permutation", 0, "indigo", "-"),
}


FIG_SIZE = (11, 9)
TEXT_SIZE = 16


def plot_sec(measurements, overlap, image, platform, cpu, include=None):
    plt.style.use("seaborn-v0_8-darkgrid")
    fig = plt.figure(figsize=FIG_SIZE)
    fig_ax = fig.gca()

    if include:
        revisions_items = ((k, revisions[k]) for k in include)
    else:
        revisions_items = revisions.items()

    for revi_key, revi in revisions_items:
        filtered = measurements[measurements["revision"]
                                == revi_key].reset_index()
        if len(filtered) == 0:
            continue

        fig_ax.plot(
            filtered["block_size"],
            filtered["runtime_s"],
            label=f"{revi.name} ({revi_key})" if revi_key != "baseline" else revi.name,
            color=revi.color,
            linestyle=revi.style,
            marker=revi.marker
        )

    plt.title(f"Quilting Runtime [{cpu}, overlap = {overlap} * block_size]",
              loc="left", fontsize=20, fontweight="bold",
              x=-0.031, y=1.06
              )
    plt.legend(fontsize=TEXT_SIZE, bbox_to_anchor=(
        0, -0.1), ncol=2, loc="upper left")
    fig_ax.grid(True, color="lightgray", linestyle="--", linewidth=1)
    fig_ax.set_yscale('log', base=2)
    # fig_ax.set_xscale('log', base=2)
    fig_ax.set_xlabel("block size", fontsize=TEXT_SIZE)
    fig_ax.set_xticks(filtered["block_size"])

    # fig_ax.set_xticks(arange(100, 1600, 100))
    fig_ax.set_ylabel("[seconds]", loc="top", rotation=0, fontsize=TEXT_SIZE)
    fig_ax.yaxis.set_label_coords(0.07, 1.02)
    fig_ax.tick_params(labelsize=14)
    # fig_ax.set_yticks(arange(0.5, 4., 0.5))
    # fig_ax.set_yticks(arange(0, 4.5, 0.5))

    # Save plot
    plt.tight_layout()
    overlap_str = f"{overlap}".replace(".", "_")
    plt.savefig(
        f"visualization/plot_sec_{image}_{platform}_overlap_{overlap_str}.pdf")
    plt.close(fig)


def plot_speedup(measurements, overlap, image, platform, cpu, include=None):
    plt.style.use("seaborn-v0_8-darkgrid")
    fig = plt.figure(figsize=FIG_SIZE)
    fig_ax = fig.gca()

    baseline = measurements[measurements["revision"]
                            == "baseline"].reset_index()

    if include:
        revisions_items = ((k, revisions[k]) for k in include)
    else:
        revisions_items = revisions.items()

    for revi_key, revi in revisions_items:
        filtered = measurements[measurements["revision"]
                                == revi_key].reset_index()
        if len(filtered) == 0:
            continue

        fig_ax.plot(
            filtered["block_size"],
            baseline["number_of_cycles"] / filtered["number_of_cycles"],
            label=f"{revi.name} ({revi_key})" if revi_key != "baseline" else revi.name,
            color=revi.color,
            linestyle=revi.style,
            marker=revi.marker
        )

    plt.title(f"Quilting Speedup [{cpu}, overlap = {overlap} * block_size]",
              loc="left", fontsize=20, fontweight="bold",
              x=-0.034, y=1.06
              )
    plt.legend(fontsize=TEXT_SIZE, bbox_to_anchor=(
        0, -0.1), ncol=2, loc="upper left")
    fig_ax.grid(True, color="lightgray", linestyle="--", linewidth=1)
    # fig_ax.set_yscale('log', base=2)
    # fig_ax.set_xscale('log', base=2)
    fig_ax.set_xlabel("block size", fontsize=TEXT_SIZE)
    fig_ax.set_xticks(filtered["block_size"])

    # fig_ax.set_xticks(arange(100, 1600, 100))
    fig_ax.set_ylabel("[speedup]", loc="top", rotation=0, fontsize=TEXT_SIZE)
    fig_ax.yaxis.set_label_coords(0.07, 1.02)
    fig_ax.tick_params(labelsize=14)
    # fig_ax.set_yticks(arange(0.5, 4., 0.5))
    # fig_ax.set_yticks(arange(0, 4.5, 0.5))

    # Save plot
    plt.tight_layout()
    overlap_str = f"{overlap}".replace(".", "_")
    plt.savefig(
        f"visualization/plot_speedup_{image}_{platform}_overlap_{overlap_str}.pdf")
    plt.close(fig)


def plot_perf(measurements, relative_overlap, image, platform, src_width, src_height, cpu, include=None):
    plt.style.use("seaborn-v0_8-darkgrid")
    fig = plt.figure(figsize=FIG_SIZE)
    fig_ax = fig.gca()

    if include:
        revisions_items = ((k, revisions[k]) for k in include)
    else:
        revisions_items = revisions.items()

    for revi_key, revi in revisions_items:
        filtered = measurements[measurements["revision"]
                                == revi_key].reset_index()
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
            marker=revi.marker
        )

    plt.title(f"Quilting Performance [{cpu}, overlap = {relative_overlap} * block_size]",
              loc="left", fontsize=20, fontweight="bold",
              x=-0.034, y=1.06
              )
    plt.legend(fontsize=TEXT_SIZE, bbox_to_anchor=(
        0, -0.1), ncol=2, loc="upper left")
    fig_ax.grid(True, color="lightgray", linestyle="--", linewidth=1)
    # fig_ax.set_yscale('log', base=2)
    # fig_ax.set_xscale('log', base=2)
    fig_ax.set_xlabel("block size", fontsize=TEXT_SIZE)
    fig_ax.set_xticks(filtered["block_size"])

    # fig_ax.set_xticks(arange(100, 1600, 100))
    fig_ax.set_ylabel("[ops/cycle]", loc="top", rotation=0, fontsize=TEXT_SIZE)
    fig_ax.yaxis.set_label_coords(0.08, 1.02)
    fig_ax.tick_params(labelsize=14)
    # fig_ax.set_yticks(arange(0.5, 4., 0.5))
    # fig_ax.set_yticks(arange(0, 4.5, 0.5))

    # Save plot
    plt.tight_layout()
    overlap_str = f"{relative_overlap}".replace(".", "_")
    plt.savefig(
        f"visualization/plot_perf_{image}_{platform}_overlap_{overlap_str}.pdf")
    plt.close(fig)


def parse_data(data_path: str):
    measurements = pd.read_csv(data_path, sep=',', header=0)

    measurements["output_size"] = (measurements['block_size'] - measurements["overlap_size"]) * (
        measurements["number_of_blocks_in_output_image"] - 1) + measurements['block_size']

    return measurements


def main(path, image_name, platform, cpu, include=None):
    measurements = parse_data(path)

    for relative_overlap in [0.25, 0.375, 0.5]:
        data = measurements[measurements["overlap_size"] ==
                            measurements["block_size"] * relative_overlap]
        data = data.reset_index()

        if 'Ryzen 9 3900x' in cpu:
            data['runtime_s'] = data["number_of_cycles"] / (3.6 * 10 ** 9)
        elif 'i7-8550U' in cpu:
            data['runtime_s'] = data["number_of_cycles"] / (1.8 * 10 ** 9)
        elif 'i5-1135G7' in cpu:
            data['runtime_s'] = data["number_of_cycles"] / (2.4 * 10 ** 9)
        else:
            raise ValueError("unknown cpu")

        if len(data) <= 0:
            continue

        if "dandelion" in image_name:
            src_w = 1024
            src_h = 683
        elif "radishes" in image_name or "blueflowers" in image_name:
            src_w = 192
            src_h = 192
        else:
            raise ValueError('unknown image')

        plot_sec(data, relative_overlap, image_name, platform, cpu, include)
        plot_speedup(data, relative_overlap,
                     image_name, platform, cpu, include)
        plot_perf(data, relative_overlap, image_name, platform,
                  src_w, src_h, cpu, include)


if __name__ == "__main__":
    main("timings/measure_red_intel.csv",
         "radishes", "intel", "i5-1135G7 @ 2.4 GHz")
    main("timings/measure_blue_intel.csv",
         "blueflowers", "intel", "i5-1135G7 @ 2.4 GHz")

    main("timings/measure_red_intel_kaby.csv",
         "radishes", "intel_kaby", "i7-8550U @ 1.8 GHz")
    main("timings/measure_blue_intel_kaby.csv",
         "blueflowers", "intel_kaby", "i7-8550U @ 1.8 GHz")

    main("timings/measure_red_amd.csv", "radishes",
         "amd", "Ryzen 9 3900x @ 3.6 GHz")
    main("timings/measure_blue_amd.csv", "blueflowers",
         "amd", "Ryzen 9 3900x @ 3.6 GHz")

    main("timings/measure_dandelion_amd.csv",
         "dandelion", "amd", "Ryzen 9 3900x @ 3.6 GHz")

    main(
        "timings/measure_blue_amd.csv", "blueflowers", "amd_f0", "Ryzen 9 3900x @ 3.6 GHz",
        include=["baseline", "opt_6b", "opt_6c", "opt_10"]
    )
    main("timings/measure_red_amd.csv", "radishes", "amd_f0", "Ryzen 9 3900x @ 3.6 GHz",
         include=["baseline", "opt_6b", "opt_6c", "opt_10"]
         )

    main(
        "timings/measure_blue_intel.csv", "blueflowers", "intel_f0", "i5-1135G7 @ 2.4 GHz",
        include=["baseline", "opt_6b", "opt_6c", "opt_10"]
    )
    main("timings/measure_red_intel.csv", "radishes", "intel_f0", "i5-1135G7 @ 2.4 GHz",
         include=["baseline", "opt_6b", "opt_6c", "opt_10"]
         )
