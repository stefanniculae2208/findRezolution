#include "../include/loadData.hpp"

/**
 * @brief Construct a new load Data::load Data object
 *
 * @param filename name of the file to be opened
 * @param treename name of the tree in the file
 */
loadData::loadData(const TString filename, const TString treename) {

  m_filename = filename;
  m_treename = treename;
}

/* loadData::~loadData() {

  m_roottree->ResetBranchAddresses();

  if (m_roottree) {

    delete (m_roottree);
    m_roottree = nullptr;
  }
} */

/**
 * @brief Checks if the file exists and opens it.
 */
void loadData::openFile() {

  try {
    m_rootfile = std::make_shared<TFile>(m_filename, "READ");

  } catch (const std::exception &e) {
    std::cerr << "Error in loadData::openFile: " << e.what() << std::endl;
    throw;
  }

  if (!m_rootfile->IsOpen()) {
    throw std::runtime_error("Failed to open ROOT file: " + m_filename);
  }
}

/**
 * @brief Reads the tree from the file.
 */
void loadData::openTree() {

  if (m_rootfile->IsZombie()) {

    throw std::runtime_error("Error: file isnot open: " + m_filename);
  }

  if (!(m_rootfile->GetListOfKeys()->Contains(m_treename))) {

    throw std::runtime_error("Error: file does not contain tree: " +
                             m_filename);
  }

  try {
    m_roottree = m_rootfile->Get<TTree>(m_treename);

    m_roottree->SetBranchAddress("Mod", &m_data_branch.module);
    m_roottree->SetBranchAddress("Ch", &m_data_branch.channel);
    m_roottree->SetBranchAddress("FineTS", &m_data_branch.timeStampNs);
    m_roottree->SetBranchAddress("ChargeLong", &m_data_branch.energy);
    m_roottree->SetBranchAddress("ChargeShort", &m_data_branch.energyShort);
    // m_roottree->SetBranchAddress("Signal", &m_data_branch.waveform);
  } catch (const std::exception &e) {
    std::cerr << "Error in loadData::openTree: " << e.what() << std::endl;
    throw;
  }
}

void loadData::readData() {

  if (m_rootfile->IsZombie()) {

    throw std::runtime_error("Error: file isnot open: " + m_filename);
  }

  if (!(m_rootfile->GetListOfKeys()->Contains(m_treename))) {

    throw std::runtime_error("Error: file does not contain tree: " +
                             m_filename);
  }

  if (m_data_vec.size() != 0) {

    m_data_vec.clear();
  }

  auto n_entries = m_roottree->GetEntries();

  for (auto entryNr = 0; entryNr < n_entries; ++entryNr) {

    m_roottree->GetEntry(entryNr);
    m_data_vec.push_back(m_data_branch);
  }
}

/**
 * @brief Returns the raw data vector. To be called only after the readData()
 * function.
 *
 * @return std::vector<rawData> the vector
 */
std::vector<TSmallEventData> loadData::returnData() const { return m_data_vec; }

/**
 * @brief Returns the number of entries. To be called only after the openTree()
 * function.
 *
 * @return Long64_t the number of entries taken from the file
 */
Long64_t loadData::returnNEntries() const { return m_roottree->GetEntries(); }
