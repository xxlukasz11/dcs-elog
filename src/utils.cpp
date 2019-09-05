#include "utils.h"

#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <utility>
#include <memory>

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

std::vector<std::string> utils::string_to_vector(std::string string) {
	std::replace(string.begin(), string.end(), ',', ' ');
	std::istringstream ss(std::move(string));
	std::vector<std::string> vector;
	for (std::string buffer; ss >> buffer;)
		vector.push_back(std::move(buffer));
	return vector;
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

void utils::log_recieved_message(int sockfd, const std::shared_ptr<Message>& message) {
	std::string log_message = "Recieved ";
	log_message += message->name();
	utils::out_log(sockfd, log_message);
}
