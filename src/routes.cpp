#include "app/router/router.h"
#include "app/session/session.h"
#include "http/cookies.h"
#include "http/request.h"
#include "http/response.h"

static Response welcome(Request request,
                        std::shared_ptr<SessionStore> session_store) {

  if (!request.cookies.has("session")) {
    return Response().set_body("No Session. Please Login\n");
  }
  auto token = request.cookies.get("session").get_value();
  auto session = session_store->get(token);

  if (!session.has_value()) {
    std::string username = request.get_query("username");

    if (username.empty()) {
      return Response().set_body("Please Login\n");
    }

    Session new_session;
    new_session.username = username;
    session_store->set(token, new_session);

    Response response = Response().set_body("Welcome " + username + "\n");
    response.cookies.add(Cookie("session", token).set_http_only(true));
    return response;
  }

  std::string username = session->username;

  if (username.empty()) {
    return Response().set_body("Please Login\n");
  }

  session->username = "";
  session_store->set(token, *session);

  return Response().set_body("Hello " + username + "\n");
}

void main_routes(std::shared_ptr<Router> &router,
                 std::shared_ptr<SessionStore> session_store) {
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
      .get("/welcome",
           [session_store](Request request) {
             return welcome(request, session_store);
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

std::shared_ptr<const Router>
get_routes(std::shared_ptr<SessionStore> session_store) {
  auto router = std::make_shared<Router>();
  main_routes(router, session_store);

  std::shared_ptr<const Router> frozen =
      std::const_pointer_cast<const Router>(router);

  return frozen;
}
