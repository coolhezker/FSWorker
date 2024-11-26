#ifndef FSWORKER_H
#define FSWORKER_H

#include <string>
#include <fstream>
#include <filesystem>
#include <exception>

namespace fswconcepts {
	template<typename T>
	concept OFStreamC = requires (T t, std::ofstream & ofs) {
		{ ofs >> t } -> std::same_as<std::ofstream&>;
	};

	template<typename T>
	concept IFStreamC = requires (T t, std::ifstream & ifs) {
		{ ifs >> t } -> std::same_as<std::ifstream&>;
	};

	template<typename T>
	concept ToPathFS = requires(T t) {
		{ static_cast<std::filesystem::path>(t) } -> std::same_as<std::filesystem::path>;
	};
}

class FSWException : public std::exception {
private:
	std::string Message;
public:
	const char* what() const noexcept override {
		return Message.c_str();
	}
};

namespace fsworker {
	
}

#endif