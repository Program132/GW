#include "Statement.h"

ExpressionStatement::ExpressionStatement(Expression expression)
    : expression(expression) {}

Expression ExpressionStatement::getExpression() const { return expression; }

PrintStatement::PrintStatement(Expression expression)
    : expression(expression) {}

Expression PrintStatement::getExpression() const { return expression; }

PrintlnStatement::PrintlnStatement(Expression expression)
    : expression(expression) {}

Expression PrintlnStatement::getExpression() const { return expression; }

VarDeclarationStatement::VarDeclarationStatement(Token name) : name(name) {}

VarDeclarationStatement::VarDeclarationStatement(Token name,
                                                 Expression initializer)
    : name(name), initializer(initializer) {}

Token VarDeclarationStatement::getName() const { return name; }

Expression VarDeclarationStatement::getInitializer() const {
  return initializer;
}

BlockStatement::BlockStatement(List<Statement> statements)
    : statements(statements) {}

List<Statement> BlockStatement::getStatements() const { return statements; }

IfStatement::IfStatement(Expression condition, Statement thenBranch)
    : condition(condition), thenBranch(thenBranch) {}

IfStatement::IfStatement(Expression condition, Statement thenBranch,
                         Statement elseBranch)
    : condition(condition), thenBranch(thenBranch), elseBranch(elseBranch) {}

Expression IfStatement::getCondition() const { return condition; }

Statement IfStatement::getThenBranch() const { return thenBranch; }

Statement IfStatement::getElseBranch() const { return elseBranch; }

WhileStatement::WhileStatement(Expression condition, Statement body)
    : condition(condition), body(body) {}

Expression WhileStatement::getCondition() const { return condition; }

Statement WhileStatement::getBody() const { return body; }

ForStatement::ForStatement(Expression initializer, Expression condition,
                           Expression increment, Statement body)
    : initializer(initializer), condition(condition), increment(increment),
      body(body) {}

Expression ForStatement::getInitializer() const { return initializer; }

Expression ForStatement::getCondition() const { return condition; }

Expression ForStatement::getIncrement() const { return increment; }

Statement ForStatement::getBody() const { return body; }

FunctionDeclarationStatement::FunctionDeclarationStatement(
    Token name, List<List<Token>> params, BlockStatement body,
    DataTypes returnType)
    : name(name), params(params), body(body), returnType(returnType) {}

FunctionDeclarationStatement::FunctionDeclarationStatement(
    Token name, List<List<Token>> params, BlockStatement body)
    : name(name), params(params), body(body) {}

Token FunctionDeclarationStatement::getName() const { return name; }

List<List<Token>> FunctionDeclarationStatement::getParams() const {
  return params;
}

BlockStatement FunctionDeclarationStatement::getBody() const { return body; }

DataTypes FunctionDeclarationStatement::getReturnType() const {
  return returnType;
}

ReturnStatement::ReturnStatement() : value() {}

ReturnStatement::ReturnStatement(Expression value) : value(value) {}

Expression ReturnStatement::getValue() const { return value; }

StructDeclarationStatement::StructDeclarationStatement(Token name,
                                                       List<List<Token>> fields)
    : name(name), fields(fields) {}

Token StructDeclarationStatement::getName() const { return name; }

List<List<Token>> StructDeclarationStatement::getFields() const {
  return fields;
}

BreakStatement::BreakStatement() {}

ContinueStatement::ContinueStatement() {}

ConstructorDeclarationStatement::ConstructorDeclarationStatement(
    List<Token> params, BlockStatement body)
    : params(params), body(body) {}

List<Token> ConstructorDeclarationStatement::getParams() const {
  return params;
}

BlockStatement ConstructorDeclarationStatement::getBody() const { return body; }

ClassDeclarationStatement::ClassDeclarationStatement(Token name,
                                                     List<List<Token>> fields)
    : name(name), fields(fields) {}

Token ClassDeclarationStatement::getName() const { return name; }

List<List<Token>> ClassDeclarationStatement::getFields() const {
  return fields;
}

OperatorDeclarationStatement::OperatorDeclarationStatement(
    Token op, List<List<Token>> params, BlockStatement body,
    DataTypes returnType)
    : op(op), params(params), body(body), returnType(returnType) {}

Token OperatorDeclarationStatement::getOperator() const { return op; }

List<List<Token>> OperatorDeclarationStatement::getParams() const {
  return params;
}

BlockStatement OperatorDeclarationStatement::getBody() const { return body; }

DataTypes OperatorDeclarationStatement::getReturnType() const {
  return returnType;
}