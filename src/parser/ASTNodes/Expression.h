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
  virtual ~Expression() = default;

  ExpressionType getType() const;
  Token getToken() const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const Expression &expression);
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

  friend std::ostream &operator<<(std::ostream &os,
                                  const BinaryExpression &binaryExpression);
};

class UnaryExpression : public Expression {
private:
  Token op;
  Expression right;

public:
  UnaryExpression(Token op, Expression right);

  Token getOp() const;
  Expression getRight() const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const UnaryExpression &unaryExpression);
};

class LiteralExpression : public Expression {
private:
  Token value;

public:
  LiteralExpression(Token value);

  Token getValue() const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const LiteralExpression &literalExpression);
};

class GroupingExpression : public Expression {
private:
  Expression expression;

public:
  GroupingExpression(Expression expression);

  Expression getExpression() const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const GroupingExpression &groupingExpression);
};

class VariableExpression : public Expression {
private:
  Token name;

public:
  VariableExpression(Token name);

  Token getName() const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const VariableExpression &variableExpression);
};

class AssignExpression : public Expression {
private:
  Token name;
  Expression value;

public:
  AssignExpression(Token name, Expression value);

  Token getName() const;
  Expression getValue() const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const AssignExpression &assignExpression);
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

  friend std::ostream &operator<<(std::ostream &os,
                                  const CallExpression &callExpression);
};

class GetExpression : public Expression {
private:
  Expression object;
  Token name;

public:
  GetExpression(Expression object, Token name);

  Expression getObject() const;
  Token getName() const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const GetExpression &getExpression);
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

  friend std::ostream &operator<<(std::ostream &os,
                                  const SetExpression &setExpression);
};

#endif // EXPRESSION_H