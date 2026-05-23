#pragma once
#include <optional>
#include <string>
#include <unordered_map>

class Cookie {
private:
  std::string key;
  std::string value;
  std::optional<std::string> domain;
  std::optional<std::string> path;
  std::optional<std::string> expires;
  std::optional<int> max_age;
  bool http_only = false;
  bool secure = false;
  std::string same_site = "Lax";

public:
  /*
   * Cookie Builder function
   */
  Cookie(const std::string &key, const std::string &value);
  Cookie &set_domain(const std::string &domain);
  Cookie &set_path(const std::string &path);
  Cookie &set_expires(const std::string &expires);
  Cookie &set_max_age(int seconds);
  Cookie &set_http_only(bool http_only);
  Cookie &set_secure(bool secure);
  Cookie &set_same_site(const std::string &same_site);

  std::string get_key() const;
  std::string to_header() const; // serializes to Set-Cookie header value
};

class Cookies {
private:
  std::unordered_map<std::string, Cookie> map;

public:
  static Cookies from_header(const std::string &header);
  Cookies &add(const Cookie &cookie);
  Cookies &remove(const std::string &key);
  bool has(const std::string &key) const;
  const Cookie &get(const std::string &key) const;
  const std::unordered_map<std::string, Cookie> &all() const;
};
