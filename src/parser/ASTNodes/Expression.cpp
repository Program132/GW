#include "Expression.h"

Expression::Expression() : type(EMPTY_EXPR), token(Token()) {}
Expression::Expression(ExpressionType type) : type(type), token(Token()) {}
Expression::Expression(ExpressionType type, Token token)
    : type(type), token(token) {}

Expression::~Expression() {
  for (int i = 0; i < children.size(); i++) {
    delete children.get(i);
  }
}

std::ostream &operator<<(std::ostream &os, const Expression &expression) {
  switch (expression.getType()) {
  case EMPTY_EXPR:
    os << "EMPTY_EXPR";
    break;
  case BINARY:
    os << "BINARY{" << *expression.getChildren().get(0) << " "
       << expression.getToken().getValue() << " "
       << *expression.getChildren().get(1) << "}";
    break;
  case UNARY:
    os << "UNARY{" << expression.getToken().getValue() << " "
       << *expression.getChildren().get(0) << "}";
    break;
  case LITERAL:
    os << "LITERAL{" << expression.getToken().getValue() << "}";
    break;
  case GROUPING:
    os << "GROUPING{" << *expression.getChildren().get(0) << "}";
    break;
  case VARIABLE:
    os << "VARIABLE{" << expression.getToken().getValue() << "}";
    break;
  case ASSIGN:
    os << "ASSIGN{" << expression.getToken().getValue() << " = "
       << *expression.getChildren().get(0) << "}";
    break;
  case CALL:
    os << "CALL{" << *expression.getChildren().get(0) << "(";
    for (int i = 1; i < expression.getChildren().size(); i++) {
      if (i > 1)
        os << ", ";
      os << *expression.getChildren().get(i);
    }
    os << ")}";
    break;
  case GET:
    os << "GET{" << *expression.getChildren().get(0) << "."
       << expression.getToken().getValue() << "}";
    break;
  case SET:
    os << "SET{" << *expression.getChildren().get(0) << "."
       << expression.getToken().getValue() << " = "
       << *expression.getChildren().get(1) << "}";
    break;
  }
  return os;
}

ExpressionType Expression::getType() const { return type; }
Token Expression::getToken() const { return token; }
List<Expression *> Expression::getChildren() const { return children; }

EmptyExpression::EmptyExpression() : Expression(EMPTY_EXPR) {}

// BINARY: children[0] = left, children[1] = right, token = op
BinaryExpression::BinaryExpression(Expression *left, Token op,
                                   Expression *right)
    : Expression(BINARY, op) {
  children.append(left);
  children.append(right);
}
Expression *BinaryExpression::getLeft() const { return children.get(0); }
Token BinaryExpression::getOp() const { return token; }
Expression *BinaryExpression::getRight() const { return children.get(1); }

std::ostream &operator<<(std::ostream &os,
                         const BinaryExpression &binaryExpression) {
  os << "BINARY{" << *binaryExpression.getLeft() << " "
     << binaryExpression.getOp().getValue() << " "
     << *binaryExpression.getRight() << "}";
  return os;
}

// UNARY: children[0] = right, token = op
UnaryExpression::UnaryExpression(Token op, Expression *right)
    : Expression(UNARY, op) {
  children.append(right);
}
Token UnaryExpression::getOp() const { return token; }
Expression *UnaryExpression::getRight() const { return children.get(0); }

std::ostream &operator<<(std::ostream &os,
                         const UnaryExpression &unaryExpression) {
  os << "UNARY{" << unaryExpression.getOp().getValue() << " "
     << *unaryExpression.getRight() << "}";
  return os;
}

// LITERAL: token = value
LiteralExpression::LiteralExpression(Token value)
    : Expression(LITERAL, value) {}
Token LiteralExpression::getValue() const { return token; }

std::ostream &operator<<(std::ostream &os,
                         const LiteralExpression &literalExpression) {
  os << "LITERAL{" << literalExpression.getValue().getValue() << "}";
  return os;
}

// GROUPING: children[0] = expression
GroupingExpression::GroupingExpression(Expression *expression)
    : Expression(GROUPING) {
  children.append(expression);
}
Expression *GroupingExpression::getExpression() const {
  return children.get(0);
}

std::ostream &operator<<(std::ostream &os,
                         const GroupingExpression &groupingExpression) {
  os << "GROUPING{" << *groupingExpression.getExpression() << "}";
  return os;
}

// VARIABLE: token = name
VariableExpression::VariableExpression(Token name)
    : Expression(VARIABLE, name) {}
Token VariableExpression::getName() const { return token; }

std::ostream &operator<<(std::ostream &os,
                         const VariableExpression &variableExpression) {
  os << "VARIABLE{" << variableExpression.getName().getValue() << "}";
  return os;
}

// ASSIGN: token = name, children[0] = value
AssignExpression::AssignExpression(Token name, Expression *value)
    : Expression(ASSIGN, name) {
  children.append(value);
}
Token AssignExpression::getName() const { return token; }
Expression *AssignExpression::getValue() const { return children.get(0); }

std::ostream &operator<<(std::ostream &os,
                         const AssignExpression &assignExpression) {
  os << "ASSIGN{" << assignExpression.getName().getValue() << " = "
     << *assignExpression.getValue() << "}";
  return os;
}

// CALL: children[0] = callee, children[1..n] = arguments, token = paren
CallExpression::CallExpression(Expression *callee, Token paren,
                               List<Expression *> arguments)
    : Expression(CALL, paren) {
  children.append(callee);
  for (int i = 0; i < arguments.size(); i++) {
    children.append(arguments.get(i));
  }
}
Expression *CallExpression::getCallee() const { return children.get(0); }
Token CallExpression::getParen() const { return token; }
List<Expression *> CallExpression::getArguments() const {
  List<Expression *> args;
  for (int i = 1; i < children.size(); i++) {
    args.append(children.get(i));
  }
  return args;
}

std::ostream &operator<<(std::ostream &os,
                         const CallExpression &callExpression) {
  os << "CALL{" << *callExpression.getCallee() << "(";
  List<Expression *> args = callExpression.getArguments();
  for (int i = 0; i < args.size(); i++) {
    if (i > 0)
      os << ", ";
    os << *args.get(i);
  }
  os << ")}";
  return os;
}

// GET: children[0] = object, token = name
GetExpression::GetExpression(Expression *object, Token name)
    : Expression(GET, name) {
  children.append(object);
}
Expression *GetExpression::getObject() const { return children.get(0); }
Token GetExpression::getName() const { return token; }

std::ostream &operator<<(std::ostream &os, const GetExpression &getExpression) {
  os << "GET{" << *getExpression.getObject() << "."
     << getExpression.getName().getValue() << "}";
  return os;
}

// SET: children[0] = object, children[1] = value, token = name
SetExpression::SetExpression(Expression *object, Token name, Expression *value)
    : Expression(SET, name) {
  children.append(object);
  children.append(value);
}
Expression *SetExpression::getObject() const { return children.get(0); }
Token SetExpression::getName() const { return token; }
Expression *SetExpression::getValue() const { return children.get(1); }

std::ostream &operator<<(std::ostream &os, const SetExpression &setExpression) {
  os << "SET{" << *setExpression.getObject() << "."
     << setExpression.getName().getValue() << " = " << *setExpression.getValue()
     << "}";
  return os;
}