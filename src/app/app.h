#pragma once

#include "router/router.h"
#include "session/session.h"
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
  int port = 8000;
  sockaddr_in addr = {};
  bool retry_port_bind = false;
  int retries_for_port = 5;
  static std::mutex cout_mutex;
  static void handle_client(int, sockaddr_in, std::shared_ptr<const Router>,
                            std::shared_ptr<SessionStore> session_store);
  std::shared_ptr<const Router> router;
  std::shared_ptr<SessionStore> session_store;

public:
  /*
   * Creates a default Server with
   * IP Address as INADDR_ANY
   * Port as 8000
   *
   * @return Server&
   */
  Server &default_server();

  Server &set_router(std::shared_ptr<const Router> r);

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

  Server &enable_sessions(int ttl_seconds = 3600,
                          int cleanup_interval_seconds = 300);

  std::shared_ptr<SessionStore> get_session_store() const;
};
