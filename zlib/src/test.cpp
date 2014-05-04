#include <cstdlib>
#include <iostream>

#include "test/testInput.h"
#include "test/testMinDFSCode.h"
#include "test/testSubGen.h"
#include "test/testSubIso.h"
#include "test/testTransform.h"
#include "test/testVFLib.h"
#include "test/testGenDataSet.h"
#include "test/testCrypto.h"

#include <vector>

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

  g_test->testSubIso(argv[5], argv[6]);

  delete g_test;
}

void testDebug(int argc, char** argv) {
  if (argc < 4) {
    cout << "please input" << endl;
    return;
  }
  TestSubIso* g_test = new TestSubIso(atoi(argv[2]), atoi(argv[4]));

  g_test->loadFromInputFile(argv[1], argv[3]);

  g_test->debugSubIso();

  delete g_test;
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

  delete g_test;
}

void testTransformSNAP(int argc, char** argv) {
  if (argc < 4) {
    cout << "please input" << endl;
    return;
  }
  TestTransform* g_test = new TestTransform(atoi(argv[2]));

  g_test->transformFromSNAP(argv[1], argv[3]);

  delete g_test;
}

void testTransform(int argc, char** argv) {
  if (argc < 4) {
    cout << "please input" << endl;
    return;
  }
  TestTransform* g_test = new TestTransform(atoi(argv[2]));

  g_test->transformFromInputFile(argv[1], argv[3]);

  delete g_test;
}

void testGenDataSet(int argc, char** argv) {
  if (argc < 4) {
    cout << "please input" << endl;
    return;
  }
  TestGenDataSet* g_test = new TestGenDataSet(atoi(argv[2]));

  g_test->genDataSet(argv[1], argv[3], atoi(argv[4]));

  delete g_test;
}

void testGenQuerySet(int argc, char** argv) {
  if (argc < 4) {
    cout << "please input" << endl;
    return;
  }
  TestGenDataSet* g_test = new TestGenDataSet(atoi(argv[2]));

  g_test->genQuerySet(argv[1], argv[3], atoi(argv[4]), atoi(argv[5]));

  delete g_test;
}

void testShowStat(int argc, char** argv) {
  if (argc < 1) {
    cout << "please input" << endl;
    return;
  }
  TestGenDataSet* g_test = new TestGenDataSet(atoi(argv[2]));

  g_test->showStat(argv[1]);

  delete g_test;
}

void testCrypto(int argc, char** argv) {
  TestCrypto* cryp = new TestCrypto();
}

void testPlain() {
  vector<int> a;
  for (int i = 0; i < 2000000; i++) {
    a.push_back(i);
  }
  cout << "finish" << endl;
  return;
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
  if (argv[1][0] == 'd') {
    testDebug(argc - 1, argv + 1);
  } else if (argv[1][0] == 'r') {
    testSubIso(argc - 1, argv + 1);
  } else if (argv[1][0] == 't') {
    testTransform(argc - 1, argv + 1);
  } else if (argv[1][0] == 'g') {
    testGenDataSet(argc - 1, argv + 1);
  } else if (argv[1][0] == 'q') {
    testGenQuerySet(argc - 1, argv + 1);
  } else if (argv[1][0] == 'e') {
    testCrypto(argc - 1, argv + 1);
  } else if (argv[1][0] == 's') {
    testShowStat(argc - 1, argv + 1);
  } else if (argv[1][0] == 'n' ) {
    testTransformSNAP(argc - 1, argv + 1);
  } else {
    testPlain();
  }
  return 0;
}
