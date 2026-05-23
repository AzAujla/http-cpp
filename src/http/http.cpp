#include <cstdint>
#include <string>
#include <unordered_map>

enum class HttpResponseCode : std::uint16_t {
  Continue = 100,
  SwitchingProtocols = 101,
  Processing = 102,
  EarlyHints = 103,
  OK = 200,
  Created = 201,
  Accepted = 202,
  NonAuthoritativeInformation = 203,
  NoContent = 204,
  ResetContent = 205,
  PartialContent = 206,
  MultiStatus = 207,
  AlreadyReported = 208,
  IMUsed = 226,
  MultipleChoices = 300,
  MovedPermanently = 301,
  Found = 302,
  SeeOther = 303,
  NotModified = 304,
  UseProxy = 305,
  TemporaryRedirect = 307,
  PermanentRedirect = 308,
  BadRequest = 400,
  Unauthorized = 401,
  PaymentRequired = 402,
  Forbidden = 403,
  NotFound = 404,
  MethodNotAllowed = 405,
  NotAcceptable = 406,
  ProxyAuthenticationRequired = 407,
  RequestTimeout = 408,
  Conflict = 409,
  Gone = 410,
  LengthRequired = 411,
  PreconditionFailed = 412,
  ContentTooLarge = 413,
  URITooLong = 414,
  UnsupportedMediaType = 415,
  RangeNotSatisfiable = 416,
  ExpectationFailed = 417,
  MisdirectedRequest = 421,
  UnprocessableContent = 422,
  Locked = 423,
  FailedDependency = 424,
  TooEarly = 425,
  UpgradeRequired = 426,
  PreconditionRequired = 428,
  TooManyRequests = 429,
  RequestHeaderFieldsTooLarge = 431,
  UnavailableForLegalReasons = 451,
  InternalServerError = 500,
  NotImplemented = 501,
  BadGateway = 502,
  ServiceUnavailable = 503,
  GatewayTimeout = 504,
  HTTPVersionNotSupported = 505,
  VariantAlsoNegotiates = 506,
  InsufficientStorage = 507,
  LoopDetected = 508,
  NetworkAuthenticationRequired = 511,
};

std::string status_code_to_string(HttpResponseCode code) {
  switch (static_cast<uint16_t>(code)) {
  case 100: {
    return "Continue";
    break;
  }
  case 101: {
    return "Switching Protocols";
    break;
  }
  case 102: {
    return "Processing";
    break;
  }
  case 103: {
    return "Early Hints";
    break;
  }
  case 200: {
    return "OK";
    break;
  }
  case 201: {
    return "Created";
    break;
  }
  case 202: {
    return "Accepted";
    break;
  }
  case 203: {
    return "Non-Authoritative Information";
    break;
  }
  case 204: {
    return "No Content";
    break;
  }
  case 205: {
    return "Reset Content";
    break;
  }
  case 206: {
    return "Partial Content";
    break;
  }
  case 207: {
    return "Multi-Status";
    break;
  }
  case 208: {
    return "Already Reported";
    break;
  }
  case 226: {
    return "IM Used";
    break;
  }
  case 300: {
    return "Multiple Choices";
    break;
  }
  case 301: {
    return "Moved Permanently";
    break;
  }
  case 302: {
    return "Found";
    break;
  }
  case 303: {
    return "See Other";
    break;
  }
  case 304: {
    return "Not Modified";
    break;
  }
  case 305: {
    return "Use Proxy";
    break;
  }
  case 307: {
    return "Temporary Redirect";
    break;
  }
  case 308: {
    return "Permanent Redirect";
    break;
  }
  case 400: {
    return "Bad Request";
    break;
  }
  case 401: {
    return "Unauthorized";
    break;
  }
  case 402: {
    return "Payment Required";
    break;
  }
  case 403: {
    return "Forbidden";
    break;
  }
  case 404: {
    return "Not Found";
    break;
  }
  case 405: {
    return "Method Not Allowed";
    break;
  }
  case 406: {
    return "Not Acceptable";
    break;
  }
  case 407: {
    return "Proxy Authentication Required";
    break;
  }
  case 408: {
    return "Request Timeout";
    break;
  }
  case 409: {
    return "Conflict";
    break;
  }
  case 410: {
    return "Gone";
    break;
  }
  case 411: {
    return "Length Required";
    break;
  }
  case 412: {
    return "Precondition Failed";
    break;
  }
  case 413: {
    return "Content Too Large";
    break;
  }
  case 414: {
    return "URI Too Long";
    break;
  }
  case 415: {
    return "Unsupported Media Type";
    break;
  }
  case 416: {
    return "Range Not Satisfiable";
    break;
  }
  case 417: {
    return "Expectation Failed";
    break;
  }
  case 421: {
    return "Misdirected Request";
    break;
  }
  case 422: {
    return "Unprocessable Content";
    break;
  }
  case 423: {
    return "Locked";
    break;
  }
  case 424: {
    return "Failed Dependency";
    break;
  }
  case 425: {
    return "Too Early";
    break;
  }
  case 426: {
    return "Upgrade Required";
    break;
  }
  case 428: {
    return "Precondition Required";
    break;
  }
  case 429: {
    return "Too Many Requests";
    break;
  }
  case 431: {
    return "Request Header Fields Too Large";
    break;
  }
  case 451: {
    return "Unavailable For Legal Reasons";
    break;
  }
  case 500: {
    return "Internal Server Error";
    break;
  }
  case 501: {
    return "Not Implemented";
    break;
  }
  case 502: {
    return "Bad Gateway";
    break;
  }
  case 503: {
    return "Service Unavailable";
    break;
  }
  case 504: {
    return "Gateway Timeout";
    break;
  }
  case 505: {
    return "HTTP Version Not Supported";
    break;
  }
  case 506: {
    return "Variant Also Negotiates";
    break;
  }
  case 507: {
    return "Insufficient Storage";
    break;
  }
  case 508: {
    return "Loop Detected";
    break;
  }
  case 511: {
    return "Network Authentication Required";
    break;
  }
  }
  return "";
}

enum class Httpversion : std::uint8_t {
  HTTP_1_0 = 0,
  HTTP_1_1 = 1,
  HTTP_2_0 = 2,
  HTTP_2_1 = 3,
  HTTP_3_0 = 4,
};

static const std::unordered_map<std::string, Httpversion> version_map = {
    {"HTTP/1.0", Httpversion::HTTP_1_0}, {"HTTP/1.1", Httpversion::HTTP_1_1},
    {"HTTP/2.0", Httpversion::HTTP_2_0}, {"HTTP/2.1", Httpversion::HTTP_2_1},
    {"HTTP/3.0", Httpversion::HTTP_3_0},
};

std::string version_to_string(Httpversion version) {
  switch (version) {
  case Httpversion::HTTP_1_0:
    return "HTTP/1.0";
    break;
  case Httpversion::HTTP_1_1:
    return "HTTP/1.1";
    break;
  case Httpversion::HTTP_2_0:
    return "HTTP/2.0";
    break;
  case Httpversion::HTTP_2_1:
    return "HTTP/2.1";
    break;
  case Httpversion::HTTP_3_0:
    return "HTTP/3.0";
    break;
  }
  return "";
}

Httpversion version_from_string(const std::string &version) {
  auto it = version_map.find(version);
  return it != version_map.end() ? it->second : Httpversion::HTTP_1_1;
}

enum class HttpMethod : std::uint8_t {
  Get = 0,
  Post = 1,
  Put = 2,
  Delete = 3,
  Head = 4,
  Connect = 5,
  Options = 6,
  Trace = 7,
  Patch = 8
};

std::string method_to_string(HttpMethod method) {
  switch (method) {
  case HttpMethod::Get:
    return "GET";
    break;
  case HttpMethod::Post:
    return "POST";
    break;
  case HttpMethod::Put:
    return "PUT";
    break;
  case HttpMethod::Delete:
    return "DELETE";
    break;
  case HttpMethod::Head:
    return "HEAD";
    break;
  case HttpMethod::Connect:
    return "CONNECT";
    break;
  case HttpMethod::Options:
    return "OPTIONS";
    break;
  case HttpMethod::Trace:
    return "TRACE";
    break;
  case HttpMethod::Patch:
    return "PATCH";
    break;
  }
  return "";
}

const std::unordered_map<std::string, HttpMethod> method_map = {
    {"GET", HttpMethod::Get},         {"POST", HttpMethod::Post},
    {"PUT", HttpMethod::Put},         {"DELETE", HttpMethod::Delete},
    {"HEAD", HttpMethod::Head},       {"CONNECT", HttpMethod::Connect},
    {"OPTIONS", HttpMethod::Options}, {"TRACE", HttpMethod::Trace},
    {"PATCH", HttpMethod::Patch},
};

HttpMethod method_from_string(std::string &method) {
  auto it = method_map.find(method);
  return it != method_map.end() ? it->second : HttpMethod::Get;
}
