#include <iostream>

#include "utils.h"
#include "custom_exceptions.h"
#include "tcp_server.h"
#include "database.h"

int main(){

	auto& server = Tcp_server::get_instance();
	server.set_ip_address( "0.0.0.0" );
	server.set_port( 9100 );
	server.set_max_connections( 50 );
	server.set_message_length( 10 );
	server.set_number_of_consumers( 3 );

	try{
		server.initialize();
	}
	catch(Init_server_error& e){
		utils::err_log(e.what());
		exit(1);
	}
	
	server.start_server();
	server.join_threads();

	Database db("../resources/database.db");

	try{
		db.open();
	} catch(Database_error& e){
		utils::err_log(e.what());
		exit(1);
	}

	std::cout << std::endl;
	try{
		db.execute(
			"SELECT Events.Id, datetime(Events.Date, 'unixepoch') as Date, Events.Title, Events.Description, group_concat(Tags.Tag) as Tags "
			"FROM (SELECT DISTINCT events.id FROM Events JOIN Tags ON Events.id = Tags.Id) sub "
			"JOIN Tags ON sub.Id = Tags.Id JOIN Events ON sub.Id = Events.Id GROUP BY sub.Id;"
			, [](Result_set&& ans){
				std::cout << ans << std::endl;
		});
	} catch(Database_error& e){
		utils::err_log(e.what());
	}

	std::cout << "\nExit program" << std::endl;
	return 0;
}
