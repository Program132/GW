#ifndef STATEMENT_H
#define STATEMENT_H

#include "../../utils/List.h"
#include "DataTypes.h"
#include "Expression.h"

class Statement {};

class ExpressionStatement : public Statement {
private:
  Expression expression;

public:
  ExpressionStatement(Expression expression);

  Expression getExpression() const;
};

class PrintStatement : public Statement {
private:
  Expression expression;

public:
  PrintStatement(Expression expression);

  Expression getExpression() const;
};

class VarDeclarationStatement : public Statement {
private:
  Token name;
  Expression initializer;

public:
  VarDeclarationStatement(Token name);
  VarDeclarationStatement(Token name, Expression initializer);

  Token getName() const;
  Expression getInitializer() const;
};

class BlockStatement : public Statement {
private:
  List<Statement> statements;

public:
  BlockStatement() = default;
  BlockStatement(List<Statement> statements);

  List<Statement> getStatements() const;
};

class IfStatement : public Statement {
private:
  Expression condition;
  Statement thenBranch;
  Statement elseBranch;

public:
  IfStatement(Expression condition, Statement thenBranch);
  IfStatement(Expression condition, Statement thenBranch, Statement elseBranch);

  Expression getCondition() const;
  Statement getThenBranch() const;
  Statement getElseBranch() const;
};

class WhileStatement : public Statement {
private:
  Expression condition;
  Statement body;

public:
  WhileStatement(Expression condition, Statement body);

  Expression getCondition() const;
  Statement getBody() const;
};

class ForStatement : public Statement {
private:
  Expression initializer;
  Expression condition;
  Expression increment;
  Statement body;

public:
  ForStatement(Expression initializer, Expression condition,
               Expression increment, Statement body);
  Expression getInitializer() const;
  Expression getCondition() const;
  Expression getIncrement() const;
  Statement getBody() const;
};

class FunctionDeclarationStatement : public Statement {
private:
  Token name;
  List<List<Token>>
      params; // Will represent list of [DataType param type, param name]
  BlockStatement body;
  DataTypes returnType;

public:
  FunctionDeclarationStatement(Token name, List<List<Token>> params,
                               BlockStatement body, DataTypes returnType);
  FunctionDeclarationStatement(Token name, List<List<Token>> params,
                               BlockStatement body);

  Token getName() const;
  List<List<Token>> getParams() const;
  BlockStatement getBody() const;
  DataTypes getReturnType() const;
};

class ReturnStatement : public Statement {
private:
  Expression value;

public:
  ReturnStatement();
  ReturnStatement(Expression value);

  Expression getValue() const;
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

class ConstructorDeclarationStatement : public Statement {
private:
  List<Token> params;
  BlockStatement body;

public:
  ConstructorDeclarationStatement(List<Token> params, BlockStatement body);

  List<Token> getParams() const;
  BlockStatement getBody() const;
};

class ClassDeclarationStatement : public Statement {
private:
  Token name;
  List<List<Token>> fields;
  List<FunctionDeclarationStatement> methods;
  List<ConstructorDeclarationStatement> constructors;

public:
  ClassDeclarationStatement(Token name, List<List<Token>> fields);

  Token getName() const;
  List<List<Token>> getFields() const;
};

class OperatorDeclarationStatement : public Statement {
private:
  Token op;
  List<List<Token>>
      params; // Will represent list of [DataType param type, param name]
  BlockStatement body;
  DataTypes returnType;

public:
  OperatorDeclarationStatement(Token op, List<List<Token>> params,
                               BlockStatement body, DataTypes returnType);

  Token getOperator() const;
  List<List<Token>> getParams() const;
  BlockStatement getBody() const;
  DataTypes getReturnType() const;
};

#endif // STATEMENT_H