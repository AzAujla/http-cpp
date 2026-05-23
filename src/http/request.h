#pragma once
#include "cookies.h"
#include "http.h"
#include <unordered_map>

class Request {
private:
  HttpMethod method;
  Httpversion version;
  std::string uri;
  std::unordered_map<std::string, std::string> headers;
  std::string body = "";
  std::unordered_map<std::string, std::string> params;

public:
  Cookies cookies;

  Request &set_method(HttpMethod method);
  Request &set_version(Httpversion version);
  Request &set_uri(const std::string &uri);
  Request &
  set_headers(const std::unordered_map<std::string, std::string> &headers);
  Request &set_header(const std::string &key, const std::string &value);
  Request &set_body(const std::string &body);

  HttpMethod get_method() const;
  Httpversion get_version() const;
  std::string get_uri() const;
  std::unordered_map<std::string, std::string> get_headers() const;
  std::string get_header(const std::string &key) const;
  std::string get_body() const;

  Request &set_param(const std::string &key, const std::string &value);
  std::string get_param(const std::string &key) const;

  static Request from_string(const std::string &raw);
};
