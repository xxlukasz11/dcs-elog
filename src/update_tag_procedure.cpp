#include "config.h"
#include "result_set.h"
#include "prepared_statement.h"
#include "update_tag_procedure.h"

Update_tag_procedure::Update_tag_procedure(Database& database, const Socket& socket, const std::shared_ptr<Message>& message)
	: Procedure(database, socket), message_(std::static_pointer_cast<Update_tag_request>(message)) {
}

void Update_tag_procedure::start() {
	Update_tag_query query = prepare_query();
	std::string response_message = update_tag(query);
	socket_.send_string(response_message);
}

std::string Update_tag_procedure::name() {
	return "UPDATE_TAG_PROCEDURE";
}

Update_tag_query Update_tag_procedure::prepare_query() const {
	Update_tag_query query;
	query.set_tag_id(message_->get_tag_id());
	query.set_tag_name(message_->get_new_name());
	return query;
}

std::string Update_tag_procedure::update_tag(const Update_tag_query& query) {
	Prepared_statement select_stmt = query.create_select_tag_statement();
	Prepared_statement update_stmt = query.create_update_statement();
	Database::Accessor accessor(database_);
	accessor.open();

	Result_set tag_exists_result = database_.execute(select_stmt);
	bool tag_exists = tag_exists_result.has_records();

	if (tag_exists) {
		std::string old_tag_name = tag_exists_result.get_first_field();
		if (old_tag_name != config::database::empty_tag_name) {
			Database::Transaction transaction(database_);
			database_.execute(update_stmt);
			transaction.commit();
			return "Tag name has been changed from '" + old_tag_name + "' to '" + query.get_tag_name() + "'";
		}
		else {
			return "Cannot update reserved tag: '" + config::database::empty_tag_name + "'";
		}
	}
	else {
		return "Cannot update tag. It does not exist.";
	}
}
