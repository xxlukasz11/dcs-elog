#ifndef _PARAM_MANAGER_H_
#define _PARAM_MANAGER_H_

#include <string>
#include "parameters.h"

/*
Creates parameter objects using param_loader
*/
class Param_manager {
public:
	Param_manager(const std::string& parameter_file_path);
	Parameters extract_parameters();

private:
	std::string parameter_file_path_;
};

#endif // !_PARAM_MANAGER_H_
