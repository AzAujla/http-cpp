#include "session.h"
#include <chrono>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <string>
#include <thread>
#include <unordered_map>

void SessionStore::cleanup_loop() {
  while (running) {
    std::this_thread::sleep_for(std::chrono::seconds(cleanup_interval_seconds));
    sweep();
  }
}

void SessionStore::sweep() {
  std::unique_lock lock(mutex);
  auto now = Clock::now();

  for (auto it = store.begin(); it != store.end();) {
    if (now > it->second.expires_at) {
      it = store.erase(it);
    } else {
      ++it;
    }
  }
}

SessionStore::SessionStore(int ttl_seconds, int cleanup_interval_seconds)
    : ttl_seconds(ttl_seconds),
      cleanup_interval_seconds(cleanup_interval_seconds) {
  cleanup_thread = std::thread(&SessionStore::cleanup_loop, this);
}

SessionStore::~SessionStore() {
  running = false;
  cleanup_thread.join();
}

void SessionStore::set(const std::string &token, Session session) {
  session.expires_at = Clock::now() + std::chrono::seconds(ttl_seconds);
  std::unique_lock lock(mutex);
  store[token] = std::move(session);
}

std::optional<Session> SessionStore::get(const std::string &token) {
  std::unique_lock lock(mutex);
  auto it = store.find(token);
  if (it == store.end())
    return std::nullopt;

  if (Clock::now() > it->second.expires_at) {
    store.erase(it);
    return std::nullopt;
  }

  it->second.expires_at = Clock::now() + std::chrono::seconds(ttl_seconds);
  return it->second;
}

void SessionStore::remove(const std::string &token) {
  std::unique_lock lock(mutex);
  store.erase(token);
}

bool SessionStore::has(const std::string &token) {
  std::shared_lock lock(mutex);
  auto it = store.find(token);
  return it != store.end() && Clock::now() <= it->second.expires_at;
}

size_t SessionStore::size() const {
  std::shared_lock lock(mutex);
  return store.size();
}
