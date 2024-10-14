#ifndef loadData_hpp
#define loadData_hpp 1

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "TFile.h"
#include "TString.h"
#include "TTree.h"
#include <TROOT.h>
#include <TSystem.h>

#include "TEventData.hpp"

class loadData {

public:
  loadData(){};
  // ~loadData();

  loadData(const TString filename, const TString treename);

  void openFile();

  void readData();

  void openTree();

  Long64_t returnNEntries() const;

  std::vector<TSmallEventData> returnData() const;

private:
  /// @brief The name of the file to be opened.
  TString m_filename;

  /// @brief The name of the TTree in the file.
  TString m_treename;

  /// @brief The file containg the data.
  std::shared_ptr<TFile> m_rootfile;

  /// @brief The TTree read from the file.
  TTree *m_roottree;

  TSmallEventData m_data_branch;

  std::vector<TSmallEventData> m_data_vec;
};

#endif