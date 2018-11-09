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

void runIt(string requestUrl, string content) {
	Url url(requestUrl);
	cout << url.toString() << endl;
	Request req = url.open();
	req.addRequestHeader("Content-Type", "application/json");
	req.addRequestHeader("Accept", "application/json");
	req.addRequestHeader("AppKey", "2ad57305-9331-4cbc-a6a2-613ffeb0c54c");
	Response resp = req.post(content);
	cout << "\nResponse:\n"; 
	cout << resp.toString() << endl;
}

int main(int argc, char *argv[]) {
	string url (argv[1]);
  string content = "";
	if (sizeof(argv) == 3)
		content = (argv[2]);
	thread first (runIt, url, content);
	first.join();
	cout << endl << "Main thread finished..." << endl;
	return 0;
}
