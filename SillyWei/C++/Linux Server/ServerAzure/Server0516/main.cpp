#include "Server.h"
#include "Socket.h"
#include "Cellphone.h"
#include "Gateway.h"

int main(int argc, const char **argv) {
    // sotre the socket value
	int intSocketListen, intSocketAccept;
	// use for socket set up
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	char s[INET6_ADDRSTRLEN];

	srand(time(NULL));

    // get the listen socket value
	intSocketListen = getInitializeListenSocket(their_addr, sin_size, s);
    if (intSocketListen == -1) {
        cout << endl << "Initialize intSocketListen fail" << endl;
        return -1;
    }
    cout << endl << "Initialize intSocketListen success" << endl;

    // initialize MySQL
    initializeMySQL();
    cout << "Initialize MySQL success" << endl;


    // begin gateway background task
    new thread(handleGatewayBack);


    cout << endl << "Waiting for the connections" << endl;
    // handle socket connections
    while (true) {
        // accept a new socket connection
        intSocketAccept = getAcceptSocket(intSocketListen, their_addr, sin_size, s);
        cout << endl << "Accept a new client: " << intSocketAccept << endl;

        // handle the connection
        new thread(handleNewConnection, intSocketAccept);
    }
    cout << endl << "Program end" << endl;

	return 0;
}
