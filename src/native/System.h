#ifndef SYSTEM_H
#define SYSTEM_H

#include "../interpreter/value/Value.h"
#include <cstdlib>
#include <iostream>
#include <vector>

inline Value __nativeExecCmd(const std::vector<Value> &args) {
  if (args.size() != 1) {
    throw std::runtime_error("[GW NATIVE] __nativeExecCmd() takes 1 argument");
  }

  std::system(args[0].asString().c_str());

  return Value();
}

inline Value __nativeIsWindows(const std::vector<Value> &args) {
  if (args.size() != 0) {
    throw std::runtime_error(
        "[GW NATIVE] __nativeIsWindows() takes 0 arguments");
  }

#ifdef _WIN32
  return Value(true);
#else
  return Value(false);
#endif
}

inline Value __nativeIsLinux(const std::vector<Value> &args) {
  if (args.size() != 0) {
    throw std::runtime_error("[GW NATIVE] __nativeIsLinux() takes 0 arguments");
  }

#ifdef __linux__
  return Value(true);
#else
  return Value(false);
#endif
}

inline Value __nativeIsMac(const std::vector<Value> &args) {
  if (args.size() != 0) {
    throw std::runtime_error("[GW NATIVE] __nativeIsMac() takes 0 arguments");
  }

#ifdef __APPLE__
  return Value(true);
#else
  return Value(false);
#endif
}

inline Value __nativeExit(const std::vector<Value> &args) {
  if (args.size() != 1) {
    throw std::runtime_error("[GW NATIVE] __nativeExit() takes 1 argument");
  }

  exit(args[0].asInt());
  return Value();
}

#endif // SYSTEM_H