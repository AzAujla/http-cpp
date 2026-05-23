#pragma once

#include "../../http/http.h"
#include "../../http/request.h"
#include "../../http/response.h"
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

using Handler =
    std::variant<std::function<Response(Request)>, std::function<Response()>>;

class Route : public std::enable_shared_from_this<Route> {

private:
  std::string uri;
  bool optional;
  bool param;
  std::optional<HttpMethod> method;
  std::optional<Handler> handler;
  std::unordered_map<std::string, std::shared_ptr<Route>> children;
  std::unordered_map<std::string, std::shared_ptr<Route>> param_children;
  std::unordered_map<std::string, std::shared_ptr<Route>> optional_children;

  std::shared_ptr<Route>
  match_segments(const std::vector<std::string> &segments, size_t index,
                 std::unordered_map<std::string, std::string> &params);
  friend class Router;

public:
  void insert(HttpMethod method, const std::string &path, Handler handler);
  std::shared_ptr<Route>
  match(const std::string &path,
        std::unordered_map<std::string, std::string> &params);
  Route(const std::string &uri, bool optional = false, bool param = false);
};

class Router {
private:
  std::shared_ptr<Route> root;
  Response dispatch(Handler &handler, Request req);

public:
  Router();

  Response handle(Request request);
  Router &add_route(HttpMethod method, const std::string &uri, Handler handler);

  Router &get(const std::string &uri, Handler handler);
  Router &post(const std::string &uri, Handler handler);
  Router &put(const std::string &uri, Handler handler);
  Router &del(const std::string &uri, Handler handler);
  Router &head(const std::string &uri, Handler handler);
  Router &connect(const std::string &uri, Handler handler);
  Router &options(const std::string &uri, Handler handler);
  Router &trace(const std::string &uri, Handler handler);
  Router &patch(const std::string &uri, Handler handler);

  Router &file_dir(const std::string &uri, const std::string &dir);
  Router &nest(const std::string &uri, Router &router);
};
