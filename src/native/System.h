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

// Global storage for command line arguments
static std::vector<std::string> g_cmdArgs;

inline void GW_setArgs(int argc, char *argv[], int startIndex) {
  g_cmdArgs.clear();
  for (int i = startIndex; i < argc; i++) {
    g_cmdArgs.push_back(argv[i]);
  }
}

inline Value __nativeGetArgCount(const std::vector<Value> &args) {
  if (args.size() != 0) {
    throw std::runtime_error("[GW NATIVE] getArgCount() takes 0 arguments");
  }
  return Value((int)g_cmdArgs.size());
}

inline Value __nativeGetArg(const std::vector<Value> &args) {
  if (args.size() != 1) {
    throw std::runtime_error("[GW NATIVE] getArg(index) takes 1 argument");
  }
  int index = args[0].asInt();
  if (index < 0 || index >= (int)g_cmdArgs.size()) {
    throw std::runtime_error("[GW NATIVE] Argument index out of bounds");
  }
  return Value(g_cmdArgs[index]);
}

#endif // SYSTEM_H