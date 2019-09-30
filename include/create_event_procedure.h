#ifndef _CREATE_EVENT_PROCEDURE_H_
#define _CREATE_EVENT_PROCEDURE_H_

#include <memory>
#include <string>
#include <vector>
#include "procedure.h"
#include "create_event_request.h"
#include "insert_query.h"

class Create_event_procedure : public Procedure {
public:
	Create_event_procedure(Database& database, const Socket& socket, const std::shared_ptr<Message>& message);
	virtual void start() override final;
	virtual std::string name() override;

protected:
	std::vector<std::string> load_not_existing_tags(const Prepared_statement& stmt);

private:
	Insert_query prepare_query() const;
	virtual std::string run_main_procedure(Insert_query& query);

	std::shared_ptr<Create_event_request> message_;
};

#endif // !_CREATE_EVENT_PROCEDURE_H_
