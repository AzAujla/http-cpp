#include "app/app.h"
#include "routes.h"
#include <iostream>

int main() {
  Server server = Server()
                      .default_server()
                      .set_port(8000)
                      .set_retry_port_bind(true)
                      .set_router(get_routes());

#ifndef NDEBUG
  std::cout << "In Debug\n";
#endif
  server.run();
  return 0;
}
