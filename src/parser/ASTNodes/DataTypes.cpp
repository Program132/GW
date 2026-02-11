#include "DataTypes.h"

DataTypes toDataTypes(Token token) {
  std::string value = token.getValue();
  if (value == "Int") {
    return DataTypes::Int;
  } else if (value == "Number") {
    return DataTypes::Number;
  } else if (value == "Boolean") {
    return DataTypes::Boolean;
  } else if (value == "String") {
    return DataTypes::String;
  } else {
    return DataTypes::Null;
  }
}

std::string fromDataTypes(DataTypes dataType) {
  switch (dataType) {
  case DataTypes::Null:
    return "Null";
  case DataTypes::Int:
    return "Int";
  case DataTypes::Number:
    return "Number";
  case DataTypes::Boolean:
    return "Boolean";
  case DataTypes::String:
    return "String";
  default:
    return "Null";
  }
}