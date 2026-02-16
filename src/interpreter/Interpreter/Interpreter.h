#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../../parser/ASTNodes/Statement.h"
#include "../environment/Environment.h"
#include "../value/Value.h"

class Interpreter {
private:
  List<Statement *> statements;
  Environment *environment;

  Value tokenToValue(const Token &token);

public:
  Interpreter();
  Interpreter(List<Statement *> statements);
  virtual ~Interpreter();

  List<Statement *> getStatements() const;
  Environment *getEnvironment() const;

  void interpret();
  void execute(Statement *statement);
  Value evaluate(Expression *expression);
};

#endif