#include "../include/calculateRezolution.hpp"

void calculateRezolution::setData(std::vector<TSmallEventData> data_vec,
                                  bool opt_verbose) {

  m_data_vec.clear();
  m_unique_channels.clear();
  m_ch_index_map.clear();
  for (auto &histos : m_expHistos) {
    delete (histos);
  }
  for (auto &histos : m_bg_Extracted_expHistos) {
    delete (histos);
  }
  m_expHistos.clear();
  m_bg_Extracted_expHistos.clear();

  m_opt_verbose = opt_verbose;
  m_data_vec = std::move(data_vec);

  if (m_data_vec.size() == 0) {

    throw std::runtime_error("Data vector is empty.");
  }

  m_unique_channels.clear();

  for (const auto &event : m_data_vec) {
    if (std::find(m_unique_channels.begin(), m_unique_channels.end(),
                  event.channel) == m_unique_channels.end()) {

      m_unique_channels.push_back(event.channel);
    }
  }

  m_unique_ch_nr = m_unique_channels.size();

  if (m_opt_verbose) {
    std::cout << "Nr of channels " << m_unique_ch_nr << "\n";
  }
}

void calculateRezolution::formHistograms() {

  int i = 0;

  m_expHistos.reserve(m_unique_channels.size());

  for (const auto &ch : m_unique_channels) {
    m_expHistos.push_back(new TH1F(Form("longCharge_ch%d", int(ch)),
                                   Form("LongCharge_ch%d", int(ch)), 2000, 0,
                                   2000));

    m_ch_index_map[int(ch)] = i;
    ++i;
  }

  for (const auto &data_el : m_data_vec) {

    m_expHistos[m_ch_index_map[int(data_el.channel)]]->Fill(data_el.energy);
  }
}

void calculateRezolution::smoothHistogram() {

  const int smoothingWindow = 8;

  for (auto &hist : m_expHistos) {
    const int nBins = hist->GetNbinsX();

    TH1F *smoothedHist =
        new TH1F(Form("smoothed_%s", hist->GetName()),
                 Form("Smoothed %s", hist->GetTitle()), nBins,
                 hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax());

    for (int i = 1; i <= nBins; ++i) {
      double sum = 0.0;
      int count = 0;

      for (int j = -smoothingWindow; j <= smoothingWindow; ++j) {
        int bin = i + j;
        if (bin >= 1 && bin <= nBins) {
          sum += hist->GetBinContent(bin);
          count++;
        }
      }
      double smoothedValue = count > 0 ? sum / count : 0;
      smoothedHist->SetBinContent(i, smoothedValue);
    }

    delete hist;
    hist = smoothedHist;
  }
}

void calculateRezolution::extractBg() {

  const Int_t iterations = 20;
  TSpectrum *spectrum = new TSpectrum();

  for (auto &histogram : m_expHistos) {
    TH1 *background = spectrum->Background(histogram, iterations);
    TH1F *copyHist = (TH1F *)histogram->Clone();
    copyHist->Add(background, -1);
    m_bg_Extracted_expHistos.push_back(copyHist);
  }
}

void calculateRezolution::searchForPeaks(TH1F *histo,
                                         Double_t DEFAULT_GAUSSIAN_SPREAD,
                                         int channel_nr) {

  Double_t threshold = 0.5;
  Double_t sigma = 30.5;

  TSpectrum *spectrum = new TSpectrum();
  Int_t nPeaks = spectrum->Search(histo, sigma, "", threshold);
  Double_t *xPeaks = spectrum->GetPositionX();
  Double_t *yPeaks = spectrum->GetPositionY();

  for (Int_t i = nPeaks - 1; i >= 0; i--) {
    Double_t peakPosition = xPeaks[i];
    Double_t fitRange = DEFAULT_GAUSSIAN_SPREAD;

    TF1 *gaussian = new TF1("gaus", "gaus", peakPosition - fitRange,
                            peakPosition + fitRange);
    histo->Fit(gaussian, "RQ");

    Double_t mean = gaussian->GetParameter(1);
    Double_t sigma = gaussian->GetParameter(2);
    Double_t resolution = (2.54 * sigma / mean) * ((i == 1) ? 1117 : 1332);

    m_resolutions.push_back({channel_nr, resolution});
    m_avg_res += resolution;
    ++m_peaks_count;

    if (m_opt_verbose) {
      std::cout << "Peak " << i + 1 << ": Mean = " << mean
                << ", Sigma = " << sigma << " / Energy resolution: "
                << (2.54 * sigma / mean) * ((i == 1) ? 1117 : 1332) << " keV"
                << std::endl;
    }
  }
}

void calculateRezolution::analyzeSpectrum(bool opt_smooth) {

  formHistograms();
  if (opt_smooth) {
    smoothHistogram();
  }
  extractBg();

  const Double_t Co60_SPREAD = 30;
  int i = 0;

  m_avg_res = 0;
  m_peaks_count = 0;

  for (auto *histo : m_bg_Extracted_expHistos) {

    if (m_opt_verbose) {
      std::cout << "\nHist " << i << " channel " << m_ch_index_map[i] << "\n";
    }

    searchForPeaks(histo, Co60_SPREAD, m_ch_index_map[i]);
    ++i;
  }

  m_avg_res /= m_peaks_count;
}

double calculateRezolution::returnAvgRes() const { return m_avg_res; }

std::vector<std::pair<int, double>> calculateRezolution::returnResVec() const {
  return m_resolutions;
}
