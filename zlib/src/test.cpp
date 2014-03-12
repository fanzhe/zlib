#include <cstdlib>
#include <iostream>

#include "test/testInput.h"
#include "test/testMinDFSCode.h"
#include "test/testSubGen.h"
#include "test/testSubIso.h"
#include "test/testTransform.h"
#include "test/testVFLib.h"

using namespace std;

void testInput(int argc, char** argv) {
  if (argc < 2) {
    cout << "please input" << endl;
    return;
  }

  TestInput* g_test = new TestInput(atoi(argv[2]));

  g_test->loadFromInputFile(argv[1]);

  delete g_test;
}

void testSubGen(int argc, char** argv) {
  if (argc < 2) {
    cout << "please input" << endl;
    return;
  }

  TestSubGen* g_test = new TestSubGen(atoi(argv[2]));

  g_test->loadFromInputFile(argv[1]);

  g_test->testSubGen();

  delete g_test;
}

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
//  g_test->debugSubIso();
}

void testVF(int argc, char** argv) {
  if (argc < 4) {
    cout << "please input" << endl;
    return;
  }

  TestVFLib* g_test = new TestVFLib(atoi(argv[2]), atoi(argv[4]));

  g_test->loadFromInputFile(argv[1], argv[3]);

  g_test->testSubIso();
//  g_test->testIndSub();
}

void testTransform(int argc, char** argv) {
  if (argc < 4) {
    cout << "please input" << endl;
    return;
  }
  TestTransform* g_test = new TestTransform(atoi(argv[2]));

  g_test->transformFromInputFile(argv[1], argv[3]);
}

int main(int argc, char** argv) {
//  test VFLib
//  testVF(argc, argv);
//	test input multiple vertex label graph
//  testInput(argc, argv);
//  test SubGen
//  testSubGen(argc, argv);

//  test minDFSCode
//  testMinDFS(argc, argv);

//  test subIso
  testSubIso(argc, argv);
  // test transform
//  testTransform(argc, argv);
  return 0;
}
