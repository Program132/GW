#include "Value.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>

Value::Value()
    : type(VAL_NULL), intValue(0), structMembers(nullptr), structName("") {}

Value::Value(int value)
    : type(VAL_INTEGER), intValue(value), structMembers(nullptr),
      structName("") {}

Value::Value(double value)
    : type(VAL_NUMBER), doubleValue(value), structMembers(nullptr),
      structName("") {}

Value::Value(const std::string &value)
    : type(VAL_STRING), intValue(0), stringValue(value), structMembers(nullptr),
      structName("") {}

Value::Value(const char *value)
    : type(VAL_STRING), intValue(0), stringValue(value), structMembers(nullptr),
      structName("") {}

Value::Value(char value)
    : type(VAL_CHAR), charValue(value), structMembers(nullptr), structName("") {
}

Value::Value(bool value)
    : type(VAL_BOOLEAN), boolValue(value), structMembers(nullptr),
      structName("") {}

Value::Value(std::string structName, std::map<std::string, Value> *members)
    : type(VAL_STRUCT), intValue(0), structName(structName) {
  structMembers = std::make_shared<std::map<std::string, Value>>(*members);
}

Value::Value(ValueType type, const std::string &value)
    : type(type), intValue(0), stringValue(value), structMembers(nullptr),
      structName("") {
  if (type != VAL_CLASS) {
  }
}

Value::Value(NativeFunction func)
    : type(VAL_NATIVE_FUNCTION), intValue(0), structName(""), nativeFunc(func) {
}

Value::Value(const Value &other)
    : type(other.type), structMembers(other.structMembers),
      structName(other.structName) {
  switch (type) {
  case VAL_INTEGER:
    intValue = other.intValue;
    break;
  case VAL_NUMBER:
    doubleValue = other.doubleValue;
    break;
  case VAL_STRING:
  case VAL_CLASS:
    new (&stringValue) std::string(other.stringValue);
    break;
  case VAL_BOOLEAN:
    boolValue = other.boolValue;
    break;
  case VAL_CHAR:
    charValue = other.charValue;
    break;
  case VAL_STRUCT:
    // Shared pointer already copied in initializer list
    break;
  case VAL_NATIVE_FUNCTION:
    nativeFunc = other.nativeFunc;
    break;
  default:
    intValue = 0;
    break;
  }
}

Value &Value::operator=(const Value &other) {
  if (this != &other) {
    if (type == VAL_STRING || type == VAL_CLASS) {
      stringValue.~basic_string();
    }
    // structMembers handled by shared_ptr assignment below

    type = other.type;
    structName = other.structName;
    structMembers = other.structMembers;

    switch (type) {
    case VAL_INTEGER:
      intValue = other.intValue;
      break;
    case VAL_NUMBER:
      doubleValue = other.doubleValue;
      break;
    case VAL_STRING:
    case VAL_CLASS:
      new (&stringValue) std::string(other.stringValue);
      break;
    case VAL_BOOLEAN:
      boolValue = other.boolValue;
      break;
    case VAL_CHAR:
      charValue = other.charValue;
      break;
    case VAL_NATIVE_FUNCTION:
      nativeFunc = other.nativeFunc;
      break;
    default:
      intValue = 0;
      break;
    }
  }
  return *this;
}

Value::~Value() {
  if (type == VAL_STRING || type == VAL_CLASS) {
    stringValue.~basic_string();
  }
  // shared_ptr handles structMembers deletion
}

ValueType Value::getType() const { return type; }

int Value::asInt() const {
  if (type == VAL_INTEGER)
    return intValue;
  throw std::runtime_error("Value is not an integer");
}

double Value::asDouble() const {
  if (type == VAL_NUMBER)
    return doubleValue;
  if (type == VAL_INTEGER)
    return static_cast<double>(intValue);
  throw std::runtime_error("Value is not a number");
}

std::string Value::asString() const {
  if (type == VAL_STRING)
    return stringValue;
  if (type == VAL_CLASS)
    return stringValue;
  throw std::runtime_error("Value is not a string or class name");
}

bool Value::asBool() const {
  if (type == VAL_BOOLEAN)
    return boolValue;
  throw std::runtime_error("Value is not a boolean");
}

char Value::asChar() const {
  if (type == VAL_CHAR)
    return charValue;
  throw std::runtime_error("Value is not a character");
}

NativeFunction Value::asNativeFunction() const {
  if (type == VAL_NATIVE_FUNCTION)
    return nativeFunc;
  throw std::runtime_error("Value is not a native function");
}

bool Value::isInt() const { return type == VAL_INTEGER; }

bool Value::isDouble() const { return type == VAL_NUMBER; }

bool Value::isString() const { return type == VAL_STRING; }

bool Value::isBool() const { return type == VAL_BOOLEAN; }

bool Value::isChar() const { return type == VAL_CHAR; }

bool Value::isNull() const { return type == VAL_NULL; }

bool Value::isNumber() const {
  return type == VAL_INTEGER || type == VAL_NUMBER;
}

bool Value::isNativeFunction() const { return type == VAL_NATIVE_FUNCTION; }

std::string Value::toString() const {
  switch (type) {
  case VAL_INTEGER: {
    std::ostringstream oss;
    oss << intValue;
    return oss.str();
  }
  case VAL_NUMBER: {
    std::ostringstream oss;
    oss << doubleValue;
    return oss.str();
  }
  case VAL_STRING:
    return stringValue;
  case VAL_BOOLEAN:
    return boolValue ? "true" : "false";
  case VAL_CHAR:
    return std::string(1, charValue);
  case VAL_NULL:
    return "null";
  default:
    return "[Unknown]";
  }
}

double Value::toNumber() const {
  switch (type) {
  case VAL_INTEGER:
    return static_cast<double>(intValue);
  case VAL_NUMBER:
    return doubleValue;
  default:
    throw std::runtime_error("Cannot convert to number");
  }
}

Value Value::operator+(const Value &other) const {
  if (this->isString() && other.isString()) {
    return Value(this->asString() + other.asString());
  }
  if (this->isString() && other.isString()) {
    return Value(this->asString() + other.asString());
  }
  if (this->isString() && other.isChar()) {
    return Value(this->asString() + other.asChar());
  }
  if (this->isChar() && other.isString()) {
    return Value(this->asChar() + other.asString());
  }
  if (this->isString()) {
    return Value(this->asString() + other.toString());
  }

  if (this->isNumber() && other.isNumber()) {
    if (this->isInt() && other.isInt()) {
      return Value(this->asInt() + other.asInt());
    }
    return Value(this->toNumber() + other.toNumber());
  }

  throw std::runtime_error("Invalid operands for +");
}

Value Value::operator-(const Value &other) const {
  if (!this->isNumber() || !other.isNumber()) {
    throw std::runtime_error("Cannot subtract non-numeric values");
  }

  if (this->isInt() && other.isInt()) {
    return Value(this->asInt() - other.asInt());
  }
  return Value(this->toNumber() - other.toNumber());
}

Value Value::operator*(const Value &other) const {
  if (this->isString() && other.isInt()) {
    std::string result;
    int count = other.asInt();
    for (int i = 0; i < count; i++) {
      result += this->asString();
    }
    return Value(result);
  }
  if (this->isInt() && other.isString()) {
    std::string result;
    int count = this->asInt();
    for (int i = 0; i < count; i++) {
      result += other.asString();
    }
    return Value(result);
  }

  if (!this->isNumber() || !other.isNumber()) {
    throw std::runtime_error("Cannot multiply non-numeric values");
  }

  if (this->isInt() && other.isInt()) {
    return Value(this->asInt() * other.asInt());
  }
  return Value(this->toNumber() * other.toNumber());
}

Value Value::operator/(const Value &other) const {
  if (!this->isNumber() || !other.isNumber()) {
    throw std::runtime_error("Cannot divide non-numeric values");
  }

  double divisor = other.toNumber();
  if (divisor == 0) {
    throw std::runtime_error("Division by zero");
  }

  double result = this->toNumber() / divisor;
  if (this->isInt() && other.isInt() && result == static_cast<int>(result)) {
    return Value(static_cast<int>(result));
  }
  return Value(result);
}

Value Value::operator%(const Value &other) const {
  if (!this->isNumber() || !other.isNumber()) {
    throw std::runtime_error("Cannot modulo non-numeric values");
  }

  if (this->isInt() && other.isInt()) {
    int divisor = other.asInt();
    if (divisor == 0) {
      throw std::runtime_error("Modulo by zero");
    }
    return Value(this->asInt() % divisor);
  }

  double divisor = other.toNumber();
  if (divisor == 0) {
    throw std::runtime_error("Modulo by zero");
  }
  return Value(std::fmod(this->toNumber(), divisor));
}

Value Value::pow(const Value &other) const {
  if (!this->isNumber() || !other.isNumber()) {
    throw std::runtime_error("Cannot exponentiate non-numeric values");
  }

  double result = std::pow(this->toNumber(), other.toNumber());
  if (this->isInt() && other.isInt() && result == static_cast<int>(result)) {
    return Value(static_cast<int>(result));
  }
  return Value(result);
}

bool Value::operator==(const Value &other) const {
  if (this->type != other.type)
    return false;

  switch (type) {
  case VAL_INTEGER:
    return this->asInt() == other.asInt();
  case VAL_NUMBER:
    return this->asDouble() == other.asDouble();
  case VAL_STRING:
    return this->asString() == other.asString();
  case VAL_BOOLEAN:
    return this->asBool() == other.asBool();
  case VAL_CHAR:
    return this->asChar() == other.asChar();
  case VAL_NULL:
    return true;
  default:
    return false;
  }
}

bool Value::operator!=(const Value &other) const { return !(*this == other); }

bool Value::operator<(const Value &other) const {
  if (this->isNumber() && other.isNumber()) {
    return this->toNumber() < other.toNumber();
  }
  if (this->isString() && other.isString()) {
    return this->asString() < other.asString();
  }
  throw std::runtime_error("Invalid comparison");
}

bool Value::operator>(const Value &other) const {
  if (this->isNumber() && other.isNumber()) {
    return this->toNumber() > other.toNumber();
  }
  if (this->isString() && other.isString()) {
    return this->asString() > other.asString();
  }
  throw std::runtime_error("Invalid comparison");
}

bool Value::operator<=(const Value &other) const {
  return *this < other || *this == other;
}

bool Value::operator>=(const Value &other) const {
  return *this > other || *this == other;
}

Value Value::operator-() const {
  if (!this->isNumber()) {
    throw std::runtime_error("Cannot negate non-numeric value");
  }

  if (this->isInt()) {
    return Value(-this->asInt());
  }
  return Value(-this->asDouble());
}

Value Value::operator+() const {
  if (!this->isNumber()) {
    throw std::runtime_error("Cannot apply unary + to non-numeric value");
  }
  return *this;
}
