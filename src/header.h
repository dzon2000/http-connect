#ifndef HEADER_H
#define HEADER_H

#include <string>
#include "method_type.h"
struct Header {
	method_type method;
	std::string host;
	std::string path;
	std::string port;
};
#endif

