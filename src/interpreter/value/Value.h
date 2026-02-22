#ifndef VALUE_H
#define VALUE_H

#include <map>
#include <memory>
#include <string>

enum ValueType {
  VAL_NULL,
  VAL_INTEGER,
  VAL_NUMBER,
  VAL_STRING,
  VAL_BOOLEAN,
  VAL_STRUCT,
  VAL_CLASS,
  VAL_CHAR,
  VAL_NATIVE_FUNCTION
};

#include <functional>
#include <vector>

class Value;
using NativeFunction = std::function<Value(const std::vector<Value> &)>;

class Value {
private:
  ValueType type;
  union {
    int intValue;
    double doubleValue;
    bool boolValue;
    char charValue;
  };
  std::string stringValue;
  std::shared_ptr<std::map<std::string, Value>> structMembers;
  std::string structName;
  std::vector<std::string>
      typeArgs; // Generic type arguments for classes/structs

  // Store native function
  NativeFunction nativeFunc;

  friend class Interpreter;

public:
  // Constructeurs
  Value(); // Null
  Value(int value);
  Value(double value);
  Value(const std::string &value);
  Value(const char *value);
  Value(char value);
  Value(bool value);
  Value(std::string structName, std::map<std::string, Value> *members);
  Value(std::string structName, std::map<std::string, Value> *members,
        std::vector<std::string> typeArgs);
  Value(ValueType type, const std::string &value);
  Value(NativeFunction func); // Constructor for native functions

  // Copy constructor et assignment operator
  Value(const Value &other);
  Value &operator=(const Value &other);

  // Destructor
  ~Value();

  // Getters
  ValueType getType() const;
  int asInt() const;
  double asDouble() const;
  std::string asString() const;
  bool asBool() const;
  char asChar() const;
  NativeFunction asNativeFunction() const;

  bool isInt() const;
  bool isDouble() const;
  bool isString() const;
  bool isBool() const;
  bool isChar() const;
  bool isNull() const;
  bool isNumber() const;
  bool isNativeFunction() const;

  std::string toString() const;
  double toNumber() const;

  Value operator+(const Value &other) const;
  Value operator-(const Value &other) const;
  Value operator*(const Value &other) const;
  Value operator/(const Value &other) const;
  Value operator%(const Value &other) const;
  Value pow(const Value &other) const;

  bool operator==(const Value &other) const;
  bool operator!=(const Value &other) const;
  bool operator<(const Value &other) const;
  bool operator>(const Value &other) const;
  bool operator<=(const Value &other) const;
  bool operator>=(const Value &other) const;

  Value operator-() const;
  Value operator+() const;
};

#endif // VALUE_H
