#include "request.h"
#include "../utils/strings.h"
#include <sstream>

std::string Request::get_query(const std::string &key) const {
  auto it = query_params.find(key);
  return it != query_params.end() ? it->second : "";
}

std::vector<std::string> Request::get_query_all(const std::string &key) const {
  std::vector<std::string> values;
  auto range = query_params.equal_range(key);
  for (auto it = range.first; it != range.second; ++it) {
    values.push_back(it->second);
  }
  return values;
}

bool Request::has_query(const std::string &key) const {
  return query_params.find(key) != query_params.end();
}

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

Request &Request::set_full_uri(const std::string &u) {
  full_uri_str = u;
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
std::string Request::get_full_uri() const { return full_uri_str; }

std::unordered_map<std::string, std::string> Request::get_headers() const {
  return headers;
}
std::string Request::get_body() const { return body; }

std::string Request::get_header(const std::string &key) const {
  auto it = headers.find(key);
  return it != headers.end() ? it->second : "";
}

static std::unordered_multimap<std::string, std::string>
parse_query(const std::string &qs) {
  std::unordered_multimap<std::string, std::string> result;
  std::istringstream stream(qs);
  std::string token;

  while (std::getline(stream, token, '&')) {
    if (token.empty())
      continue;

    auto eq = token.find('=');
    if (eq != std::string::npos) {
      std::string key = url_decode(token.substr(0, eq));
      std::string value = url_decode(token.substr(eq + 1));
      result.emplace(key, value);
    } else {
      // Flag param e.g. ?readonly
      result.emplace(url_decode(token), "");
    }
  }

  return result;
}

Request Request::from_string(const std::string &raw) {
  if (raw.empty()) {
    return Request();
  }

  Request req;
  std::istringstream stream(raw);
  std::string line;

  if (std::getline(stream, line)) {
    if (!line.empty() && line.back() == '\r')
      line.pop_back();

    std::istringstream request_line(line);
    std::string method_str, uri_str, version_str;

    request_line >> method_str >> uri_str >> version_str;

    std::string query_string = "";
    req.full_uri_str = uri_str;

    auto query_pos = uri_str.find('?');
    if (query_pos != std::string::npos) {
      query_string = uri_str.substr(query_pos + 1);
      req.query_params = parse_query(query_string);
      req.uri = uri_str.substr(0, query_pos);
    } else {
      req.uri = uri_str;
    }

    req.method = method_from_string(method_str);
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

  auto cookie_header = req.get_header("Cookie");
  if (!cookie_header.empty()) {
    req.cookies = Cookies::from_header(cookie_header);
  }

  return req;
}

std::string Request::get_param(const std::string &key) const {
  auto it = params.find(key);
  return it != params.end() ? it->second : "";
}

Request &Request::set_param(const std::string &key, const std::string &value) {
  params[key] = value;
  return *this;
}
