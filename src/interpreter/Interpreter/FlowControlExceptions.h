#ifndef FLOW_CONTROL_EXCEPTIONS_H
#define FLOW_CONTROL_EXCEPTIONS_H

#include <exception>

class BreakException : public std::exception {
public:
  const char *what() const noexcept override { return "Break Exception"; }
};

class ContinueException : public std::exception {
public:
  const char *what() const noexcept override { return "Continue Exception"; }
};

#endif // FLOW_CONTROL_EXCEPTIONS_H
