#ifndef CONNECTIONSTREAM_H
#define CONNECTIONSTREAM_H

#include <string>
#include "header.h"
using namespace std;

class ConnectionStream {
	int socket_desc;
	string host;
	string port;
	string path;
public:
	void send(string);
	ConnectionStream(int, string, string, string);
	int readStream(char *buf);
	Header createHTTPHeader(string, string, string, method_type);
	string buildRequest(Header);
};

ConnectionStream::ConnectionStream(int socket_des, string host, string port, string path) {
	socket_desc = socket_des;
	this->host = host;
	this->port = port;
	this->path = path;
}

Header ConnectionStream::createHTTPHeader(string  host, string port, string path, method_type method) {
	Header h;
	h.host = host;
	h.path = path;
	h.port = port;
	h.method = method;
	return h;
}

string ConnectionStream::buildRequest(Header h) {
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

void ConnectionStream::send(string what) {
	Header h = createHTTPHeader(host, port, path, GET);
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

