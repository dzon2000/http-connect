#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <regex>

using namespace std;

class ConnectionStream {
	int socket_desc;
private:
	void send(string);
public:
	ConnectionStream(int);
	int readStream(char *buf);
};

ConnectionStream::ConnectionStream(int socket_des) {
	socket_desc = socket_des;

}

class Url {
	string url;
	string host;
	string port;

public:
	Url(string);
	ConnectionStream open();
};

Url::Url(string urlStr) {
	url = urlStr;
//	regex regexp ("");
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
	return ConnectionStream(socket_desc);
}

int main() {
	regex rx("A|D");
	return 0;
}
