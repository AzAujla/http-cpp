#include "response.h"
#include "../utils/strings.h"
#include "http.h"
#include <string>
#include <unordered_map>

Response::Response() {
  code = HttpResponseCode::OK;
  version = Httpversion::HTTP_1_1;
  headers["Connection"] = "close";
  this->headers["Content-Length"] = "0";
}

Response &Response::set_body(std::string text) {
  this->headers["Content-Type"] = "text/plain";
  this->headers["Content-Length"] = std::to_string(text.length());
  this->body = text;
  return *this;
}

Response &Response::set_code(HttpResponseCode code) {
  this->code = code;
  return *this;
}

Response &Response::set_version(Httpversion v) {
  version = v;
  return *this;
}

HttpResponseCode Response::get_code() const { return code; }

Httpversion Response::get_version() const { return version; }

std::string Response::build() {
  std::string response;

  response += version_to_string(version) + " ";
  response += std::to_string(static_cast<uint16_t>(code)) + " ";
  response += status_code_to_string(code) + "\r\n";

  for (auto &[key, value] : headers) {
    response += sanitize(key) + ": " + sanitize(value) + "\r\n";
  }
  for (auto &[key, cookie] : cookies.all()) {
    response += "Set-Cookie: " + cookie.to_header() + "\r\n";
  }

  response += "\r\n";
  response += body;

  return response;
}
