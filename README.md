# findRezolution

This program was created to find the best parameter configuration for digitizers when using different sources.

Install:
bash install.sh

Run:
python calculateRezolution.py [path to the folder containing the root files]

The folder should contain the the runs taken with the different parameters from the 'python_params.yaml' file.
The results can be found in './output'. Missing_files.txt mentions the name of the runs not found in the folder.