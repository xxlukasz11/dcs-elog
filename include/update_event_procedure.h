#ifndef _UPDATE_EVENT_PROCEDURE_H_
#define _UPDATE_EVENT_PROCEDURE_H_

#include <memory>
#include <string>
#include <vector>
#include "procedure.h"
#include "update_event_request.h"
#include "update_event_query.h"
#include "insert_query.h"

class Update_event_procedure : public Procedure {
public:
	Update_event_procedure(Database& database, const Socket& socket, const std::shared_ptr<Message>& message);
	virtual void start() override final;

private:
	Update_event_query prepare_update_query() const;
	Insert_query prepare_insert_query() const;
	std::string run_main_procedure(const Update_event_query& update_query, const Insert_query& insert_query);
	bool check_if_event_exists(const Prepared_statement& stmt);
	std::vector<std::string> load_not_existing_tags(const Prepared_statement& stmt);

	std::shared_ptr<Update_event_request> message_;
};

#endif // !_UPDATE_EVENT_PROCEDURE_H_

