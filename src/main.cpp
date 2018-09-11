#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <sstream>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <regex>

using namespace std;

enum method_type {
	GET,
	POST
};

struct Header {
	method_type method;
	string host;
	string path;
	string port;
};

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

class Url {
	static string REGEX;
	string url;
	string host;
	string port;
	string path;

public:
	Url(string);
	ConnectionStream open();
	string ToString();
};

string Url::REGEX = "https?:\\/\\/([a-zA-Z0-9\\.]+):?(\\d*)(.*)";

Url::Url(string urlStr) {
	regex rx(REGEX);
	url = urlStr;
	smatch match;
	if (regex_search(url, match, rx)) {
		host = match.str(1);
		port = match.str(2);
		path = match.str(3);
	} else {
		throw "Mismatched URL";
	}
}

string Url::ToString() {
	stringstream ss;
	ss << "Host: " << host << endl << "Port: " << port << endl << "Path: " << path << endl;
	return ss.str();
}


ConnectionStream Url::open() {
	struct addrinfo hints;
	struct addrinfo *result;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
	hints.ai_protocol = 0; /* Any protocol */

	int addr_info;
	addr_info = getaddrinfo(host.c_str(), port.c_str(), &hints, &result);

	if (addr_info != 0) {
		throw "Address not resolved";
	}
	int socket_desc = socket(result->ai_family, result->ai_socktype,
			result->ai_protocol);

	if (socket_desc == -1) {
		throw "Could not create socket";
	}
	int connect_status = connect(socket_desc, result->ai_addr,
			result->ai_addrlen);
	if (connect_status == -1) {
		throw "Could not connect socket";
	}
	return ConnectionStream(socket_desc, host, port, path);
}

int main(int argc, char *argv[]) {
	string s (argv[1]);
	Url url(s);
	cout << url.ToString() << endl;
	url.open().send("not used");
	return 0;
}
