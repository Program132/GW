#ifndef GW_ASSERT_H
#define GW_ASSERT_H

#include <iostream>
#include <stdexcept>
#include <string>

#define GW_ASSERT(condition)                                                   \
  if (!(condition)) {                                                          \
    throw std::runtime_error("Assertion failed: " #condition);                 \
  }

#endif // GW_ASSERT_H