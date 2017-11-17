#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct Header header;

typedef enum {
	GET, POST
} method_type;

struct Header {
	method_type method;
	char *host;
	char *path;
	char *port;
};

char* buildRequest(header h) {
	char *method;
	if (h.method == GET) {
		method = "GET";
	} else {
		method = "POST";
	}
	int length = 11 + strlen(method) + strlen(h.host) + 1 + strlen(h.port) + strlen(h.path);
	char *request = malloc(length);
	sprintf(request, "%s http://%s:%s%s\r\n", method, h.host, h.port, h.path);
	return request;
}

header createHTTPHeader(char* host, char *port, char* path, method_type method) {
	header h;
	h.host = malloc(strlen(host));
	h.path = malloc(strlen(path));
	h.port = malloc(strlen(port));
	strcpy(h.host, host);
	strcpy(h.path, path);
	strcpy(h.port, port);
	h.method = method;
	return h;
}

int main() {
	char *host = "bla-opengrok.ptcnet.ptc.com";
	char *port = "80";
	char *path = "/raw/wnc-wnc.cairo-cps03/Windchill/DevModules/Carambola/src_web/config/actions/Carambola-actions.xml";
	header h = createHTTPHeader(host, port, path, GET);
	char *request = buildRequest(h);
	puts(request);

	struct addrinfo hints;
	struct addrinfo *result;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET; /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
	hints.ai_protocol = 0;          /* Any protocol */

	int addr_info;
	addr_info = getaddrinfo(h.host, h.port, &hints, &result);

	if (addr_info != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(addr_info));
		exit(EXIT_FAILURE);
	}

	int socket_desc = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

	if (socket_desc == -1) {
		printf("Could not create socket");
		return 0;
	}

	int connect_status = connect(socket_desc, result->ai_addr, result->ai_addrlen);
	if (connect_status == -1) {
		printf("Could not connect");
		return 0;
	}
	printf("Connected!\n");

	int sent_bytes = write(socket_desc, request, strlen(request));
	printf("Sent: %d\n", sent_bytes);

	char buf[2056];
	int byte_count;

	FILE *fp;
	fp = fopen("Carambola-actions.xml", "wb");

	while((byte_count = read(socket_desc, buf, sizeof(buf))) != 0) {
		fwrite(buf, byte_count, 1, fp);
		printf("%d\n", byte_count);
	}
	printf("Done\n");
	return 0;
}
