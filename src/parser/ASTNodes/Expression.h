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
  List<Expression *> children;

public:
  Expression();
  Expression(ExpressionType type);
  Expression(ExpressionType type, Token token);
  virtual ~Expression();

  ExpressionType getType() const;
  Token getToken() const;
  List<Expression *> getChildren() const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const Expression &expression);
};

class EmptyExpression : public Expression {
public:
  EmptyExpression();
};

class BinaryExpression : public Expression {
public:
  BinaryExpression(Expression *left, Token op, Expression *right);

  Expression *getLeft() const;
  Token getOp() const;
  Expression *getRight() const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const BinaryExpression &binaryExpression);
};

class UnaryExpression : public Expression {
public:
  UnaryExpression(Token op, Expression *right);

  Token getOp() const;
  Expression *getRight() const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const UnaryExpression &unaryExpression);
};

class LiteralExpression : public Expression {
public:
  LiteralExpression(Token value);

  Token getValue() const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const LiteralExpression &literalExpression);
};

class GroupingExpression : public Expression {
public:
  GroupingExpression(Expression *expression);

  Expression *getExpression() const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const GroupingExpression &groupingExpression);
};

class VariableExpression : public Expression {
private:
  List<Token> typeArgs;

public:
  VariableExpression(Token name);
  VariableExpression(Token name, List<Token> typeArgs);

  Token getName() const;
  List<Token> getTypeArgs() const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const VariableExpression &variableExpression);
};

class AssignExpression : public Expression {
public:
  AssignExpression(Token name, Expression *value);

  Token getName() const;
  Expression *getValue() const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const AssignExpression &assignExpression);
};

class CallExpression : public Expression {
public:
  CallExpression(Expression *callee, Token paren, List<Expression *> arguments);

  Expression *getCallee() const;
  Token getParen() const;
  List<Expression *> getArguments() const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const CallExpression &callExpression);
};

class GetExpression : public Expression {
public:
  GetExpression(Expression *object, Token name);

  Expression *getObject() const;
  Token getName() const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const GetExpression &getExpression);
};

class SetExpression : public Expression {
public:
  SetExpression(Expression *object, Token name, Expression *value);

  Expression *getObject() const;
  Token getName() const;
  Expression *getValue() const;

  friend std::ostream &operator<<(std::ostream &os,
                                  const SetExpression &setExpression);
};

#endif // EXPRESSION_H