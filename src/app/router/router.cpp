#include "router.h"
#include <sstream>

// Helpers
static std::vector<std::string> split_path(const std::string &path) {
  std::vector<std::string> segments;
  std::istringstream stream(path);
  std::string segment;

  while (std::getline(stream, segment, '/')) {
    if (!segment.empty())
      segments.push_back(segment);
  }

  return segments;
}

// Route
Route::Route(const std::string &uri, bool optional, bool param)
    : uri(uri), optional(optional), param(param) {}

void Route::insert(HttpMethod method, const std::string &path,
                   Handler handler) {
  std::vector<std::string> segments = split_path(path);
  Route *current = this;
  bool seen_optional = false; // ← track if we've seen an optional

  for (size_t i = 0; i < segments.size(); i++) {
    const std::string &segment = segments[i];
    bool is_last = (i == segments.size() - 1);

    // Enforce: nothing can come after an optional segment
    if (seen_optional) {
      throw std::invalid_argument("Invalid route '" + path +
                                  "': optional segments must be at the end");
    }

    if (segment[0] == ':') {
      std::string key = segment.substr(1);
      if (current->param_children.find(key) == current->param_children.end()) {
        current->param_children[key] =
            std::make_shared<Route>(key, false, true);
      }
      current = current->param_children[key].get();

    } else if (segment[0] == '?') {
      std::string key = segment.substr(1);
      seen_optional = true; // ← mark it
      if (current->optional_children.find(key) ==
          current->optional_children.end()) {
        current->optional_children[key] =
            std::make_shared<Route>(key, true, false);
      }
      current = current->optional_children[key].get();

    } else {
      if (current->children.find(segment) == current->children.end()) {
        current->children[segment] =
            std::make_shared<Route>(segment, false, false);
      }
      current = current->children[segment].get();
    }

    if (is_last) {
      current->method = method;
      current->handler = handler;
    }
  }
}

std::shared_ptr<Route>
Route::match(const std::string &path,
             std::unordered_map<std::string, std::string> &params) {
  std::vector<std::string> segments = split_path(path);
  return match_segments(segments, 0, params);
}

std::shared_ptr<Route>
Route::match_segments(const std::vector<std::string> &segments, size_t index,
                      std::unordered_map<std::string, std::string> &params) {
  // Consumed all segments
  if (index == segments.size()) {
    if (handler.has_value())
      return shared_from_this();

    // Even if this node has no handler, check if any optional
    // children have a handler — since they can be skipped
    for (auto &[key, child] : optional_children) {
      if (child->handler.has_value())
        return child;
    }

    return nullptr;
  }

  const std::string &segment = segments[index];

  // 1. Static children (most specific)
  auto static_it = children.find(segment);
  if (static_it != children.end()) {
    auto result =
        static_it->second->match_segments(segments, index + 1, params);
    if (result)
      return result;
  }

  // 2. Param children
  for (auto &[key, child] : param_children) {
    params[key] = segment;
    auto result = child->match_segments(segments, index + 1, params);
    if (result)
      return result;
    params.erase(key);
  }

  // 3. Optional children
  for (auto &[key, child] : optional_children) {
    // 3a. Segment matches the optional key — consume it
    if (segment == key) {
      auto result = child->match_segments(segments, index + 1, params);
      if (result)
        return result;
    }

    // 3b. Segment does NOT match — skip the optional entirely
    // and continue matching from the current index on the child's
    // children (not back to this node, avoiding infinite recursion)
    auto result = child->match_segments(segments, index, params);
    if (result)
      return result;
  }

  return nullptr;
}

// Router
Router::Router() : root(std::make_shared<Route>("", false, false)) {}

Router &Router::add_route(HttpMethod method, const std::string &uri,
                          Handler handler) {
  root->insert(method, uri, handler);
  return *this;
}

Response Router::dispatch(Handler &handler, Request req) {
  return std::visit(
      [&](auto &&fn) -> Response {
        using T = std::decay_t<decltype(fn)>;

        if constexpr (std::is_same_v<T, std::function<Response(Request)>>) {
          return fn(req);
        } else if constexpr (std::is_same_v<T, std::function<Response()>>) {
          return fn();
        } else {
          return Response()
              .set_code(HttpResponseCode::InternalServerError)
              .set_body("Unhandled handler type");
        }
      },
      handler);
}

Response Router::handle(Request request) {
  std::unordered_map<std::string, std::string> params;
  auto route = root->match(request.get_uri(), params);

  if (!route) {
    return Response()
        .set_code(HttpResponseCode::NotFound)
        .set_body("404 Not Found");
  }

  if (!route->method.has_value() ||
      route->method.value() != request.get_method()) {
    return Response()
        .set_code(HttpResponseCode::MethodNotAllowed)
        .set_body("405 Method Not Allowed");
  }

  // Inject params into request
  for (auto &[key, value] : params) {
    request.set_param(key, value);
  }

  return dispatch(*route->handler, request);
}

// HTTP method helpers
Router &Router::get(const std::string &uri, Handler handler) {
  return add_route(HttpMethod::Get, uri, handler);
}
Router &Router::post(const std::string &uri, Handler handler) {
  return add_route(HttpMethod::Post, uri, handler);
}
Router &Router::put(const std::string &uri, Handler handler) {
  return add_route(HttpMethod::Put, uri, handler);
}
Router &Router::del(const std::string &uri, Handler handler) {
  return add_route(HttpMethod::Delete, uri, handler);
}
Router &Router::head(const std::string &uri, Handler handler) {
  return add_route(HttpMethod::Head, uri, handler);
}
Router &Router::connect(const std::string &uri, Handler handler) {
  return add_route(HttpMethod::Connect, uri, handler);
}
Router &Router::options(const std::string &uri, Handler handler) {
  return add_route(HttpMethod::Options, uri, handler);
}
Router &Router::trace(const std::string &uri, Handler handler) {
  return add_route(HttpMethod::Trace, uri, handler);
}
Router &Router::patch(const std::string &uri, Handler handler) {
  return add_route(HttpMethod::Patch, uri, handler);
}

// Nest
Router &Router::nest(const std::string &uri, Router &router) {
  // TODO: merge trees directly for faster DFS
  std::vector<std::string> segments = split_path(uri);
  Route *current = root.get();

  for (const auto &segment : segments) {
    if (current->children.find(segment) == current->children.end()) {
      current->children[segment] =
          std::make_shared<Route>(segment, false, false);
    }
    current = current->children[segment].get();
  }

  // Merge sub-router's root children into the attachment node
  for (auto &[key, child] : router.root->children) {
    current->children[key] = child;
  }
  for (auto &[key, child] : router.root->param_children) {
    current->param_children[key] = child;
  }
  for (auto &[key, child] : router.root->optional_children) {
    current->optional_children[key] = child;
  }

  return *this;
}

Router &Router::file_dir(const std::string &uri, const std::string &dir) {
  // TODO: serve static files from dir
  return *this;
}
