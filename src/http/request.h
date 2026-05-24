#pragma once
#include "cookies.h"
#include "http.h"
#include <unordered_map>
#include <vector>

class Request {
private:
  HttpMethod method;
  Httpversion version;
  std::string full_uri_str;
  std::string uri;
  std::unordered_map<std::string, std::string> headers;
  std::string body = "";
  std::unordered_map<std::string, std::string> params;
  std::unordered_multimap<std::string, std::string> query_params;

public:
  Cookies cookies;

  /*
   * First Value of the Key
   */
  std::string get_query(const std::string &key) const;
  /*
   * All Values of the Key
   */
  std::vector<std::string> get_query_all(const std::string &key) const;
  /*
   * Check if Key exists
   */
  bool has_query(const std::string &key) const;

  Request &set_method(HttpMethod method);
  Request &set_version(Httpversion version);
  Request &set_uri(const std::string &uri);
  Request &set_full_uri(const std::string &uri);
  Request &
  set_headers(const std::unordered_map<std::string, std::string> &headers);
  Request &set_header(const std::string &key, const std::string &value);
  Request &set_body(const std::string &body);

  HttpMethod get_method() const;
  Httpversion get_version() const;
  std::string get_uri() const;
  std::string get_full_uri() const;
  std::unordered_map<std::string, std::string> get_headers() const;
  std::string get_header(const std::string &key) const;
  std::string get_body() const;

  Request &set_param(const std::string &key, const std::string &value);
  std::string get_param(const std::string &key) const;

  static Request from_string(const std::string &raw);
};
