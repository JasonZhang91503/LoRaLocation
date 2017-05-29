/*

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

*/

#include <websocketpp/config/asio_no_tls.hpp>

#include <websocketpp/server.hpp>

#include<iostream>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include <string>
using namespace std;

struct Coor{
    int x;
    int y;
};

Coor parseStrongHold();


typedef websocketpp::server<websocketpp::config::asio> server;

int pipeFds[2];
char readBuff[256];
char sendBuff[256];


using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

// Define a callback to handle incoming messages
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
    
    std::cout << "on_message called with hdl: " << hdl.lock().get() 
              << " and message: " << msg->get_payload()
              << std::endl;

    try {
        s->send(hdl, msg->get_payload(), msg->get_opcode());
    } catch (const websocketpp::lib::error_code& e) {
        std::cout << "Echo failed because: " << e  
                  << "(" << e.message() << ")" << std::endl;
    }
    

    vector<Coor> coorVec;
    int state = 1;

    std::cout << msg->get_payload() << std::endl;
    cout << "BuildConnection" << endl;
    s->send(hdl, msg->get_payload(), msg->get_opcode());
/*
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
                s->send(hdl, sendBuff, websocketpp::frame::opcode::TEXT);
            }
            s->send(hdl, readBuff, websocketpp::frame::opcode::TEXT);
            break;
        case '3':
            s->send(hdl, readBuff, websocketpp::frame::opcode::TEXT);
            break;
        }

        //print_server.send(hdl, msg->get_payload(), msg->get_opcode());
    }
*/
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


	// Create a server endpoint
    server echo_server;
	
	try {
        // Set logging settings
        echo_server.set_access_channels(websocketpp::log::alevel::all);
        echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);
        
        // Initialize ASIO
        echo_server.init_asio();
        
        // Register our message handler
        echo_server.set_message_handler(bind(&on_message,&echo_server,::_1,::_2));
        
        // Listen on port 9002
        echo_server.listen(9002);
        
        // Start the server accept loop
        echo_server.start_accept();
	    
	    // Start the ASIO io_service run loop
        echo_server.run();
    } catch (const std::exception & e) {
        std::cout << e.what() << std::endl;
    } catch (websocketpp::lib::error_code e) {
        std::cout << e.message() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}