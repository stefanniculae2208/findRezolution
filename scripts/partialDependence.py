import pandas as pd
import matplotlib.pyplot as plt


def findPartialDependence(filename: str):

    data = pd.read_csv(filename)

    print(data)

    input_columns = [
        "ch_gate",
        "ch_gateshort",
        "ch_cfd_delay",
        "ch_cfd_smoothexp",
        "ch_cfd_fraction",
    ]
    output_columns = ["Count", "Result"]

    num_inputs = len(input_columns)
    num_outputs = len(output_columns)
    fig, axes = plt.subplots(num_inputs, num_outputs, figsize=(12, num_inputs * 4))

    if num_inputs == 1:
        axes = [axes]

    for i, input_col in enumerate(input_columns):
        for j, output_col in enumerate(output_columns):
            ax = axes[i][j] if num_inputs > 1 else axes[j]
            ax.scatter(
                data[input_col], data[output_col], alpha=0.7, label="Data points"
            )
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

    filename = "./output/results_alls.csv"
    findPartialDependence(filename)
