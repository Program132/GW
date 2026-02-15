#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "../value/Value.h"
#include <map>
#include <string>

class Environment {
private:
  Environment *parent;
  std::map<std::string, Value> variables;
  std::map<std::string, std::string> variableTypes;

public:
  Environment() : parent(nullptr) {}
  Environment(Environment *parent) : parent(parent) {}

  void addVariable(const std::string &name, const Value &value,
                   const std::string &type) {
    variables[name] = value;
    variableTypes[name] = type;
  }

  bool variableExist(const std::string &name) {
    if (variables.find(name) != variables.end()) {
      return true;
    }
    if (parent != nullptr) {
      return parent->variableExist(name);
    }
    return false;
  }

  Value getVariableValue(const std::string &name) {
    if (variables.find(name) != variables.end()) {
      return variables[name];
    }
    if (parent != nullptr) {
      return parent->getVariableValue(name);
    }
    return Value(); // Returns Null value
  }

  std::string getVariableType(const std::string &name) {
    if (variableTypes.find(name) != variableTypes.end()) {
      return variableTypes[name];
    }
    if (parent != nullptr) {
      return parent->getVariableType(name);
    }
    return "Unknown";
  }

  void updateVariable(const std::string &name, const Value &value,
                      const std::string &type = "") {
    if (variables.find(name) != variables.end()) {
      variables[name] = value;
      if (!type.empty()) {
        variableTypes[name] = type;
      }
      return;
    }
    if (parent != nullptr) {
      parent->updateVariable(name, value, type);
    }
  }
};

#endif