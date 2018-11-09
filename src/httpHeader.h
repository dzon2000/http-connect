#ifndef HTTPHEADER_H
#define HTTPHEADER_H

#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <iterator>
#include <regex>

template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
		*(result++) = item;
	}
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}

class HttpHeader {
	std::map<std::string, std::string> attributes;
	int status;
public:
	HttpHeader(int);
	HttpHeader();
	void addAttribute(std::string);
	std::string getAttribute(std::string);
	std::string toString();
};

HttpHeader::HttpHeader() {
}

HttpHeader::HttpHeader(int status) {
	this->status = status;
}

void HttpHeader::addAttribute(std::string attStr) {
	std::vector<std::string> splitt = split(attStr, ':');
	attributes[splitt.at(0)] = std::regex_replace(splitt.at(1), std::regex("^\\s+|\\s+$|(\\s)\\s+"), "$1");
}

std::string HttpHeader::getAttribute(std::string att) {
	return attributes[att];
}

std::string HttpHeader::toString() {
	std::stringstream ss;
	ss << "Status: " << status;
	for (auto const& x : attributes) {
		ss << x.first << " : " << x.second;
	}
	return ss.str();
}
#endif
