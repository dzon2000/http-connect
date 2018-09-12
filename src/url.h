#ifndef URL_H
#define URL_H

#include <string>
#include <regex>
#include "request.h"
using namespace std;

class Url {
	static string REGEX;
	string url;
	string host;
	string port;
	string path;

public:
	Url(string);
	Request open();
	string toString();
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

string Url::toString() {
	stringstream ss;
	ss << "Host: " << host << endl << "Port: " << port << endl << "Path: " << path << endl;
	return ss.str();
}


Request Url::open() {
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
	return Request(socket_desc, host, port, path);
}
#endif
