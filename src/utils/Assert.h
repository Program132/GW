#ifndef GW_ASSERT_H
#define GW_ASSERT_H

#include <iostream>

#define GW_ASSERT(condition)                                                   \
  if (!(condition)) {                                                          \
    std::cerr << "Assertion failed: " << #condition << std::endl;              \
    std::abort();                                                              \
  }

#endif // GW_ASSERT_H