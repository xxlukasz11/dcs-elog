#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <string>

namespace config {

	namespace thread {
		extern int no_of_connection_handlers;
	}

	namespace connection {
		extern int tcp_message_length;
		extern int max_no_of_connections;
		extern int recieve_timeout_seconds;
	}

	namespace server {
		extern std::string ip_address;
		extern int port;
		extern int accept_delay_ms;
	}

	namespace database {
		extern std::string path;
		extern std::string empty_tag_name;
	}
}

#endif