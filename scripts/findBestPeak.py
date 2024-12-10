import pandas as pd


def findBestPeaks(filename: str, opt_print_all: bool):

    df = pd.read_csv(filename)

    channel_columns = [col for col in df.columns if col.startswith("Channel_")]
    parameter_columns = [
        col for col in df.columns if col not in channel_columns + ["Result"]
    ]

    print("\nNon-null counts for each channel column:")
    for column_name in channel_columns:
        non_null_count = df[column_name].notnull().sum()
        print(f"{column_name}: {non_null_count} non-null values")

    min_values = df[channel_columns].min()

    print("\nMinimum values for each channel column with affiliated parameters:")
    for column_name, min_value in min_values.items():
        parts = column_name.split("_")
        channel = parts[1]
        val = parts[3]

        if opt_print_all:
            min_rows = df[df[column_name] == min_value]
            for _, row in min_rows.iterrows():

                print(f"Channel: {channel}, Peak: {val}, Result: {min_value}")
                parameter_values = {param: row[param] for param in parameter_columns}
                print("Affiliated parameters:", parameter_values)
        else:
            if int(val) < 2:
                min_rows = df[df[column_name] == min_value]
                for _, row in min_rows.iterrows():

                    print(f"Channel: {channel}, Peak: {val}, Result: {min_value}")
                    parameter_values = {
                        param: row[param] for param in parameter_columns
                    }
                    print("Affiliated parameters:", parameter_values)


if __name__ == "__main__":

    filename = "./output/results_all.csv"
    findBestPeaks(filename, False)
