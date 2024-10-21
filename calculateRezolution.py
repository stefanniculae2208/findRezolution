import subprocess


def getResolution(file_name):
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
    message = output_dict.get("message", "")

    print(f"The returned double value is: {double_value}")
    print(f"Status: {status}")
    print(f"Message: {message}")


if __name__ == "__main__":

    file_name = "./test_data/run_500_60_4_CFD_SMOOTH_EXP_2_CFD_FRACTLIST_50_0.root"
    getResolution(file_name)
