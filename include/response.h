#ifndef _RESPONSE_H_
#define _RESPONSE_H_

/*
Base class for every E-Log response
*/
class Response {
public:
	virtual ~Response() = default;
	enum class Code {
		SUCCESS,
		FAILURE
	};
};

#endif // !_RESPONSE_H_
