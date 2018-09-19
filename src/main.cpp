#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "url.h"
#include <thread>
#include "httpHeader.h"

using namespace std;

void runIt(string s) {
	Url url(s);
	cout << url.toString() << endl;
	Request req = url.open();
	req.addRequestHeader("Accept", "application/json");
	req.addRequestHeader("Content-Type", "application/json");
	req.addRequestHeader("AppKey", "662cfb2f-a22b-440b-8c1f-591bd2d5b286");
	req.post();
}

int main(int argc, char *argv[]) {
	string s (argv[1]);
	thread first (runIt, s);
	first.join();
	cout << "Main thread finished" << endl;
	return 0;
}
