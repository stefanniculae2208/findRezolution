import subprocess
from pathlib import Path
import yaml
import itertools
import numpy as np
import pandas as pd
import argparse


def getResolution(file_name: Path):
    command = ["./build/find-rezolution", "-i", file_name]

    result = subprocess.run(
        command,
        capture_output=True,
        text=True,
        check=True,
    )

    output_lines = result.stdout.strip().split("\n")
    output_dict = {}

    for line in output_lines:
        key, value = line.split(":", 1)
        output_dict[key.strip()] = value.strip()

    double_value = float(output_dict.get("result", 0))
    status = output_dict.get("status", "Unknown")

    print(f"The returned double value is: {double_value}")
    print(f"Status: {status}\n")

    return double_value


def getAllRootFiles(folder_path: Path):
    extension = "*.root"

    root_files = list(folder_path.glob(extension))

    return root_files


def findFilesInFolder(folder_path: Path, param_file: Path, missing_files_txt: str):
    with open(param_file, "r") as file:
        config = yaml.safe_load(file)

    param_values = {}

    for param, settings in config.items():

        if (
            isinstance(settings, dict)
            and "min" in settings
            and "max" in settings
            and "step" in settings
        ):

            param_values[param] = np.arange(
                settings["min"], settings["max"] + settings["step"], settings["step"]
            )
        else:

            param_values[param] = settings

    combinations = list(itertools.product(*param_values.values()))

    files_list = []
    used_combinations = []
    missing_files = []

    for combination in combinations:

        filename = f"run_" + "_".join(map(str, combination)) + "_0.root"

        file_path = folder_path / filename

        if file_path.exists():
            files_list.append(file_path)
            used_combinations.append(combination)
        else:
            missing_files.append(filename)

    if missing_files:
        with open(missing_files_txt, "w") as f:
            for missing_file in missing_files:
                f.write(missing_file + "\n")

    return (files_list, used_combinations)


def save_results_to_csv(combinations, results, param_names, output_file):
    df = pd.DataFrame(combinations, columns=param_names)
    df["Result"] = results
    df.to_csv(output_file, index=False)


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Process folder path.")
    parser.add_argument(
        "folder_path",
        type=str,
        nargs="?",
        default="./test_data",
        help="Path to the folder to process (default: './test_data')",
    )
    args = parser.parse_args()
    folder_path = Path(args.folder_path)

    print(f"Folder path: {folder_path}")

    # folder_path = Path("./test_data")
    # root_files = getAllRootFiles(folder_path)
    # file_name = "./test_data/run_500_60_4_CFD_SMOOTH_EXP_2_CFD_FRACTLIST_50_0.root"

    root_files, used_combinations = findFilesInFolder(
        folder_path, Path("python_params.yaml"), "output/missing_files.txt"
    )

    results = []

    for file_name in root_files:
        print(f"Processing file: {file_name}")
        avg_res = getResolution(file_name)
        results.append(avg_res)

    with open("python_params.yaml", "r") as file:
        config = yaml.safe_load(file)
        param_names = list(config.keys())

    save_results_to_csv(
        used_combinations, results, param_names, "output/results_sci.csv"
    )

    print(f"Min value is {np.min(results)}")
    print(f"Max value is {np.max(results)}")
