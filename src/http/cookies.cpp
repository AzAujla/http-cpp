#include "cookies.h"
#include "../utils/strings.h"
#include <sstream>
#include <stdexcept>

Cookie::Cookie(const std::string &key, const std::string &value)
    : key(key), value(value) {}

Cookie &Cookie::set_domain(const std::string &d) {
  domain = d;
  return *this;
}

Cookie &Cookie::set_path(const std::string &p) {
  path = p;
  return *this;
}

Cookie &Cookie::set_expires(const std::string &e) {
  expires = e;
  return *this;
}

Cookie &Cookie::set_max_age(int seconds) {
  max_age = seconds;
  return *this;
}

Cookie &Cookie::set_http_only(bool h) {
  http_only = h;
  return *this;
}

Cookie &Cookie::set_secure(bool s) {
  secure = s;
  return *this;
}

Cookie &Cookie::set_same_site(const std::string &s) {
  same_site = s;
  return *this;
}

std::string Cookie::get_key() const { return key; }
std::string Cookie::get_value() const { return value; }

std::string Cookie::to_header() const {
  std::ostringstream ss;

  ss << sanitize_cookie_key(key) << "=" << sanitize_cookie_key(value);

  if (domain)
    ss << "; Domain=" << *domain;
  if (path)
    ss << "; Path=" << *path;
  if (expires)
    ss << "; Expires=" << *expires;
  if (max_age)
    ss << "; Max-Age=" << *max_age;
  if (http_only)
    ss << "; HttpOnly";
  if (secure)
    ss << "; Secure";

  ss << "; SameSite=" << same_site;

  return ss.str();
}

// Cookies

Cookies &Cookies::add(const Cookie &cookie) {
  map.emplace(cookie.get_key(), cookie);
  return *this;
}

Cookies &Cookies::remove(const std::string &key) {
  map.erase(key);
  return *this;
}

bool Cookies::has(const std::string &key) const { return map.count(key) > 0; }

const Cookie &Cookies::get(const std::string &key) const {
  auto it = map.find(key);
  if (it == map.end())
    throw std::runtime_error("Cookie not found: " + key);
  return it->second;
}

const std::unordered_map<std::string, Cookie> &Cookies::all() const {
  return map;
}

Cookies Cookies::from_header(const std::string &header) {
  Cookies cookies;
  std::istringstream stream(header);
  std::string token;

  while (std::getline(stream, token, ';')) {
    size_t start = token.find_first_not_of(' ');
    if (start == std::string::npos)
      continue;
    token = token.substr(start);

    auto eq = token.find('=');
    if (eq == std::string::npos)
      continue;

    std::string key = token.substr(0, eq);
    std::string value = token.substr(eq + 1);

    cookies.add(Cookie(key, value));
  }

  return cookies;
}
