#pragma once

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
std::string status_code_to_string(HttpResponseCode);

enum class Httpversion : std::uint8_t {
  HTTP_1_0 = 0,
  HTTP_1_1 = 1,
  HTTP_2_0 = 2,
  HTTP_2_1 = 3,
  HTTP_3_0 = 4,
};

std::string version_to_string(Httpversion);
Httpversion version_from_string(const std::string &version);

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
std::string method_to_string(HttpMethod);
HttpMethod method_from_string(const std::string &method);
