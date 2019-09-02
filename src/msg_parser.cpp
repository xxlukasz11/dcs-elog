#include <sstream>
#include <algorithm>
#include <vector>
#include <string>

#include "msg_parser.h"
#include "custom_exceptions.h"

Msg_parser::Msg_parser(const std::string& msg) : msg_(msg) {
	std::string mode_str = raw_next();
	try {
		mode_ = std::stoi(mode_str);
	}
	catch (...) {
		throw Unknown_message_format();
	}
	std::istringstream ss( raw_next() );

	for(int buffer; ss >> buffer;){
		chunks_sizes_.push_back( buffer );
	}

	chunks_ = chunks_sizes_.size();
	current_chunk_ = 0;
}

bool Msg_parser::has_next() const {
	return chunks_ - current_chunk_ > 0;
}

int Msg_parser::get_mode() const {
	return mode_;
}

std::string Msg_parser::next() {
	if (!has_next())
		throw Msg_parser_exception("Cant obtain next chunk with index ", current_chunk_);

	auto x1 = msg_.find("[", position_);
	auto x2 = msg_.find("]", x1 + chunks_sizes_[current_chunk_]);
	position_ = x2;
	++current_chunk_;
	return msg_.substr(x1+1, x2-x1-1);
}

std::string Msg_parser::raw_next(){
	auto x1 = msg_.find("[", position_);
	auto x2 = msg_.find("]", x1);
	position_ = x2;
	return msg_.substr(x1+1, x2-x1-1);
}
