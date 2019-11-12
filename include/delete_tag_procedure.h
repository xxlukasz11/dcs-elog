#ifndef _DELETE_TAG_PROCEDURE_H_
#define _DELETE_TAG_PROCEDURE_H_

#include <memory>
#include <utility>
#include "procedure.h"
#include "delete_tag_request.h"
#include "delete_tag_query.h"
#include "website_response.h"

class Delete_tag_procedure : public Procedure {
public:
	using Procedure::Procedure;
	virtual void start() override final;
	virtual std::string name() override final;
	virtual void set_message(const std::shared_ptr<Message>& message) override final;

private:
	Delete_tag_query prepare_query() const;
	void run_main_procedure(const Delete_tag_query& query);
	bool check_if_tag_exists(const Result_set& result_set);
	std::string extract_tag_name(const Result_set& result_set);
	bool check_if_tag_is_reserved(const std::string tag_name);

	Website_response response_;
	std::shared_ptr<Delete_tag_request> message_;
};

#endif // !_DELETE_TAG_PROCEDURE_H_
