#ifndef FSWORKER_H
#define FSWORKER_H

#include <string>
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
	concept ContainerC = requires(T t) {
		{ t.begin() } -> same_as<typename T::iterator>;
		{ t.end() } -> same_as<typename T::iterator>;
		//{ t.empty() };
		{ t.insert(t.end(), typename T::value_type{}) };
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
			throw fsw_e::FSWException("Invalid path");
		ifs >> obj;
	}

	
	template<fsw_c::ContainerC Container, fsw_c::ToPathFS PathFS>
	void Reader(Container& cont, PathFS pfs) {
		using Object = Container::value_type;
		ifstream ifs;
		Object buf;
		ifs.open(pfs, ios::in);
		if (!ifs.is_open())
			throw fsw_e::FSWException("Invalid path");
		while (ifs >> buf) {
			cont.insert(cont.end(), buf);
		}
	}

	template<fsw_c::OStreamC Object, fsw_c::ToPathFS PathFS>
	void Writer(Object& obj, PathFS pfs) {
		ofstream ofs;
		ofs.open(pfs, ios::app, ios::ate);
		if (!ofs.is_open())
			throw fsw_e::FSWException("Invalid path");
		ofs << obj;
	}

	template<fsw_c::ContainerC Container, fsw_c::ToPathFS PathFS>
	void Writer(Container& cont, PathFS pfs) {
		ofstream ofs;
		ofs.open(pfs, ios::app, ios::ate);
		if (!ofs.is_open())
			throw fsw_e::FSWException("Invalid path");
		for (auto it = cont.begin(); it != cont.end(); it++) {
			ofs << *it;
		}
	}

	template<fsw_c::OStreamC Object, fsw_c::ToPathFS PathFS>
	void ReWriter(Object& obj, PathFS pfs) {
		ofstream ofs;
		ofs.open(pfs, ios::out, ios::trunc);
		if (!ofs.is_open())
			throw fsw_e::FSWException("Invalid path");
		ofs << obj;
	}

	template<fsw_c::ContainerC Container, fsw_c::ToPathFS PathFS>
	void ReWriter(Container& cont, PathFS pfs) {
		ofstream ofs;
		ofs.open(pfs, ios::out, ios::trunc);
		if (!ofs.is_open())
			throw fsw_e::FSWException("Invalid path");
		for (auto it = cont.begin(); it != cont.end(); it++) {
			ofs << *it;
		}
	}

}

#endif // !FSWORKER_H