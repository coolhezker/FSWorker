#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "fsworker.h"

using namespace std;

int main() {
	unordered_set<double> vec;
	cout << ":";
	try {
		fsw::Reader(vec, "D:\\taws\\pistesss.huini", '\n');
	}
	catch (const exception& e) {
		cout << e.what();
	}
	for (auto it = vec.begin(); it != vec.end(); it++) {
		cout << *it << endl;
	}
	fsw::ReWriter(vec, "D:\\taws\\pistesssa.huini", '\n');
	system("pause");
	return 0;
}