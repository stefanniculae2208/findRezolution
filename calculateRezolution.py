import subprocess
from pathlib import Path


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


if __name__ == "__main__":

    folder_path = Path("./test_data")

    root_files = getAllRootFiles(folder_path)

    # file_name = "./test_data/run_500_60_4_CFD_SMOOTH_EXP_2_CFD_FRACTLIST_50_0.root"

    for file_name in root_files:
        print(file_name)
        avg_res = getResolution(file_name)
