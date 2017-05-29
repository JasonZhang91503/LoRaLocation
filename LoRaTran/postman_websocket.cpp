#include<iostream>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

server print_server;

void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg)
{
    std::cout << msg->get_payload() << std::endl;
    print_server.send(hdl, msg->get_payload(), msg->get_opcode());
}

int main() {
    

    print_server.set_message_handler(&on_message);

    print_server.init_asio();
    print_server.listen(9002);
    print_server.start_accept();

    print_server.run();
}