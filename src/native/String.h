#ifndef STRING_H
#define STRING_H

#include "../interpreter/value/Value.h"
#include <iostream>
#include <vector>

inline Value __nativeStringLength(const std::vector<Value> &args) {
  if (args.size() != 1) {
    throw std::runtime_error("[GW NATIVE] stringLength() takes 1 argument");
  }
  if (!args[0].isString()) {
    throw std::runtime_error("[GW NATIVE] stringLength() takes a string");
  }
  return Value((int)args[0].asString().length());
}

inline Value __nativeStringAt(const std::vector<Value> &args) {
  if (args.size() != 2) {
    throw std::runtime_error("[GW NATIVE] stringAt() takes 2 arguments");
  }
  if (!args[0].isString() || !args[1].isInt()) {
    throw std::runtime_error(
        "[GW NATIVE] stringAt() takes a string and an int");
  }
  return Value(args[0].asString()[args[1].asInt()]);
}

#endif