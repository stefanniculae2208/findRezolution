#include <iostream>

#include <TROOT.h>
#include <TString.h>

#include "./include/TEventData.hpp"
#include "./src/loadData.cpp"

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
    throw;
  }

  auto data_size = data_load.returnNEntries();
  auto data_vec = data_load.returnData();

  std::cout << data_size << std::endl;
}

int main(int argc, char *argv[]) {

  test_loadData();
  return 0;
}