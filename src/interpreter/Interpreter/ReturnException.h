#ifndef RETURN_EXCEPTION_H
#define RETURN_EXCEPTION_H

#include "../value/Value.h"
#include <exception>

class ReturnException : public std::exception {
public:
  Value value;
  ReturnException(Value value) : value(value) {}
  const char *what() const noexcept override { return "Return Exception"; }
};

#endif
