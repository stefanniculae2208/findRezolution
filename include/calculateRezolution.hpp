#ifndef calculateRezolution_hpp
#define calculateRezolution_hpp 1

#include <fstream>
#include <iostream>
#include <unordered_set>

#include "TCanvas.h"
#include "TF1.h"
#include "TH1.h"
#include "TMath.h"
#include "TROOT.h"
#include "TSpectrum.h"

#include "TEventData.hpp"

class calculateRezolution {
public:
  calculateRezolution(){};

  void setData(std::vector<TSmallEventData> data_vec, bool opt_verbose = false);

  void analyzeSpectrum(bool opt_smooth = false);

  double returnAvgRes() const;

private:
  std::vector<TSmallEventData> m_data_vec;
  std::vector<uint8_t> m_unique_channels;
  int m_unique_ch_nr = 0;
  bool m_opt_verbose = false;

  std::vector<TH1F *> m_expHistos;
  std::vector<TH1F *> m_bg_Extracted_expHistos;
  std::unordered_map<int, int> m_ch_index_map;

  TH1F *m_raw_hist = nullptr;
  TH1F *m_fit_hist = nullptr;

  double m_avg_res = 0;
  int m_peaks_count = 0;

  void formHistograms();
  void smoothHistogram();
  void extractBg();
  void searchForPeaks(TH1F *histo, Double_t DEFAULT_GAUSSIAN_SPREAD);
};

#endif