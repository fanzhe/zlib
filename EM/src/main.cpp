#include <cstdlib>
#include <iostream>

#include "KBQuality/testKBQuality.h"

using namespace std;

int main(int argc, char** argv) {
  TestKBQuality* dg_test = new TestKBQuality();

  // load data
  dg_test->loadFromInputFile(argv[1], atoi(argv[2]), argv[3], atoi(argv[4]), argv[5], argv[6], argv[7], argv[8], argv[9]);
  cout << "load finish" << endl;

  // load G^d for all e
  cout << "start load d-neighbor: " << endl;
  dg_test->loadDNeighbor();

  // pairing L
  cout << "start pairing: " << endl;
  dg_test->pairing();

  return 0;
}
