#ifndef FUNCTION_H
#define FUNCTION_H

#include "../../parser/ASTNodes/Statement.h"
#include <string>

class Function {
public:
  std::string name;
  List<List<Token>> params;
  BlockStatement body;
  DataTypes returnType;

  Function() : name(""), body(BlockStatement()), returnType(Null) {}
  Function(std::string name, List<List<Token>> params, BlockStatement body,
           DataTypes returnType)
      : name(name), params(params), body(body), returnType(returnType) {}
};

#endif
