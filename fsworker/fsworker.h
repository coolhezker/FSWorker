#ifndef FSWORKER_H
#define FSWORKER_H

#include <string>
#include <fstream>
#include <filesystem>
#include <exception>

namespace fsw_c {
	template<typename T>
	concept OStreamC = requires (T t, std::ostream & ofs) {
		{ ofs << t } -> std::same_as<std::ostream&>;
	};

	template<typename T>
	concept IStreamC = requires (T t, std::istream & ifs) {
		{ ifs >> t } -> std::same_as<std::istream&>;
	};

	template<typename T>
	concept ContainerC = requires(T t) {
		{ t.begin() } -> std::same_as<typename T::iterator>;
		{ t.end() } -> std::same_as<typename T::iterator>;
		//{ t.empty() };
		{ t.insert(t.end(), typename T::value_type{}) };
	};

	template<typename T>
	concept ToPathFS = requires(T t) {
		{ static_cast<std::filesystem::path>(t) } -> std::same_as<std::filesystem::path>;
		{ static_cast<std::string>(t) } -> std::same_as<std::string>;
	};
}

namespace fsw_e {
	class FSWException : public std::exception {
	private:
		std::string Message;
	public:
		FSWException(const std::string& msg) : Message{ msg } {};
		const char* what() const noexcept override {
			return Message.c_str();
		}
	};
}

namespace fsw {
	template<fsw_c::IStreamC Object, fsw_c::ToPathFS PathFS>
	inline void Reader(Object& obj, PathFS pfs) {
		std::ifstream ifs;
		ifs.open(pfs, std::ios::in);
		if (!ifs.is_open())
			throw fsw_e::FSWException("Invalid path");
		ifs >> obj;
	}

	
	template<fsw_c::ContainerC Container, fsw_c::ToPathFS PathFS>
	inline void Reader(Container& cont, PathFS pfs) {
		using Object = Container::value_type;
		std::ifstream ifs;
		Object buf;
		ifs.open(pfs, std::ios::in);
		if (!ifs.is_open())
			throw fsw_e::FSWException("Invalid path");
		while (ifs >> buf) {
			cont.insert(cont.end(), buf);
		}
	}

	template<fsw_c::OStreamC Object, fsw_c::ToPathFS PathFS>
	inline void Writer(Object& obj, PathFS pfs) {
		std::ofstream ofs;
		ofs.open(pfs, std::ios::app, std::ios::ate);
		if (!ofs.is_open())
			throw fsw_e::FSWException("Invalid path");
		ofs << obj;
	}

	template<fsw_c::ContainerC Container, fsw_c::ToPathFS PathFS>
	inline void Writer(Container& cont, PathFS pfs) {
		std::ofstream ofs;
		ofs.open(pfs, std::ios::app, std::ios::ate);
		if (!ofs.is_open())
			throw fsw_e::FSWException("Invalid path");
		for (auto it = cont.begin(); it != cont.end(); it++) {
			ofs << *it;
		}
	}

	template<fsw_c::OStreamC Object, fsw_c::ToPathFS PathFS>
	inline void ReWriter(Object& obj, PathFS pfs) {
		std::ofstream ofs;
		ofs.open(pfs, std::ios::out, std::ios::trunc);
		if (!ofs.is_open())
			throw fsw_e::FSWException("Invalid path");
		ofs << obj;
	}

	template<fsw_c::ContainerC Container, fsw_c::ToPathFS PathFS>
	inline void ReWriter(Container& cont, PathFS pfs) {
		std::ofstream ofs;
		ofs.open(pfs, std::ios::out, std::ios::trunc);
		if (!ofs.is_open())
			throw fsw_e::FSWException("Invalid path");
		for (auto it = cont.begin(); it != cont.end(); it++) {
			ofs << *it;
		}
	}

}

#endif // !FSWORKER_H