#ifndef _CREATE_LIBRARY_EVENT_PROCEDURE_H_
#define _CREATE_LIBRARY_EVENT_PROCEDURE_H_

#include <memory>
#include <string>
#include <vector>
#include "create_event_procedure.h"
#include "create_library_event_request.h"
#include "insert_query.h"

class Create_library_event_procedure : public Create_event_procedure {
public:
	using Create_event_procedure::Create_event_procedure;
	virtual std::string name() override final;

private:
	void leave_only_existing_tags(Insert_query& query, const std::vector<std::string>& not_existing_tags);
	std::string run_main_procedure(Insert_query& query);
};

#endif // !_CREATE_LIBRARY_EVENT_PROCEDURE_H_
