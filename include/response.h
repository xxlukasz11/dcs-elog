#ifndef _RESPONSE_H_
#define _RESPONSE_H_

class Response {
public:
	virtual ~Response() = default;
	enum class Code {
		SUCCESS,
		FAILURE
	};
};

#endif // !_RESPONSE_H_
