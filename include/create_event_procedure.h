#ifndef _CREATE_EVENT_PROCEDURE_H_
#define _CREATE_EVENT_PROCEDURE_H_

#include <memory>
#include <string>
#include <vector>
#include "procedure.h"
#include "create_event_request.h"
#include "insert_query.h"
#include "website_response.h"

class Create_event_procedure : public Procedure {
public:
	using Procedure::Procedure;
	virtual void start() override final;
	virtual std::string name() override final;
	virtual void set_message(const std::shared_ptr<Message>& message) override final;

private:
	std::vector<std::string> load_not_existing_tags(const Prepared_statement& stmt);
	Insert_query prepare_query() const;
	void run_main_procedure(Insert_query& query);
	void yeld_success(const std::string& event_id);

	Website_response response_;
	std::shared_ptr<Create_event_request> message_;
};

#endif // !_CREATE_EVENT_PROCEDURE_H_
