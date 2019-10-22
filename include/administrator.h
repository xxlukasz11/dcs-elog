#ifndef _ADMINISTRATOR_H_
#define _ADMINISTRATOR_H_

#include <csignal>
#include "database.h"
#include "thread_manager.h"
#include "tcp_server.h"
#include "concurrent_queue.h"
#include "logger.h"
#include "parameters.h"

class Administrator {
public:
	static Administrator& instance();
	const Parameters& params() const;
	void initialize(const Parameters& parameters);
	void start();
	void stop();

private:
	void create_database();
	void stop_logger();
	void create_and_start_logger();
	void setup_server();
	void prepare_threads();
	void on_exit();
	static void handle_sigint(int);

	Administrator();
	Administrator(const Administrator&) = default;
	Administrator(Administrator&&) = default;
	Administrator& operator=(const Administrator&) = default;
	Administrator& operator=(Administrator&&) = default;

	Parameters parameters_;
	Thread_manager thread_manager_;
	Socket_queue socket_queue_;
	std::unique_ptr<Database> database_;
	std::shared_ptr<Tcp_server> server_;
	std::shared_ptr<Logger> logger_;
	bool initialized_{ false };
	bool started_{ false };
};

#endif // !_ADMINISTRATOR_H_
