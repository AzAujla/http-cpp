#include "response.h"
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

std::string Response::build() {
  std::string response;

  response += version_to_string(version) + " ";
  response += std::to_string(static_cast<uint16_t>(code)) + " ";
  response += status_code_to_string(code) + "\r\n";

  for (auto &[key, value] : headers) {
    response += key + ": " + value + "\r\n";
  }

  response += "\r\n";
  response += body;

  return response;
}
