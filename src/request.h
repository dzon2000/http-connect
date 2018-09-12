#ifndef CONNECTIONSTREAM_H
#define CONNECTIONSTREAM_H

#include <string>
#include <set>
#include "method_type.h"

using namespace std;

struct RequestLine {
	method_type method;
	std::string host;
	std::string path;
	std::string port;
};

class Request {
	int socket_desc;
	string host;
	string port;
	string path;
	RequestLine header;
	set<string> headers;
public:
	void addRequestHeader(string, string);
	void get();
	Request(int, string, string, string);
	int readStream(char buf);
	RequestLine createRequestLine(string, string, string, method_type);
	string buildRequest(RequestLine);
};

void Request::addRequestHeader(string key, string value) {
	stringstream ss;
	ss << key << ": " << value;
	string header = ss.str();
	headers.insert(header);
}

Request::Request(int socket_des, string host, string port, string path) {
	socket_desc = socket_des;
	this->host = host;
	this->port = port;
	this->path = path;
}

RequestLine Request::createRequestLine(string  host, string port, string path, method_type method) {
	RequestLine h;
	h.host = host;
	h.path = path;
	h.port = port;
	h.method = method;
	return h;
}

string Request::buildRequest(RequestLine h) {
	cout << "Size: " << headers.size() << endl;
	string method;
	if (h.method == GET) {
		method = "GET";
	} else {
		method = "POST";
	}
	stringstream ss;
	ss << method << " " << h.path << " HTTP/1.1\r\n";
	ss << "Host: " << h.host << ":" << h.port << "\r\n";
	for (auto it : headers) {
		ss << it << "\r\n";
	}
	ss << "\r\n";
	string request = ss.str();
	cout << request;
	return request;
}

void Request::get() {
	RequestLine h = createRequestLine(host, port, path, GET);
	string request = buildRequest(h);
	char * req = new char [request.length()+1];
	strcpy (req, request.c_str());
	int sent_bytes = send(socket_desc, req, strlen(req), 0);
	cout << "Sent: " << sent_bytes << endl;
	char buf[2056];
	try {
		int byte_count;
		while((byte_count = read(socket_desc, buf, sizeof(buf))) != 0) {
			cout << buf;
		}
	} catch (const exception& e) {
		cout << e.what() << endl;
	}
}
#endif

