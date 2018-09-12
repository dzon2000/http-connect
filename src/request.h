#ifndef CONNECTIONSTREAM_H
#define CONNECTIONSTREAM_H

#include <string>
#include "header.h"
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
	RequestRequestLine header;
public:
	void send();
	Request(int, string, string, string);
	int readStream(char buf);
	RequestLine createRequestLine(string, string, string, method_type);
	string buildRequest(RequestLine);
};

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
	string method;
	if (h.method == GET) {
		method = "GET";
	} else {
		method = "POST";
	}
	stringstream ss;
	ss << method << " http://" << h.host << ":" << h.port << h.path << "\r\n";
	string request = ss.str();
	cout << request;
	return request;
}

void Request::send(string what) {
	RequestLine h = createRequestLine(host, port, path, GET);
	string request = buildRequest(h);
	char * req = new char [request.length()+1];
	strcpy (req, request.c_str());
	int sent_bytes = write(socket_desc, req, strlen(req));
	cout << "Sent: " << sent_bytes << endl;
	char buf[2056];
	int byte_count;
	while((byte_count = read(socket_desc, buf, sizeof(buf))) != 0) {
		cout << buf;
	}
}
#endif

