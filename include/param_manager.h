#ifndef _PARAM_MANAGER_H_
#define _PARAM_MANAGER_H_

#include <string>
#include "parameters.h"

class Param_manager {
public:
	Param_manager(const std::string& parameter_file_path);
	Parameters extract_pareameters();

private:
	std::string parameter_file_path_;
};

#endif // !_PARAM_MANAGER_H_
