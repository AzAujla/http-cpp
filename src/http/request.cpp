#include "request.h"
#include <sstream>

Request &Request::set_method(HttpMethod m) {
  method = m;
  return *this;
}

Request &Request::set_version(Httpversion v) {
  version = v;
  return *this;
}

Request &Request::set_uri(const std::string &u) {
  uri = u;
  return *this;
}

Request &
Request::set_headers(const std::unordered_map<std::string, std::string> &h) {
  headers = h;
  return *this;
}

Request &Request::set_header(const std::string &key, const std::string &value) {
  headers[key] = value;
  return *this;
}

Request &Request::set_body(const std::string &b) {
  body = b;
  return *this;
}

HttpMethod Request::get_method() const { return method; }
Httpversion Request::get_version() const { return version; }
std::string Request::get_uri() const { return uri; }
std::unordered_map<std::string, std::string> Request::get_headers() const {
  return headers;
}
std::string Request::get_body() const { return body; }

std::string Request::get_header(const std::string &key) const {
  auto it = headers.find(key);
  return it != headers.end() ? it->second : "";
}

Request Request::from_string(const std::string &raw) {
  Request req;
  std::istringstream stream(raw);
  std::string line;

  if (std::getline(stream, line)) {
    if (!line.empty() && line.back() == '\r')
      line.pop_back();

    std::istringstream request_line(line);
    std::string method_str, uri_str, version_str;

    request_line >> method_str >> uri_str >> version_str;

    req.method = method_from_string(method_str);
    req.uri = uri_str;
    req.version = version_from_string(version_str);
  }

  while (std::getline(stream, line)) {
    if (!line.empty() && line.back() == '\r')
      line.pop_back();
    if (line.empty())
      break;

    auto colon = line.find(':');
    if (colon != std::string::npos) {
      std::string key = line.substr(0, colon);
      std::string value = line.substr(colon + 2);
      req.headers[key] = value;
    }
  }

  std::string body_line;
  while (std::getline(stream, body_line)) {
    req.body += body_line + "\n";
  }

  // request.cpp — in from_string, after parsing headers
  auto cookie_header = req.get_header("Cookie");
  if (!cookie_header.empty()) {
    req.cookies = Cookies::from_header(cookie_header);
  }

  return req;
}
