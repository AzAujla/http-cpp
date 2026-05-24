#pragma once

#include "cookies.h"
#include "http.h"
#include <string>
#include <unordered_map>

/*
 * Response Builder class
 */
class Response {
private:
  HttpResponseCode code;
  Httpversion version;
  std::unordered_map<std::string, std::string> headers;
  std::string body = "";

public:
  /*
   * Default HTTP Response
   * HTTP/1.1 200 OK
   * Connection: Close
   */
  Cookies cookies;

  Response();
  std::string build();
  Response &set_code(HttpResponseCode code);
  Response &set_version(Httpversion v);
  HttpResponseCode get_code() const;
  Httpversion get_version() const;
  Response &set_body(std::string);
};
