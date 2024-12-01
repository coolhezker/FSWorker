#ifndef FSWORKER_H
#define FSWORKER_H

#include <string>
#include <fstream>
#include <filesystem>
#include <exception>

namespace fsw::fsw_concepts {
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

namespace fsw::fsw_exceptions {
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
	template<fsw_concepts::IStreamC Object, fsw_concepts::ToPathFS PathFS>
	inline void Reader(Object& obj, PathFS pfs) {
		std::ifstream ifs;
		ifs.open(pfs, std::ios::in);
		if (!ifs.is_open())
			throw fsw_exceptions::FSWException("Invalid path");
		ifs >> obj;
	}

	
	template<fsw_concepts::ContainerC Container, fsw_concepts::ToPathFS PathFS>
	inline void Reader(Container& cont, PathFS pfs, char sep) {
		std::ifstream ifs;
		typename Container::value_type obj;
		char cbuf;
		ifs.open(pfs, std::ios::in);
		if (!ifs.is_open())
			throw fsw_exceptions::FSWException("Invalid path");
		while (ifs >> obj) {
			ifs.get(cbuf);
			if (cbuf != sep)
				throw fsw_exceptions::FSWException("Invalid separator");
			cont.insert(cont.end(), obj);
		}
	}

	template<fsw_concepts::OStreamC Object, fsw_concepts::ToPathFS PathFS>
	inline void Writer(Object& obj, PathFS pfs) {
		std::ofstream ofs;
		ofs.open(pfs, std::ios::app, std::ios::ate);
		if (!ofs.is_open())
			throw fsw_exceptions::FSWException("Invalid path");
		ofs << obj << std::endl;
	}

	template<fsw_concepts::ContainerC Container, fsw_concepts::ToPathFS PathFS>
	inline void Writer(Container& cont, PathFS pfs, char sep) {
		std::ofstream ofs;
		ofs.open(pfs, std::ios::app, std::ios::ate);
		if (!ofs.is_open())
			throw fsw_exceptions::FSWException("Invalid path");
		for (auto it = cont.begin(); it != cont.end();) {
			ofs << *it;
			it++;
			if (it != cont.end())
				ofs << sep;
		}
		ofs << std::endl;
	}

	template<fsw_concepts::OStreamC Object, fsw_concepts::ToPathFS PathFS>
	inline void ReWriter(Object& obj, PathFS pfs) {
		std::ofstream ofs;
		ofs.open(pfs, std::ios::out, std::ios::trunc);
		if (!ofs.is_open())
			throw fsw_exceptions::FSWException("Invalid path");
		ofs << obj << std::endl;
	}

	template<fsw_concepts::ContainerC Container, fsw_concepts::ToPathFS PathFS>
	inline void ReWriter(Container& cont, PathFS pfs, char sep) {
		std::ofstream ofs;
		ofs.open(pfs, std::ios::out, std::ios::trunc);
		if (!ofs.is_open())
			throw fsw_exceptions::FSWException("Invalid path");
		for (auto it = cont.begin(); it != cont.end();) {
			ofs << *it;
			it++;
			if (it != cont.end())
				ofs << sep;
		}
		ofs << std::endl;
	}

}

#endif // !FSWORKER_H