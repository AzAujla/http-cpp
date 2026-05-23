#pragma once

#include <arpa/inet.h>
#include <mutex>
#include <netinet/in.h>
#include <string>
#include <unistd.h>

/*
 * HTTP Server for the App
 * Uses a Builder Pattern
 */
class Server {
private:
  int port;
  sockaddr_in addr;
  bool retry_port_bind;
  int retries_for_port;
  static std::mutex cout_mutex;
  static void handle_client(int, sockaddr_in);

public:
  /*
   * Creates a default Server with
   * IP Address as INADDR_ANY
   * Port as 8000
   *
   * @return Server&
   */
  Server &default_server();

  /*
   * Whether to retry binding port if
   * it fails on the first attempt with
   * an incremented port number
   *
   * @param bool toggle
   * @return Server&
   */
  Server &set_retry_port_bind(bool);

  /*
   * How many times to retry binding the port
   *
   * @param int retry_count How many times to retry
   * @return Server&
   */
  Server &set_retries_for_port(int);

  /*
   * Port number for the server
   *
   * @param int port Port Number
   * @return Server&
   */
  Server &set_port(int);

  /*
   * Starts the server with the given configuration
   */
  int run();
};
