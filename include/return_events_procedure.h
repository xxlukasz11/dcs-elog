#ifndef _RETURN_EVENTS_PROCEDURE_H_
#define _RETURN_EVENTS_PROCEDURE_H_

#include <memory>
#include "procedure.h"
#include "return_events_request.h"
#include "select_query.h"
#include "result_set.h"

/*
Read requested events from database and sends them. Triggers attachments sending
*/
class Return_events_procedure : public Procedure {
public:
	using Procedure::Procedure;
	virtual void start() override final;
	virtual std::string name() override final;
	virtual void set_message(const std::shared_ptr<Message>& message) override final;

private:
	Select_query prepare_query() const;
	Result_set load_events(const Select_query& query);
	Result_set load_attachments_info(const Select_query& query, const Result_set& events);
	void run_main_procedure(const Select_query& query);

	std::shared_ptr<Return_events_request> message_;
};

#endif // !_RETURN_EVENTS_PROCEDURE_H_
