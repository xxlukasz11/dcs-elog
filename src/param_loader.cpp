#include <fstream>
#include <string>
#include "param_loader.h"

Param_loader::Param_loader(const std::string& parameter_file_path) :
	parameter_fiile_path_(parameter_file_path) {
}

void Param_loader::load() {
	std::ifstream file(parameter_fiile_path_);
	if (!file.is_open()) {
		throw Load_parameters_error("Could not open parameter file: ", parameter_fiile_path_);
	}

	for (std::string param_name; file >> param_name;) {
		std::string param_value;
		file >> param_value;
		parameters_[param_name] = param_value;
	}
}
