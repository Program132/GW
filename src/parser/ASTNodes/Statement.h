#ifndef STATEMENT_H
#define STATEMENT_H

#include "../../utils/List.h"
#include "DataTypes.h"
#include "Expression.h"

enum StatementType {
  EXPRESSION_STATEMENT,
  PRINT_STATEMENT,
  PRINTLN_STATEMENT,
  VAR_DECLARATION,
  BLOCK,
  IF,
  WHILE,
  FOR,
  FUNCTION_DECLARATION,
  RETURN,
  STRUCT_DECLARATION,
  BREAK,
  CONTINUE,
  CONSTRUCTOR_DECLARATION,
  CLASS_DECLARATION,
  OPERATOR_DECLARATION
};

class Statement {
protected:
  StatementType type;
  Token token;
  Expression *expression;

public:
  Statement();
  Statement(StatementType type);
  Statement(StatementType type, Token token);
  Statement(StatementType type, Expression *expression);
  Statement(StatementType type, Token token, Expression *expression);
  virtual ~Statement();

  StatementType getType() const;
  Token getToken() const;
  Expression *getExpression() const;

  friend std::ostream &operator<<(std::ostream &os, const Statement &statement);
};

class ExpressionStatement : public Statement {
public:
  ExpressionStatement(Expression *expression);
};

class PrintStatement : public Statement {
public:
  PrintStatement(Expression *expression);

  friend std::ostream &operator<<(std::ostream &os,
                                  const PrintStatement &printStatement);
};

class PrintlnStatement : public Statement {
public:
  PrintlnStatement(Expression *expression);

  friend std::ostream &operator<<(std::ostream &os,
                                  const PrintlnStatement &printlnStatement);
};

class VarDeclarationStatement : public Statement {
private:
  Token name;

public:
  VarDeclarationStatement(Token name);
  VarDeclarationStatement(Token name, Expression *initializer);

  Token getName() const;
  Expression *getInitializer() const;
};

class BlockStatement : public Statement {
private:
  List<Statement *> statements;

public:
  BlockStatement() = default;
  BlockStatement(List<Statement *> statements);

  List<Statement *> getStatements() const;
};

class IfStatement : public Statement {
private:
  Expression *condition;
  Statement *thenBranch;
  Statement *elseBranch;

public:
  IfStatement(Expression *condition, Statement *thenBranch);
  IfStatement(Expression *condition, Statement *thenBranch,
              Statement *elseBranch);

  Expression *getCondition() const;
  Statement *getThenBranch() const;
  Statement *getElseBranch() const;
};

class WhileStatement : public Statement {
private:
  Expression *condition;
  Statement *body;

public:
  WhileStatement(Expression *condition, Statement *body);

  Expression *getCondition() const;
  Statement *getBody() const;
};

class ForStatement : public Statement {
private:
  Statement *initializer;
  Expression *condition;
  Expression *increment;
  Statement *body;

public:
  ForStatement(Statement *initializer, Expression *condition,
               Expression *increment, Statement *body);
  Statement *getInitializer() const;
  Expression *getCondition() const;
  Expression *getIncrement() const;
  Statement *getBody() const;
};

class FunctionDeclarationStatement : public Statement {
private:
  Token name;
  List<List<Token>>
      params; // Will represent list of [DataType param type, param name]
  BlockStatement *body;
  DataTypes returnType;

public:
  FunctionDeclarationStatement(Token name, List<List<Token>> params,
                               BlockStatement *body, DataTypes returnType);
  FunctionDeclarationStatement(Token name, List<List<Token>> params,
                               BlockStatement *body);

  Token getName() const;
  List<List<Token>> getParams() const;
  BlockStatement *getBody() const;
  DataTypes getReturnType() const;
};

class ReturnStatement : public Statement {
public:
  ReturnStatement();
  ReturnStatement(Expression *value);

  Expression *getValue() const;
};

class StructDeclarationStatement : public Statement {
private:
  Token name;
  List<List<Token>>
      fields; // Will represent list of [DataType Token, Field Name]

public:
  StructDeclarationStatement(Token name, List<List<Token>> fields);

  Token getName() const;
  List<List<Token>> getFields() const;
};

class BreakStatement : public Statement {
public:
  BreakStatement();
};

class ContinueStatement : public Statement {
public:
  ContinueStatement();
};

class OperatorDeclarationStatement;

class ConstructorDeclarationStatement : public Statement {
private:
  List<List<Token>> params;
  BlockStatement *body;

public:
  ConstructorDeclarationStatement(List<List<Token>> params,
                                  BlockStatement *body);

  List<List<Token>> getParams() const;
  BlockStatement *getBody() const;
};

class ClassDeclarationStatement : public Statement {
private:
  Token name;
  Token superclass;
  List<List<Token>> fields;
  List<FunctionDeclarationStatement *> methods;
  List<ConstructorDeclarationStatement *> constructors;
  List<OperatorDeclarationStatement *> operators;

public:
  ClassDeclarationStatement(
      Token name, Token superclass, List<List<Token>> fields,
      List<FunctionDeclarationStatement *> methods,
      List<ConstructorDeclarationStatement *> constructors,
      List<OperatorDeclarationStatement *> operators);

  Token getName() const;
  Token getSuperclass() const;
  List<List<Token>> getFields() const;
  List<FunctionDeclarationStatement *> getMethods() const;
  List<ConstructorDeclarationStatement *> getConstructors() const;
  List<OperatorDeclarationStatement *> getOperators() const;
};

class OperatorDeclarationStatement : public Statement {
private:
  Token op;
  List<List<Token>>
      params; // Will represent list of [DataType param type, param name]
  BlockStatement *body;
  DataTypes returnType;

public:
  OperatorDeclarationStatement(Token op, List<List<Token>> params,
                               BlockStatement *body, DataTypes returnType);

  Token getOperator() const;
  List<List<Token>> getParams() const;
  BlockStatement *getBody() const;
  DataTypes getReturnType() const;
};

#endif // STATEMENT_H