#ifndef NO_CAR_MODE
#include <iostream>
//Include WebSocket library
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#endif


#ifndef NO_CAR_MODE
typedef websocketpp::server<websocketpp::config::asio> server;
#endif


#ifndef NO_CAR_MODE
void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg)
{
    std::cout << msg->get_payload() << std::endl;
}

void* asyncWebSocketServer(void* param){
    cout << "asyncWebSocketServer start\n";
    /*
    server print_server;

    print_server.set_message_handler(&on_message);

    print_server.init_asio();
    print_server.listen(9002);
    print_server.start_accept();

    print_server.run();
    */
    utility_server s;
    s.run();

}

class utility_server {
public:
    utility_server() {
        m_endpoint.set_message_handler(&echo_handler);

        // Initialize Asio
        m_endpoint.init_asio();
    }
    

    void echo_handler(websocketpp::connection_hdl hdl, server::message_ptr msg) {
        // write a new message
        std::cout << msg->get_payload() << std::endl;
        m_endpoint.send(hdl, msg->get_payload(), msg->get_opcode());
    }

    void run() {
        // Listen on port 9002
        m_endpoint.listen(9002);

        // Queues a connection accept operation
        m_endpoint.start_accept();

        // Start the Asio io_service run loop
        m_endpoint.run();
    }
private:
    server m_endpoint;
};



#endif

