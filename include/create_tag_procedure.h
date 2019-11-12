#ifndef _CREATE_TAG_PROCEDURE_H_
#define _CREATE_TAG_PROCEDURE_H_

#include <memory>
#include "procedure.h"
#include "create_tag_request.h"
#include "add_tag_query.h"
#include "website_response.h"

class Create_tag_procedure : public Procedure {
public:
	using Procedure::Procedure;
	virtual void start() override final;
	virtual std::string name() override final;
	virtual void set_message(const std::shared_ptr<Message>& message) override final;

private:
	Add_tag_query prepare_query() const;
	void create_tag(const Add_tag_query& query);

	Website_response response_;
	std::shared_ptr<Create_tag_request> message_;
};

#endif // !_CREATE_TAG_PROCEDURE_H_
