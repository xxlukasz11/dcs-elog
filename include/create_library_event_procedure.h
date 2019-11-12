#ifndef _CREATE_LIBRARY_EVENT_PROCEDURE_H_
#define _CREATE_LIBRARY_EVENT_PROCEDURE_H_

#include <memory>
#include <string>
#include <vector>
#include "procedure.h"
#include "create_library_event_request.h"
#include "server_response.h"
#include "insert_query.h"

class Create_library_event_procedure : public Procedure {
public:
	using Procedure::Procedure;
	virtual void start() override final;
	virtual std::string name() override final;
	virtual void set_message(const std::shared_ptr<Message>& message) override final;

private:
	std::vector<std::string> load_not_existing_tags(const Prepared_statement& stmt);
	void leave_only_existing_tags(Insert_query& query, const std::vector<std::string>& not_existing_tags);
	Insert_query prepare_query() const;
	void run_main_procedure(Insert_query& query);

	Server_response response_;
	std::shared_ptr<Create_library_event_request> message_;
};

#endif // !_CREATE_LIBRARY_EVENT_PROCEDURE_H_
