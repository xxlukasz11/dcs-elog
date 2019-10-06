#ifndef _RETURN_TAGS_TREE_PROCEDURE_H_
#define _RETURN_TAGS_TREE_PROCEDURE_H_

#include <memory>
#include "procedure.h"
#include "return_tags_tree_request.h"
#include "result_set.h"
#include "website_response.h"

class Return_tags_tree_procedure : public Procedure {
public:
	Return_tags_tree_procedure(Database& database, const Socket& socket, const std::shared_ptr<Message>& message);
	virtual void start() override final;
	virtual std::string name() override final;

private:
	Result_set load_tags_tree(const Prepared_statement& stmt);

	Website_response response_;
	std::shared_ptr<Return_tags_tree_request> message_;
};

#endif // !_RETURN_TAGS_TREE_PROCEDURE_H_
