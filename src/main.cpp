#include "app/app.h"

int main() {
  Server server =
      Server().default_server().set_port(8000).set_retry_port_bind(true);

  server.run();
  return 0;
}
