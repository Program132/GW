#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "../../parser/ASTNodes/Statement.h"
#include "../Interpreter/Function.h"
#include "../value/Value.h"
#include <map>
#include <string>

class Environment {
private:
  Environment *parent;
  std::map<std::string, Value> variables;
  std::map<std::string, std::string> variableTypes;
  std::map<std::string, Function> functions;
  std::map<std::string, StructDeclarationStatement *> structs;
  std::map<std::string, ClassDeclarationStatement *> classes;
  std::map<std::string, std::string> typeMappings; // Generic type mappings

public:
  Environment() : parent(nullptr) {}
  Environment(Environment *parent) : parent(parent) {}

  void addStruct(const std::string &name, StructDeclarationStatement *stmt) {
    structs[name] = stmt;
  }

  bool structExist(const std::string &name) {
    if (structs.find(name) != structs.end()) {
      return true;
    }
    if (parent != nullptr) {
      return parent->structExist(name);
    }
    return false;
  }

  StructDeclarationStatement *getStruct(const std::string &name) {
    if (structs.find(name) != structs.end()) {
      return structs[name];
    }
    if (parent != nullptr) {
      return parent->getStruct(name);
    }
    return nullptr;
  }

  void addClass(const std::string &name, ClassDeclarationStatement *stmt) {
    classes[name] = stmt;
  }

  bool classExist(const std::string &name) {
    if (classes.find(name) != classes.end()) {
      return true;
    }
    if (parent != nullptr) {
      return parent->classExist(name);
    }
    return false;
  }

  ClassDeclarationStatement *getClass(const std::string &name) {
    if (classes.find(name) != classes.end()) {
      return classes[name];
    }
    if (parent != nullptr) {
      return parent->getClass(name);
    }
    return nullptr;
  }

  void addVariable(const std::string &name, const Value &value,
                   const std::string &type = "Unknown") {
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

  void addFunction(const std::string &name, const Function &func) {
    functions[name] = func;
  }

  bool functionExist(const std::string &name) {
    if (functions.find(name) != functions.end()) {
      return true;
    }
    if (parent != nullptr) {
      return parent->functionExist(name);
    }
    return false;
  }

  Function getFunction(const std::string &name) {
    if (functions.find(name) != functions.end()) {
      return functions[name];
    }
    if (parent != nullptr) {
      return parent->getFunction(name);
    }
    return Function();
  }

  void addTypeMapping(const std::string &param, const std::string &actual) {
    typeMappings[param] = actual;
  }

  std::string resolveType(const std::string &type) {
    if (typeMappings.find(type) != typeMappings.end()) {
      return typeMappings[type];
    }
    if (parent != nullptr) {
      return parent->resolveType(type);
    }
    return type;
  }
};

#endif