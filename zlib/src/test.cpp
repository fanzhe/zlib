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
//#include "utility/EVector.h"

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

void testSnap(int argc, char** argv) {
  if (argc < 4) {
    cout << "please input" << endl;
    return;
  }

  TestTransform* g_test = new TestTransform(atoi(argv[2]));
  g_test->transformFromSnap(argv[1], argv[3]);

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

void testGenQuerySetDFS(int argc, char** argv) {
  if (argc < 4) {
    cout << "please input" << endl;
    return;
  }
  TestGenDataSet* g_test = new TestGenDataSet(atoi(argv[2]));

  g_test->genQuerySetDFS(argv[1], argv[3], atoi(argv[4]), atoi(argv[5]));

  delete g_test;
}

void testGenQuerySetBFS(int argc, char** argv) {
  if (argc < 4) {
    cout << "please input" << endl;
    return;
  }
  TestGenDataSet* g_test = new TestGenDataSet(atoi(argv[2]));

  g_test->genQuerySetBFS(argv[1], argv[3], atoi(argv[4]), atoi(argv[5]));

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
  vector<int> u;
  u.push_back(1);
  u.push_back(4);
  u.push_back(5);
  vector<int> v;
  v.push_back(1);
  v.push_back(3);

  int inter = 0;
  int ind_u = 0;
  int ind_v = 0;
  int flg_u = 1;
  int flg_v = 1;
  while (ind_u < u.size() && ind_v < v.size()) {
    int nu = u[ind_u];
    int nv = v[ind_v];
    if (nu < nv) {
      ind_u++;
      flg_u = 0;
    } else if (nv < nu) {
      ind_v++;
      flg_v = 0;
    } else {
      inter++;
      ind_u++;
      ind_v++;
    }
    if (flg_u == flg_v == 0)
      break;
  }

  cout << inter << endl;
  cout << flg_u << " " << flg_v << endl;
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
    if (argv[1][1] == 'd') {
      testGenQuerySetDFS(argc - 1, argv + 1);
    }
    if (argv[1][1] == 'b') {
      testGenQuerySetBFS(argc - 1, argv + 1);
    }
  } else if (argv[1][0] == 'e') {
    testCrypto(argc - 1, argv + 1);
  } else if (argv[1][0] == 's') {
    if (argv[1][1] == 'n') {
      testSnap(argc - 1, argv + 1);
    } else {
      testShowStat(argc - 1, argv + 1);
    }
  } else if (argv[1][0] == 'n') {
    testTransformSNAP(argc - 1, argv + 1);
  } else {
    testPlain();
  }
  return 0;
}
