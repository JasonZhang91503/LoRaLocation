/*

#ifndef NO_CAR_MODE
//Include WebSocket library
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#endif


#ifndef NO_CAR_MODE
void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg)
{
  	std::cout << msg->get_payload() << std::endl;
}

void* asyncWebSocketServer(void* param){
	server print_server;
  
	print_server.set_message_handler(&on_message);

	print_server.init_asio();
	print_server.listen(9002);
	print_server.start_accept();

	print_server.run();
}
#endif
*/