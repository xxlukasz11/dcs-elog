#ifndef _CREATE_TAG_PROCEDURE_H_
#define _CREATE_TAG_PROCEDURE_H_

#include <memory>
#include <string>
#include "procedure.h"
#include "create_tag_request.h"
#include "add_tag_query.h"

class Create_tag_procedure : public Procedure {
public:
	Create_tag_procedure(Database& database, const Socket& socket, const std::shared_ptr<Message>& message);
	virtual void start() override final;

private:
	Add_tag_query prepare_query() const;
	std::string create_tag(const Add_tag_query& query);

	std::shared_ptr<Create_tag_request> message_;
};

#endif // !_CREATE_TAG_PROCEDURE_H_
