#ifndef VALUE_H
#define VALUE_H

#include <string>

enum ValueType {
  VAL_NULL,
  VAL_INTEGER,
  VAL_NUMBER,
  VAL_STRING,
  VAL_BOOLEAN,
  VAL_STRUCT,
  VAL_CLASS
};

class Value {
private:
  ValueType type;
  union {
    int intValue;
    double doubleValue;
    bool boolValue;
  };
  std::string stringValue;

public:
  // Constructeurs
  Value(); // Null
  Value(int value);
  Value(double value);
  Value(const std::string &value);
  Value(const char *value);
  Value(bool value);

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

  // Vérifications de type
  bool isInt() const;
  bool isDouble() const;
  bool isString() const;
  bool isBool() const;
  bool isNull() const;
  bool isNumber() const; // Int ou Double

  // Conversions
  std::string toString() const;
  double toNumber() const;

  // Opérations arithmétiques
  Value operator+(const Value &other) const;
  Value operator-(const Value &other) const;
  Value operator*(const Value &other) const;
  Value operator/(const Value &other) const;
  Value operator%(const Value &other) const;
  Value pow(const Value &other) const;

  // Opérations de comparaison
  bool operator==(const Value &other) const;
  bool operator!=(const Value &other) const;
  bool operator<(const Value &other) const;
  bool operator>(const Value &other) const;
  bool operator<=(const Value &other) const;
  bool operator>=(const Value &other) const;

  // Opérateur unaire
  Value operator-() const;
  Value operator+() const;
};

#endif // VALUE_H
