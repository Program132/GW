#ifndef TIME_H
#define TIME_H

#include "../interpreter/value/Value.h"
#include <ctime>
#include <iostream>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

inline Value __nativeTime(const std::vector<Value> &args) {
  if (args.size() != 0) {
    throw std::runtime_error("[GW NATIVE] time() takes 0 argument");
  }
  return Value((int)std::time(nullptr));
}

inline Value __nativeTimeDouble(const std::vector<Value> &args) {
  if (args.size() != 0) {
    throw std::runtime_error("[GW NATIVE] time() takes 0 argument");
  }
  return Value((double)std::time(nullptr));
}

inline Value __nativeSleep(const std::vector<Value> &args) {
  if (args.size() != 1) {
    throw std::runtime_error("[GW NATIVE] sleep() takes 1 argument");
  }
  if (!args[0].isNumber()) {
    throw std::runtime_error(
        "[GW NATIVE] sleep() takes a number (int or double)");
  }
  int seconds = args[0].asInt();
#ifdef _WIN32
  Sleep(seconds * 1000);
#else
  sleep(seconds);
#endif
  return Value();
}

inline Value __nativeCTime(const std::vector<Value> &args) {
  std::time_t timestamp;
  if (args.size() == 0) {
    timestamp = std::time(nullptr);
  } else if (args.size() == 1) {
    if (!args[0].isNumber()) {
      throw std::runtime_error("[GW NATIVE] ctime() takes a number");
    }
    timestamp = (std::time_t)args[0]
                    .toNumber(); // Use toNumber to support large ints/double
  } else {
    throw std::runtime_error("[GW NATIVE] ctime() takes 0 or 1 argument");
  }

  char *dt = std::ctime(&timestamp);
  std::string s(dt);
  // ctime adds a newline at the end, remove it
  if (!s.empty() && s.back() == '\n') {
    s.pop_back();
  }
  return Value(s);
}

#endif