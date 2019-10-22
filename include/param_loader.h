#ifndef _PARAM_LOADER_H_
#define _PARAM_LOADER_H_

#include <string>
#include <unordered_map>
#include "custom_exceptions.h"
#include "utils.h"

class Param_loader {
public:
	Param_loader(const std::string& parameter_file_path);
	void load();

	template<typename T>
	T get_parameter(const std::string& param_name);

private:
	std::string parameter_fiile_path_;
	std::unordered_map<std::string, std::string> parameters_;
};

template<typename T>
inline T Param_loader::get_parameter(const std::string& param_name) {
	auto parameter = parameters_.find(param_name);
	if (parameter == parameters_.end()) {
		throw Load_parameters_error(
			"Param_loader error: Could not find ", param_name, " parameter in ", parameter_fiile_path_, " file");
	}
	try {
		auto value = utils::convert_string<T>(parameter->second);
		return value;
	} catch (const std::exception& e) {
		throw Load_parameters_error("Param_loader error: Could not convert ", param_name, " parameter");
	}
}

#endif // !_PARAM_LOADER_H_
