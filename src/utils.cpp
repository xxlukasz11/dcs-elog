#include "utils.h"

#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

std::string utils::concatenate_string_array(const std::vector<std::string>& array) {
	std::string text;
	size_t array_size = array.size();
	for (size_t i = 0; i < array_size; ++i) {
		text += array[i];
		if (i != array_size - 1)
			text += ", ";
	}
	return text;
}

std::string utils::get_date_time_str(){
	time_t abs_sec = std::time(nullptr);
	tm t_local = *std::localtime(&abs_sec);

	std::ostringstream ss;
	ss << std::put_time(&t_local, "%Y-%m-%d %H:%M:%S");
	
	return ss.str();
}

void utils::out_log(std::string message){
	std::string date = utils::get_date_time_str();
	std::cout << date << "\t" << message << std::endl;
}

void utils::err_log(std::string message){
	std::string date = utils::get_date_time_str();
	std::cerr << date << "\t" << message << std::endl;
}

void utils::out_log(int sockfd, std::string message){
	std::string date = utils::get_date_time_str();
	std::cout << date << "\t" << sockfd << ": " << message << std::endl;
}

void utils::err_log(int sockfd, std::string message){
	std::string date = utils::get_date_time_str();
	std::cerr << date << "\t" << sockfd << ": " << message << std::endl;
}