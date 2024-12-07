#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include "fsworker.h"

using namespace std;

struct Test {
	int pinel;
	Test() : pinel{ 0 } {};
	Test(int i) : pinel{ i } {};
	/*bool operator==(const Test& oth) const {
		return this->pinel == oth.pinel;
	}*/
};

istream& operator>>(istream& is, Test& t) {
	is >> t.pinel;
	return is;
}

ostream& operator<<(ostream& os, const Test& t) {
	os << t.pinel;
	return os;
}


int main() {
	vector<Test> vec;
	fstream t(std::filesystem::path("228"));
	cout << ":";
	try {
		fsw::Reader(vec, "D:\\taws\\pistes.txt", '\n');
	}
	catch (const exception& e) {
		cout << e.what();
	}
	for (auto it = vec.begin(); it != vec.end(); it++) {
		cout << *it << endl;
	}
	fsw::ReWriter(vec, "D:\\taws\\pg.huini", '\n');
	system("pause");
	return 0;
}