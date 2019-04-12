#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include <algorithm>

#include "json.h"
#include "result_set.h"

std::string quote(const std::string& str){
	return "\"" + str + "\"";
}

std::string json_obj(const Result_set::header_type& header, const Result_set::header_type& row){
	std::string str_obj = "{";
	
	auto size = std::max(header.size(), row.size());
	for(size_t i = 0; i < size; ++i){
		str_obj += quote(header[i]);
		str_obj += ":";

		if(header[i] != "Id")
			str_obj += quote(row[i]);
		else
			str_obj += row[i];
		
		
		if(i != size -1){
			str_obj += ",";
		}
	}
	
	str_obj += "}";
	return str_obj;
}

std::string json_array(std::vector<std::string>&& objects){
	std::ostringstream str_array;
	str_array << "[";
	
	auto size = objects.size();
	for(size_t i = 0; i < size; ++i){
		str_array << std::move(objects[i]);
		if(i != size-1)
			str_array << ",";
	}
	
	str_array << "]";
	return str_array.str();
}

std::string get_array(const Result_set::header_type& header, const Result_set::data_type& data){
	auto size = data.size();
	std::vector<std::string> objs(size);
	for(size_t i = 0 ; i < size; ++i){
		objs[i] = json_obj(header, data[i]);
	}
	
	return json_array(std::move(objs));
}

std::string Json::stringify(const Result_set& result_set){
	return get_array(result_set.get_header(), result_set.get_data());
}


std::string Json::stringify(Result_set&& result_set){
	auto& h = const_cast<Result_set::header_type&>( result_set.get_header() );
	auto& d = const_cast<Result_set::data_type&>( result_set.get_data() );
	return get_array(h, d);
}