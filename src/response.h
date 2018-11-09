#ifndef RESPONSE_H
#define RESPONSE_H

#include <string>
#include <sstream>

class Response {
  HttpHeader header;
  std::string body;
public:
  Response(HttpHeader, std::string);
  std::string toString();
};

Response::Response(HttpHeader header, std::string body) {
	this->header = header;
	this->body = body;
}

std::string Response::toString() {
  return this->header.toString() + "\n\n" + body;
}

#endif
