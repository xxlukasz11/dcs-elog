#include <iostream>
#include <string>
#include "custom_exceptions.h"
#include "database.h"
#include "administrator.h"
#include "param_manager.h"

Parameters load_parameters(const std::string& parameter_file_path) {
	Param_manager param_manager(parameter_file_path);
	return param_manager.extract_parameters();
}

void assert_valid_no_of_args(int args_count) {
	const int expected_no_of_args = 1;
	if (args_count < expected_no_of_args + 1) {
		std::cerr << "Wrong nmber of invocation arguments. Expected " << expected_no_of_args
			<< " but " << args_count - 1 << " were provided\n";
		std::cerr << "Syntax is:\te_log <parameter file path>" << std::endl;
		exit(1);
	}
}

const char* get_parameter_file(char** argv) {
	return argv[1];
}

int main(int argc, char** argv){
	assert_valid_no_of_args(argc);
	try {
		const auto parameters = load_parameters(get_parameter_file(argv));
		Administrator::instance().initialize(parameters);
		Administrator::instance().start();
	} catch (const Base_exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}

	return 0;
}
