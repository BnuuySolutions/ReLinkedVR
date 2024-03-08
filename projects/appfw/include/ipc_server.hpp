/*#pragma once
#define _WEBSOCKETPP_CPP11_TYPE_TRAITS_

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <iostream>
#include <fstream>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

typedef server::message_ptr message_ptr;

static void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
  if (msg->get_opcode() == websocketpp::frame::opcode::text) {
    s->get_alog().write(websocketpp::log::alevel::app,
      "Text Message Received: " + msg->get_payload());
  } else {
    s->get_alog().write(websocketpp::log::alevel::app,
      "Binary Message Received: " + websocketpp::utility::to_hex(msg->get_payload()));
  }

  try {
    s->send(hdl, msg->get_payload(), msg->get_opcode());
  } catch (websocketpp::exception const& e) {
    s->get_alog().write(websocketpp::log::alevel::app,
      std::string("Echo Failed: ") + e.what());
  }
}

static void initipc() {
  server ipc_server;
  try {
    // Set logging settings
    ipc_server.set_access_channels(websocketpp::log::alevel::all);
    ipc_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

    // Initialize Asio
    ipc_server.init_asio();

    // Register our message handler
    ipc_server.set_message_handler(bind(&on_message, &ipc_server, ::_1, ::_2));

    // Listen on port 9002
    ipc_server.listen(9002);

    // Start the server accept loop
    ipc_server.start_accept();

    // Start the ASIO io_service run loop
    ipc_server.run();
  } catch (websocketpp::exception const& e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
    std::cout << "other exception" << std::endl;
  }
}
*/