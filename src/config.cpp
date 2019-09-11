#include "config.h"

namespace config {

	namespace thread {
		int no_of_connection_handlers = 3;
	}

	namespace connection {
		int tcp_message_length = 1024;
		int max_no_of_connections = 50;
		int recieve_timeout_seconds = 5;
	}

	namespace server {
		std::string ip_address = "0.0.0.0";
		int port = 9100;
		int accept_delay_ms = 50;
	}

	namespace database {
		std::string path = "../resources/database.db";
		std::string empty_tag_name = "-";
	}
}