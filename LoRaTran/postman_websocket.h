
#ifndef NO_CAR_MODE
//Include WebSocket library
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#endif


#ifndef NO_CAR_MODE
typedef websocketpp::server<websocketpp::config::asio> server;
#endif


#ifndef NO_CAR_MODE
void on_message(websocketpp::connection_hdl hdl, server::message_ptr msg);

void* asyncWebSocketServer(void* param);
#endif

