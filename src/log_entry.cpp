#include <ostream>
#include "log_entry.h"

std::ostream& operator<<(std::ostream& ostream, const Log_entry& entry) {
	ostream << entry.log_stream_.rdbuf();
	return ostream;
}
