#ifndef FSWORKER_H
#define FSWORKER_H

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <exception>

using namespace std;

namespace fsw_c {
	template<typename T>
	concept OStreamC = requires (T t, ostream & ofs) {
		{ ofs << t } -> same_as<ostream&>;
	};

	template<typename T>
	concept IStreamC = requires (T t, istream & ifs) {
		{ ifs >> t } -> same_as<istream&>;
	};

	template<typename T>
	concept ToPathFS = requires(T t) {
		{ static_cast<filesystem::path>(t) } -> same_as<filesystem::path>;
		{ static_cast<string>(t) } -> same_as<string>;
	};
}

namespace fsw_e {
	class FSWException : public exception {
	private:
		string Message;
	public:
		FSWException(const string& msg) : Message{ msg } {};
		const char* what() const noexcept override {
			return Message.c_str();
		}
	};
}

namespace fsw {
	template<fsw_c::IStreamC Object, fsw_c::ToPathFS PathFS>
	void Reader(Object& obj, PathFS pfs) {
		ifstream ifs;
		ifs.open(pfs, ios::in);
		if (!ifs.is_open())
			throw fsw_e::FSWException("Givenchi");
		ifs >> obj;
	}

	template<fsw_c::IStreamC Object, fsw_c::ToPathFS PathFS>
	void ReaderVec(vector<Object>& vobj, PathFS pfs) {
		ifstream ifs;
		Object buf;
		ifs.open(pfs, ios::in);
		if(!ifs.is_open()) 
			throw fsw_e::FSWException("Houdini");
		while (ifs >> buf) {
			vobj.push_back(buf);
		}
	}

	template<fsw_c::OStreamC Object, fsw_c::ToPathFS PathFS>
	void ReWriter(Object& obj, PathFS pfs) {
		ofstream ofs;
		ofs.open(pfs, ios::out, ios::trunc);
		if (!ofs.is_open())
			throw fsw_e::FSWException("Gulfik");
		ofs << obj;
	}

	template<fsw_c::OStreamC Object, fsw_c::ToPathFS PathFS>
	void ReWriterVec(vector<Object>& vobj, PathFS pfs) {
		ofstream ofs;
		ofs.open(pfs, ios::out, ios::trunc);
		if (!ofs.is_open())
			throw fsw_e::FSWException("Gulfik");
		for (auto it = vobj.begin(); it != vobj.end(); it++) {
			ofs << *it;
		}
	}
}

#endif // !FSWORKER_H