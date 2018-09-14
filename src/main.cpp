#include <iostream>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "url.h"
#include <thread>

using namespace std;

void runIt(string s) {
	Url url(s);
	cout << url.toString() << endl;
	Request req = url.open();
	req.addRequestHeader("Accept", "application/json");
	req.addRequestHeader("Content-Type", "application/json");
	req.addRequestHeader("AppKey", "a99a2921-a80c-4a21-ae81-8a72a64461ac");
	req.post();
}

int main(int argc, char *argv[]) {
	string s (argv[1]);
	thread first (runIt, s);
	first.join();
	cout << "Main thread finished" << endl;
	return 0;
}
