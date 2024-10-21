#include <fstream>
#include <iostream>
#include <vector>

#include <TROOT.h>
#include <TString.h>

#include "TCanvas.h"
#include "TF1.h"
#include "TH1.h"
#include "TSpectrum.h"

#include "./include/TEventData.hpp"
#include "./include/calculateRezolution.hpp"
#include "./include/loadData.hpp"

void test_loadData() {

  TString test_data_file =
      "./test_data/run_500_60_4_CFD_SMOOTH_EXP_2_CFD_FRACTLIST_50_0.root";

  TString test_data_tree = "data";

  loadData data_load(test_data_file, test_data_tree);

  try {

    data_load.openFile();
    data_load.openTree();
    data_load.readData();
  } catch (const std::exception &e) {
    std::cerr << "Error in test_loadData: " << e.what() << std::endl;
  }

  auto data_size = data_load.returnNEntries();
  auto data_vec = data_load.returnData();

  std::cout << data_size << std::endl;
}

void test_calculateRezolutions() {

  TString test_data_file =
      "./test_data/run_500_60_4_CFD_SMOOTH_EXP_2_CFD_FRACTLIST_50_0.root";

  TString test_data_tree = "data";

  loadData data_load(test_data_file, test_data_tree);

  try {

    data_load.openFile();
    data_load.openTree();
    data_load.readData();
  } catch (const std::exception &e) {
    std::cerr << "Error in test_loadData: " << e.what() << std::endl;
  }

  auto data_size = data_load.returnNEntries();
  auto data_vec = data_load.returnData();

  calculateRezolution rez_calculator;

  try {
    rez_calculator.setData(data_vec, true);
  } catch (const std::exception &e) {
    std::cerr << "Error in test_calculateRezolutions: " << e.what()
              << std::endl;
  }

  rez_calculator.analyzeSpectrum(false);

  auto avg_res = rez_calculator.returnAvgRes();

  std::cout << "Average rezolution no smoothing is " << avg_res << "\n\n\n";

  try {
    rez_calculator.setData(data_vec, true);
  } catch (const std::exception &e) {
    std::cerr << "Error in test_calculateRezolutions: " << e.what()
              << std::endl;
  }

  rez_calculator.analyzeSpectrum(true);

  avg_res = rez_calculator.returnAvgRes();

  std::cout << "Average rezolution with smoothing is " << avg_res << "\n";
}

int main(int argc, char *argv[]) {

  test_loadData();
  test_calculateRezolutions();
}
