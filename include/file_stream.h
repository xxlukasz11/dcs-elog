#ifndef _FILE_STREAM_H_
#define _FILE_STREAM_H_

#include <string>
#include <memory>
#include <utility>
#include <fstream>

/*
Convinient wrapper for file I/O operations
*/
class File_stream {
public:
	File_stream(std::string file_name);
	void open();
	void open_if_closed();
	void close();
	void flush();
	bool clear_file();

	template<typename T>
	void write(T&& element);
	template<typename T>
	void write_line(T&& element);
private:
	std::string file_name_;
	std::shared_ptr<std::ofstream> stream_;
};

template<typename T>
inline void File_stream::write(T&& element) {
	*stream_ << std::forward<T>(element);
}

template<typename T>
inline void File_stream::write_line(T&& element) {
	*stream_ << std::forward<T>(element) << "\n";
}

#endif // !_FILE_STREAM_H_
