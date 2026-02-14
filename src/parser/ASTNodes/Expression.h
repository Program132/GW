#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "../../lexer/Token/Token.h"
#include "../../utils/List.h"

enum ExpressionType {
  EMPTY_EXPR,
  BINARY,
  UNARY,
  LITERAL,
  GROUPING,
  VARIABLE,
  ASSIGN,
  CALL,
  GET,
  SET
};

class Expression {
protected:
  ExpressionType type;
  Token token;

public:
  Expression();
  Expression(ExpressionType type);
  Expression(ExpressionType type, Token token);

  ExpressionType getType() const;
  Token getToken() const;
};

class EmptyExpression : public Expression {
public:
  EmptyExpression();
};

class BinaryExpression : public Expression {
private:
  Expression left;
  Token op;
  Expression right;

public:
  BinaryExpression(Expression left, Token op, Expression right);

  Expression getLeft() const;
  Token getOp() const;
  Expression getRight() const;
};

class UnaryExpression : public Expression {
private:
  Token op;
  Expression right;

public:
  UnaryExpression(Token op, Expression right);

  Token getOp() const;
  Expression getRight() const;
};

class LiteralExpression : public Expression {
private:
  Token value;

public:
  LiteralExpression(Token value);

  Token getValue() const;
};

class GroupingExpression : public Expression {
private:
  Expression expression;

public:
  GroupingExpression(Expression expression);

  Expression getExpression() const;
};

class VariableExpression : public Expression {
private:
  Token name;

public:
  VariableExpression(Token name);

  Token getName() const;
};

class AssignExpression : public Expression {
private:
  Token name;
  Expression value;

public:
  AssignExpression(Token name, Expression value);

  Token getName() const;
  Expression getValue() const;
};

class CallExpression : public Expression {
private:
  Expression callee;
  Token paren;
  List<Expression> arguments;

public:
  CallExpression(Expression callee, Token paren, List<Expression> arguments);

  Expression getCallee() const;
  Token getParen() const;
  List<Expression> getArguments() const;
};

class GetExpression : public Expression {
private:
  Expression object;
  Token name;

public:
  GetExpression(Expression object, Token name);

  Expression getObject() const;
  Token getName() const;
};

class SetExpression : public Expression {
private:
  Expression object;
  Token name;
  Expression value;

public:
  SetExpression(Expression object, Token name, Expression value);

  Expression getObject() const;
  Token getName() const;
  Expression getValue() const;
};

#endif // EXPRESSION_H