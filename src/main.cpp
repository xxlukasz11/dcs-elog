#include <iostream>
#include "utils.h"
#include "custom_exceptions.h"
#include "database.h"
#include "config.h"
#include "administrator.h"

int main(){
	Administrator::instance().initialize();
	Administrator::instance().start();

	Database db(config::path::database);
	Database::Accessor accesor(db);
	accesor.open();
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
