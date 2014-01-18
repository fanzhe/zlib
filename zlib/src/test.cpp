#include <iostream>

//#include "GraphToMinDFSCode.h"
#include "Test/TestMinDFSCode.h"
#include "Test/testSubIso.h"

using namespace std;

void testMinDFS(int argc, char** argv) {
  if (argc < 2) {
    cout << "please input" << endl;
    return;
  }
  TestMinDFSCode* g_test = new TestMinDFSCode(atoi(argv[2]));

  g_test->loadFromInputFile(argv[1]);

  // transform a test graph to its minDFSCode
  g_test->testMinDFSCode();

  delete g_test;
}

void testSubIso(int argc, char** argv) {
  if (argc < 4) {
      cout << "please input" << endl;
      return;
    }
  TestSubIso* g_test = new TestSubIso(atoi(argv[2]), atoi(argv[4]));

  g_test->loadFromInputFile(argv[1], argv[3]);

  g_test->testSubIso();
}

int main(int argc, char** argv) {
//  test minDFSCode
  testMinDFS(argc, argv);

//  test subIso
  testSubIso(argc, argv);

  return 0;
}
