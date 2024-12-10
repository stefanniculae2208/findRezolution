import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from typing import List


def findPartialDependence(
    filename: str, input_columns: List[str], output_columns: List[str]
):

    data = pd.read_csv(filename)

    num_inputs = len(input_columns)
    num_outputs = len(output_columns)
    fig, axes = plt.subplots(num_inputs, num_outputs, figsize=(12, num_inputs * 4))

    if num_inputs == 1:
        axes = np.array([axes])
    if num_outputs == 1:
        axes = axes[:, np.newaxis]

    for i, input_col in enumerate(input_columns):
        for j, output_col in enumerate(output_columns):

            # ax = axes[i][j] if num_inputs > 1 else axes[i]
            ax = axes[i][j]
            ax.plot(
                data[input_col].sort_values(),
                data.groupby(input_col)[output_col]
                .mean()
                .reindex(data[input_col].sort_values())
                .values,
                color="red",
                label="Mean trend",
            )
            ax.set_title(f"{output_col} vs {input_col}")
            ax.set_xlabel(input_col)
            ax.set_ylabel(output_col)
            ax.legend()

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":

    filename = "./output/results_all.csv"
    input_columns = [
        "ch_gate",
        "ch_gateshort",
        "ch_cfd_delay",
        "ch_cfd_smoothexp",
        "ch_cfd_fraction",
    ]
    # output_columns = ["Count", "Result"]
    output_columns = ["Result"]

    findPartialDependence(filename, input_columns, output_columns)
