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

void returnRes(std::string data_file, bool opt_smooth = false) {

  gROOT->SetBatch(kTRUE);

  if (data_file.empty()) {

    std::cerr << "error:Bad file string.\n";
    throw;
  }

  TString data_tree = "data";

  loadData data_load(data_file, data_tree);

  try {

    data_load.openFile();
    data_load.openTree();
    data_load.readData();
  } catch (const std::exception &e) {
    std::cerr << "error:Error in test_loadData: " << e.what() << std::endl;
    throw;
  }

  auto data_size = data_load.returnNEntries();
  auto data_vec = data_load.returnData();

  calculateRezolution rez_calculator;

  try {
    rez_calculator.setData(data_vec, false);
  } catch (const std::exception &e) {
    std::cerr << "error:Error in test_calculateRezolutions: " << e.what()
              << std::endl;
    throw;
  }

  rez_calculator.analyzeSpectrum(opt_smooth);

  double avg_res = rez_calculator.returnAvgRes();
  auto res_vec = rez_calculator.returnResVec();

  std::cout << "status:Success\n";
  std::cout << "result:" << avg_res << "\n";
  std::cout << "count:" << data_size << "\n";

  std::cout << "resolutions:";
  for (const auto &res : res_vec) {
    std::cout << res.first << ":" << res.second << ";";
  }

  gROOT->SetBatch(kFALSE);
}

int main(int argc, char *argv[]) {

  std::string filename;
  bool opt_smooth = false;

  if (argc > 1) {
    for (auto i = 1; i < argc; ++i) {
      if (std::string(argv[i]) == "-i") {
        filename = argv[++i];
      }
      if (std::string(argv[i]) == "--smooth") {

        opt_smooth = true;
      }
    }

    try {
      returnRes(filename, opt_smooth);
    } catch (const std::exception &e) {
      std::cerr << e.what();
      return -1;
    }

  } else {
    std::cerr << "error:Must provide the appropriate command line arguments.\n";
    return -1;
  }

  return 0;
}