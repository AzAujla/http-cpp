#pragma once
#include <atomic>
#include <chrono>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>

using Clock = std::chrono::steady_clock;
using TimePoint = std::chrono::steady_clock::time_point;

struct Session {
  std::string username;
  TimePoint expires_at;
};

class SessionStore {
private:
  std::unordered_map<std::string, Session> store;
  mutable std::shared_mutex mutex;
  std::thread cleanup_thread;
  std::atomic<bool> running = true;
  int ttl_seconds;
  int cleanup_interval_seconds;

  void cleanup_loop();
  void sweep();

public:
  /*
   * @param ttl_seconds How long a session lives without activity
   * @param cleanup_interval How often the sweep thread runs (seconds)
   */
  SessionStore(int ttl_seconds = 3600, int cleanup_interval_seconds = 300);
  ~SessionStore();
  SessionStore(const SessionStore &) = delete;
  SessionStore &operator=(const SessionStore &) = delete;

  void set(const std::string &token, Session session);
  std::optional<Session> get(const std::string &token);
  void remove(const std::string &token);
  bool has(const std::string &token);
  size_t size() const;
};
