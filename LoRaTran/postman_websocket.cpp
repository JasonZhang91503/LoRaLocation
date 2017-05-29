#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

using namespace std;

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

server print_server;

int pipeFds[2];
char readBuff[256];

struct Coor{
    int x;
    int y;
}

void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg)
{
    vector<Coor> coorVec;

    std::cout << msg->get_payload() << std::endl;
    cout << "BuildConnection" << endl;

    while(1){
        read(pipeFds[0],readBuff,sizeof(readBuff));
        cout << "read from postman : "<< readBuff << endl;

        switch(readBuff[0]){
        case '1':
            coorVec.push_back(parseStrongHold());
            break;
        case '2':
            break;
        case '3':
            break;
        }

        //print_server.send(hdl, msg->get_payload(), msg->get_opcode());
    }

    
}

Coor parseStrongHold(){
	const char *d = " ,";
	char* eventNumStr;
    char* xStr;
    char* yStr;

	eventNumStr = strtok( readBuff ,d);
	xStr = strtok(NULL,d);
    yStr = strtok(NULL,d);

	
    Coor node = {atoi(xStr),atoi(yStr)};

    cout << atoi(xStr) << "  " << atoi(yStr) << endl;

	return node;
}

int main(int argc, const char * argv[]) {
    

    pipeFds[0] = atoi(argv[1]);
    pipeFds[1] = atoi(argv[2]);
    close(pipeFds[1]);


    print_server.set_message_handler(&on_message);

    print_server.init_asio();
    print_server.listen(9002);
    print_server.start_accept();

    print_server.run();
}