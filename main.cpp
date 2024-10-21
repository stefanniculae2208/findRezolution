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

void test_viewHist() {

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

  // Base histogram.

  auto loc_canv =
      new TCanvas("View entries canvas", "View entries", 1500, 1000);
  auto loc_pad = new TPad("pad name", "pad title", 0, 0, 1, 1);
  loc_pad->Divide(2, 1);
  loc_pad->Draw();

  auto hist_view = new TH1F("Charge_Long", "Charge Long", 1800, 200, 2000);

  for (const auto &data_el : data_vec) {

    if (int(data_el.channel) == 1)
      hist_view->Fill(data_el.energy);
  }

  loc_pad->cd(1);
  hist_view->Draw();

  // Apply smoothing

  TH1F *hist_smoothed =
      new TH1F("Charge_Long_Smoothed", "Charge Long Smoothed", 1800, 200, 2000);

  const int nBins = hist_view->GetNbinsX();
  const int smoothingWindow = 8;

  for (int i = 1; i <= nBins; ++i) {

    double sum = 0.0;
    int count = 0;
    for (int j = -smoothingWindow; j <= smoothingWindow; ++j) {
      int bin = i + j;
      if (bin >= 1 && bin <= nBins) {
        sum += hist_view->GetBinContent(bin);
        count++;
      }
    }
    double smoothedValue = count > 0 ? sum / count : 0;
    hist_smoothed->SetBinContent(i, smoothedValue);
  }

  loc_pad->cd(2);
  hist_smoothed->Draw();

  loc_canv->Update();
  loc_canv->Print("./test_data/poza_test.png");

  loc_canv->Close();
}

void returnRes(std::string data_file) {

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

  rez_calculator.analyzeSpectrum(true);

  double avg_res = rez_calculator.returnAvgRes();

  std::cout << "status:Success\n";
  std::cout << "result:" << avg_res << "\n";

  gROOT->SetBatch(kFALSE);
}

int main(int argc, char *argv[]) {

  // test_viewHist();

  std::string filename;

  if (argc > 1) {
    for (auto i = 1; i < argc; ++i) {
      if (std::string(argv[i]) == "-i") {
        filename = argv[++i];
      }
    }

    try {
      returnRes(filename);
    } catch (const std::exception &e) {
      std::cerr << e.what();
    }

  } else {
    std::cerr << "error:Must provide the appropriate command line arguments.\n";
    return -1;
  }

  return 0;
}