#include "app.h"
#include "../http/http.h"
#include "../http/request.h"
#include "../http/response.h"
#include <arpa/inet.h>
#include <iostream>
#include <mutex>
#include <netinet/in.h>
#include <string>
#include <thread>
#include <unistd.h>

Server &Server::default_server() {
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
  addr.sin_port = htons(this->port);
  return *this;
};

Server &Server::set_retry_port_bind(bool toggle) {
  this->retry_port_bind = toggle;
  return *this;
}

Server &Server::set_retries_for_port(int retry_count) {
  this->retries_for_port = retry_count;
  return *this;
}

Server &Server::set_port(int port) {
  this->port = port;
  addr.sin_port = htons(port);
  return *this;
}

Server &Server::set_router(std::shared_ptr<const Router> r) {
  router = r;
  return *this;
}

std::mutex Server::cout_mutex;

void Server::handle_client(int client_fd, sockaddr_in client_addr,
                           std::shared_ptr<const Router> router) {
  char buffer[4096] = {};
  ssize_t bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

  if (bytes > 0) {
    const Request request = Request::from_string(buffer);
    Response response = router->handle(request);

#ifndef NDEBUG
    {
      std::lock_guard<std::mutex> lock(cout_mutex);

      std::cout << inet_ntoa(client_addr.sin_addr) << " "
                << method_to_string(request.get_method()) << " "
                << request.get_full_uri() << " "
                << status_code_to_string(response.get_code()) << "\n";
    }
#endif // !NDEBUG

    std::string response_str = response.build();
    send(client_fd, response_str.c_str(), response_str.size(), 0);
  }
  close(client_fd);
}

int Server::run() {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    std::cerr << "Failed to create socket\n";
    exit(EXIT_FAILURE);
  }

  int opt = 1;
  // Allow port reuse
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  int bind_result = bind(server_fd, (sockaddr *)&addr, sizeof(addr));
  if (bind_result < 0) {
    if (this->retry_port_bind) {
      for (int i = 0; i < this->retries_for_port; i++) {
        this->port = this->port + 1;
        addr.sin_port = htons(this->port);
        int bind_result = bind(server_fd, (sockaddr *)&addr, sizeof(addr));
        if (bind_result >= 0) {
          break;
        } else if (i == this->retries_for_port - 1) {
          std::cerr << "Bind failed\n";
          exit(EXIT_FAILURE);
        }
      }
    } else {
      std::cerr << "Bind failed\n";
      exit(EXIT_FAILURE);
    }
  }

  if (listen(server_fd, 10) < 0) {
    std::cerr << "Listen failed\n";
    exit(EXIT_FAILURE);
  }

  std::cout << "Listening on port " << this->port << "\n";

  while (true) {
    sockaddr_in client_addr{};
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(server_fd, (sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {
      std::cerr << "Accept failed\n";
      continue;
    }

    std::thread(handle_client, client_fd, client_addr, router).detach();
  }
  close(server_fd);
  return 0;
}
