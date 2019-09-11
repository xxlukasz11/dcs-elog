#ifndef _ADMINISTRATOR_H_
#define _ADMINISTRATOR_H_

#include <csignal>
#include "database.h"
#include "thread_manager.h"
#include "tcp_server.h"
#include "socket_queue.h"

class Administrator {
	Database database_;
	Thread_manager thread_manager_;
	Socket_queue socket_queue_;
	std::shared_ptr<Tcp_server> server_;

	bool initialized_{ false };
	bool started_{ false };

public:
	static Administrator& instance();
	void initialize();
	void start();
	void stop();

private:
	void setup_server();
	void prepare_threads();
	void on_exit();
	static void handle_sigint(int);

	Administrator();
	Administrator(const Administrator&) = default;
	Administrator(Administrator&&) = default;
	Administrator& operator=(const Administrator&) = default;
	Administrator& operator=(Administrator&&) = default;
};

#endif // !_ADMINISTRATOR_H_
