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

  struct HttpMethodHash {
    size_t operator()(HttpMethod m) const {
      return std::hash<uint8_t>()(static_cast<uint8_t>(m));
    }
  };

private:
  std::string uri;
  bool optional;
  bool param;
  std::unordered_map<HttpMethod, Handler, HttpMethodHash> handlers;
  std::unordered_map<std::string, std::shared_ptr<Route>> children;
  std::unordered_map<std::string, std::shared_ptr<Route>> param_children;
  std::unordered_map<std::string, std::shared_ptr<Route>> optional_children;

  std::shared_ptr<const Route>
  match_segments(const std::vector<std::string> &segments, size_t index,
                 std::unordered_map<std::string, std::string> &params) const;
  friend class Router;

public:
  void insert(HttpMethod method, const std::string &path, Handler handler);
  std::shared_ptr<const Route>
  match(const std::string &path,
        std::unordered_map<std::string, std::string> &params) const;
  Route(const std::string &uri, bool optional = false, bool param = false);
};

class Router {
private:
  std::shared_ptr<Route> root;
  Response dispatch(const Handler &handler, Request req) const;

public:
  Router();

  Response handle(Request request) const;
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
