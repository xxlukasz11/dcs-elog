#include <utility>
#include "administrator.h"
#include "result_set.h"
#include "prepared_statement.h"
#include "update_tag_procedure.h"

void Update_tag_procedure::start() {
	Update_tag_query query = prepare_query();
	update_tag(query);
	log_response_message(response_);
	send_response(std::move(response_));
}

std::string Update_tag_procedure::name() {
	return "UPDATE_TAG_PROCEDURE";
}

void Update_tag_procedure::set_message(const std::shared_ptr<Message>& message) {
	message_ = std::static_pointer_cast<Update_tag_request>(message);
}

Update_tag_query Update_tag_procedure::prepare_query() const {
	Update_tag_query query;
	query.set_tag_id(message_->get_tag_id());
	query.set_tag_name(message_->get_new_name());
	return query;
}

void Update_tag_procedure::update_tag(const Update_tag_query& query) {
	Prepared_statement select_stmt = query.create_select_tag_statement();
	Prepared_statement update_stmt = query.create_update_statement();
	Database::Accessor accessor(database_);
	accessor.open();

	Result_set tag_exists_result = database_.execute(select_stmt);
	bool tag_exists = tag_exists_result.has_records();

	if (tag_exists) {
		auto empty_tag_name = Administrator::instance().params().get_empty_tag_name();
		std::string old_tag_name = tag_exists_result.get_first_field();
		if (old_tag_name != empty_tag_name) {
			Database::Transaction transaction(database_);
			database_.execute(update_stmt);
			transaction.commit();
			response_.set_success("Tag name has been changed from '" + old_tag_name + "' to '" + query.get_tag_name() + "'");
		}
		else {
			response_.set_failure("Cannot update reserved tag: '" + empty_tag_name + "'");
		}
	}
	else {
		response_.set_failure("Cannot update tag. It does not exist.");
	}
}
