#ifndef _UPDATE_TAG_PROCEDURE_H_
#define _UPDATE_TAG_PROCEDURE_H_

#include <memory>
#include <string>
#include "procedure.h"
#include "update_tag_request.h"
#include "update_tag_query.h"
#include "website_response.h"

/*
Updates tag in database and sends feedback
*/
class Update_tag_procedure : public Procedure {
public:
	using Procedure::Procedure;
	virtual void start() override final;
	virtual std::string name() override final;
	virtual void set_message(const std::shared_ptr<Message>& message) override final;

private:
	Update_tag_query prepare_query() const;
	void update_tag(const Update_tag_query& query);

	Website_response response_;
	std::shared_ptr<Update_tag_request> message_;
};

#endif // !_UPDATE_TAG_PROCEDURE_H_

