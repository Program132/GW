#include "DataTypes.h"

DataTypes toDataTypes(Token token) {
  std::string value = token.getValue();
  if (value == "Integer") {
    return DataTypes::Int;
  } else if (value == "Number") {
    return DataTypes::Number;
  } else if (value == "Boolean") {
    return DataTypes::Boolean;
  } else if (value == "String") {
    return DataTypes::String;
  } else if (value == "Character") {
    return DataTypes::Char;
  } else {
    return DataTypes::Null;
  }
}

std::string fromDataTypes(DataTypes dataType) {
  switch (dataType) {
  case DataTypes::Null:
    return "Null";
  case DataTypes::Int:
    return "Integer";
  case DataTypes::Number:
    return "Number";
  case DataTypes::Boolean:
    return "Boolean";
  case DataTypes::String:
    return "String";
  case DataTypes::Char:
    return "Character";
  default:
    return "Null";
  }
}