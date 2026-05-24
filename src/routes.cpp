#include "app/router/router.h"
#include "http/request.h"
#include "http/response.h"

void main_routes(std::shared_ptr<Router> &router) {
  router
      ->get("/",
            [](Request request) {
              Response response = Response();
              if (request.cookies.has("Auth")) {
                response.set_body("Authenticated with " +
                                  request.cookies.get("Auth").get_value() +
                                  "\n");
              } else {
                response.set_body("Please login\n");
              }
              return response;
            })
      .post("/users", []() { return Response().set_body("Created user\n"); })
      .get("/users", []() { return Response().set_body("Listed All users\n"); })
      .get("/users/:id",
           [](Request request) {
             return Response().set_body("Retrieved User " +
                                        request.get_param("id") + "\n");
           })
      .put("/users/:id",
           [](Request request) {
             return Response().set_body("Updated User " +
                                        request.get_param("id") + "\n");
           })
      .del("/users/:id", [](Request request) {
        return Response().set_body("Deleted User " + request.get_param("id") +
                                   "\n");
      });
}

std::shared_ptr<const Router> get_routes() {
  auto router = std::make_shared<Router>();
  main_routes(router);

  std::shared_ptr<const Router> frozen =
      std::const_pointer_cast<const Router>(router);

  return frozen;
}
