#include "param_loader.h"
#include "param_manager.h"

Param_manager::Param_manager(const std::string& parameter_file_path) :
	parameter_file_path_(parameter_file_path) {
}

Parameters Param_manager::extract_parameters() {
	Param_loader loader(parameter_file_path_);
	loader.load();

	Parameters params;
	params.set_no_of_connection_handlers(
		loader.get_parameter<int>("no_of_connection_handlers")
	);
	params.set_tcp_message_length(
		loader.get_parameter<int>("tcp_message_length")
	);
	params.set_max_no_of_connections(
		loader.get_parameter<int>("max_no_of_connections")
	);
	params.set_receive_timeout_seconds(
		loader.get_parameter<int>("receive_timeout_seconds")
	);
	params.set_listening_port(
		loader.get_parameter<int>("listening_port")
	);
	params.set_connection_accept_delay_ms(
		loader.get_parameter<int>("connection_accept_delay_ms")
	);
	params.set_local_ip_address(
		loader.get_parameter<std::string>("local_ip_address")
	);
	params.set_database_path(
		loader.get_parameter<std::string>("database_path")
	);
	params.set_attachment_storage_path(
		loader.get_parameter<std::string>("attachment_storage_path")
	);
	params.set_empty_tag_name(
		loader.get_parameter<std::string>("empty_tag_name")
	);
	params.set_log_file_path(
		loader.get_parameter<std::string>("log_file_path")
	);
	params.set_logger_inactivity_timeout(
		std::chrono::seconds(
			loader.get_parameter<int>("logger_inactivity_timeout"))
	);
	params.set_log_level(
		utils::decode_log_level(
			loader.get_parameter<std::string>("log_level"))
	);

	return params;
}
