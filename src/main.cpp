#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "url.h"

using namespace std;

int main(int argc, char *argv[]) {
	string s (argv[1]);
	Url url(s);
	cout << url.toString() << endl;
	Request req = url.open();
	req.addRequestHeader("Accept", "application/json");
	req.get();

	return 0;
}
