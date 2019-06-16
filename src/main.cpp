#include <iostream>

#include "utils.h"
#include "custom_exceptions.h"
#include "tcp_server.h"
#include "database.h"

int main(){
	Tcp_server::set_ip_address( "0.0.0.0" );
	Tcp_server::set_port( 9100 );
	Tcp_server::set_max_connections( 50 );
	Tcp_server::set_message_length( 10 );
	Tcp_server::set_number_of_consumers( 3 );

	auto& server = Tcp_server::get_instance();
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
			"SELECT Events.Id, datetime(Events.Date, 'unixepoch') AS Date, Events.Title AS Title, "
			"Events.Description AS Description, group_concat(Tags_list.Tag) AS Tags, Events.author "
			"AS Author FROM ( SELECT DISTINCT events.id FROM Events JOIN Tags_of_events ON Events.id "
			"= Tags_of_events.Event_id JOIN ( WITH RECURSIVE nodes(tag_id) AS ( SELECT Tags_tree.id "
			"FROM Tags_tree WHERE Tags_tree.id IN (SELECT id FROM Tags_list) UNION ALL SELECT Tags_tree.id "
			"FROM Tags_tree JOIN nodes ON Tags_tree.parent_id = nodes.tag_id) SELECT Tags_list.id AS id FROM "
			"Tags_list JOIN (SELECT DISTINCT tag_id FROM nodes) tmp_table ON tmp_table.tag_id = Tags_list.id) "
			"tags_ids ON Tags_of_events.Tag_id = tags_ids.id) sub JOIN Tags_of_events ON sub.Id = "
			"Tags_of_events.Event_id JOIN Tags_list ON Tags_of_events.Tag_id = Tags_list.id JOIN Events ON "
			"sub.Id = Events.Id GROUP BY sub.Id ORDER BY Events.Date DESC;"
			, [](Result_set&& ans){
				std::cout << ans << std::endl;
		});
	} catch(Database_error& e){
		utils::err_log(e.what());
	}

	std::cout << "\nExit program" << std::endl;
	return 0;
}
