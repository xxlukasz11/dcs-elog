#include <iostream>
#include <string>
#include "custom_exceptions.h"
#include "database.h"
#include "administrator.h"
#include "param_manager.h"

Parameters load_parameters(const std::string& parameter_file_path) {
	Param_manager param_manager(parameter_file_path);
	return param_manager.extract_parameters();
}

void assert_valid_no_of_args(int args_count) {
	const int expected_no_of_args = 1;
	if (args_count < expected_no_of_args + 1) {
		std::cerr << "Wrong nmber of invocation arguments. Expected " << expected_no_of_args
			<< " but " << args_count - 1 << " were provided\n";
		std::cerr << "Syntax is:\te_log <parameter file path>" << std::endl;
		exit(1);
	}
}

const char* get_parameter_file(char** argv) {
	return argv[1];
}

int main(int argc, char** argv){
	assert_valid_no_of_args(argc);
	try {
		const auto parameters = load_parameters(get_parameter_file(argv));
		Administrator::instance().initialize(parameters);
		Administrator::instance().start();
	} catch (const Base_exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}
	
	#ifdef _DEBUG_
	Database db(Administrator::instance().params().get_database_path());
	try{
		Database::Accessor accesor(db);
		accesor.open();
		std::cout << std::endl;
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
		std::cout << e.what() << std::endl;
	}

	std::cout << "\nExit program" << std::endl;
	#endif
	return 0;
}
