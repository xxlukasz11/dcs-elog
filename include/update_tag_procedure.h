#ifndef _UPDATE_TAG_PROCEDURE_H_
#define _UPDATE_TAG_PROCEDURE_H_

#include <memory>
#include <string>
#include "procedure.h"
#include "update_tag_request.h"
#include "update_tag_query.h"

class Update_tag_procedure : public Procedure {
public:
	Update_tag_procedure(Database& database, const Socket& socket, const std::shared_ptr<Message>& message);
	virtual void start() override final;

private:
	Update_tag_query prepare_query() const;
	std::string update_tag(const Update_tag_query& query);

	std::shared_ptr<Update_tag_request> message_;
};

#endif // !_UPDATE_TAG_PROCEDURE_H_

