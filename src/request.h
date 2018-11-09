#ifndef CONNECTIONSTREAM_H
#define CONNECTIONSTREAM_H

#include <string>
#include <set>
#include <sstream>
#include "method_type.h"
#include "httpHeader.h"
#include "response.h"

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
	Response get();
	Response post(string);
	Request(int, string, string, string);
	int readStream(char buf);
	RequestLine createRequestLine(string, string, string, method_type);
	string buildRequest(RequestLine, string);
private:
	Response make(method_type, string body="");
	HttpHeader readHeader();
	string readHeaderLine();
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

string Request::buildRequest(RequestLine h, string body) {
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
	ss << "Content-Length: " << std::to_string(body.size()) << "\r\n";
	ss << "\r\n";
	ss << body;
	string request = ss.str();
	cout << request;
	return request;
}

Response Request::get() {
	return make(GET);
}

Response Request::post(string body="") {
	return make(POST, body);
}

Response Request::make(method_type mt, string body) {
	RequestLine h = createRequestLine(host, port, path, mt);
	string request = buildRequest(h, body);
	int sent_bytes = send(socket_desc, request.data(), static_cast<ssize_t>(request.size()), 0);
	cout << endl << "Sent: " << sent_bytes << endl;
	char buf[2048];
	try {
		HttpHeader hh = readHeader();
		cout << hh.toString();
		int byte_count;
		while((byte_count = recv(socket_desc, buf, sizeof(buf), 0)) > 1) {
			cout << buf;
			if (buf[byte_count - 1] == '\n' && buf[byte_count - 2] == '\r') {
				break;
			}
		}
	} catch (const exception& e) {
		cout << e.what() << endl;
	}
	return Response();
}

HttpHeader Request::readHeader() {
	char buf[1];
	int byte_count;
	stringstream ss;
	HttpHeader hh;
	// Read http status
	while (true) {
		byte_count = recv(socket_desc, buf, sizeof(buf), 0);
		if (byte_count != 1)
			throw "No data to read";
		if (buf[0] == '\r') {
			string header = ss.str();
			regex rx("\\s([\\d]{3})");
			smatch match;
			if (regex_search(header, match, rx)) {
				hh = HttpHeader(stoi(match.str(1)));
			}
			ss.str(string());
			break;
		} else {
			ss << buf[0];
		}
	}
	string line;
	while ((line = readHeaderLine()).length() != 1) {
		hh.addAttribute(line);
	}
	return hh;
}

string Request::readHeaderLine() {
	stringstream ss;
	int byte_count;
	char buf[1];
	while (true) {
		byte_count = recv(socket_desc, buf, sizeof(buf), 0);
		if (byte_count != 1)
			throw "No data to read";
		if (buf[0] == '\r') {
			string header = ss.str();
			return ss.str();
		}
		ss << buf[0];
	}
	return "";
}

#endif
