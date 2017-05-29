#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <string>
using namespace std;

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

struct Coor{
    int x;
    int y;
};

Coor parseStrongHold();

typedef websocketpp::server<websocketpp::config::asio> server;

server print_server;

int pipeFds[2];
char readBuff[256];
char sendBuff[256];

void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg)
{
    vector<Coor> coorVec;
    int state = 1;
    //std::string sendStr;

    std::cout << msg->get_payload() << std::endl;
    cout << "BuildConnection" << endl;
    print_server.send(hdl, msg->get_payload(), msg->get_opcode());

    while(1){
        read(pipeFds[0],readBuff,sizeof(readBuff));
        cout << "read from postman : "<< readBuff << endl;

        switch(readBuff[0]){
        case '1':
            if(state == 2){
                coorVec.clear();
                state = 1;
            }
            coorVec.push_back(parseStrongHold());
            break;
        case '2':
            if(state == 1){
                sendBuff[0] = 1;
                sendBuff[1] = coorVec.size();
                int count = 2;
                for(int i = 0; i < coorVec.size(); i++){
                    sendBuff[count] = coorVec[i].x;
                    count++;
                    sendBuff[count] = coorVec[i].y;
                    count++;
                }
                sendBuff[count] = '\0';
                state = 2;
                print_server.send(hdl, sendBuff, websocketpp::frame::opcode::TEXT);
            }
            print_server.send(hdl, readBuff, websocketpp::frame::opcode::TEXT);
            break;
        case '3':
            print_server.send(hdl, readBuff, websocketpp::frame::opcode::TEXT);
            break;
        }

        print_server.send(hdl, msg->get_payload(), msg->get_opcode());
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