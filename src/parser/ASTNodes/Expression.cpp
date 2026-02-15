#include "Expression.h"

Expression::Expression() : type(EMPTY_EXPR), token(Token()) {}
Expression::Expression(ExpressionType type) : type(type), token(Token()) {}
Expression::Expression(ExpressionType type, Token token)
    : type(type), token(token) {}

std::ostream &operator<<(std::ostream &os, const Expression &expression) {
  switch (expression.getType()) {
  case EMPTY_EXPR:
    os << "EMPTY_EXPR";
    break;
  case BINARY:
    os << "BINARY_EXPRESSION";
    break;
  case UNARY:
    os << "UNARY_EXPRESSION";
    break;
  case LITERAL:
    os << "LITERAL_EXPRESSION";
    break;
  case GROUPING:
    os << "GROUPING_EXPRESSION";
    break;
  case VARIABLE:
    os << "VARIABLE_EXPRESSION";
    break;
  case ASSIGN:
    os << "ASSIGN_EXPRESSION";
    break;
  case CALL:
    os << "CALL_EXPRESSION";
    break;
  case GET:
    os << "GET_EXPRESSION";
    break;
  case SET:
    os << "SET_EXPRESSION";
    break;
  }
  return os;
}

ExpressionType Expression::getType() const { return type; }
Token Expression::getToken() const { return token; }

EmptyExpression::EmptyExpression() : Expression(EMPTY_EXPR) {}

BinaryExpression::BinaryExpression(Expression left, Token op, Expression right)
    : Expression(BINARY, op), left(left), op(op), right(right) {}

Expression BinaryExpression::getLeft() const { return left; }
Token BinaryExpression::getOp() const { return op; }
Expression BinaryExpression::getRight() const { return right; }

std::ostream &operator<<(std::ostream &os,
                         const BinaryExpression &binaryExpression) {
  os << "BINARY_EXPRESSION{";
  os << binaryExpression.getLeft() << " ";
  os << binaryExpression.getOp().getValue() << " ";
  os << binaryExpression.getRight();
  os << "}";
  return os;
}

UnaryExpression::UnaryExpression(Token op, Expression right)
    : Expression(UNARY, op), op(op), right(right) {}

Token UnaryExpression::getOp() const { return op; }
Expression UnaryExpression::getRight() const { return right; }

std::ostream &operator<<(std::ostream &os,
                         const UnaryExpression &unaryExpression) {
  os << "UNARY_EXPRESSION{";
  os << unaryExpression.getOp().getValue() << " ";
  os << unaryExpression.getRight();
  os << "}";
  return os;
}

LiteralExpression::LiteralExpression(Token value)
    : Expression(LITERAL, value), value(value) {}

Token LiteralExpression::getValue() const { return value; }

std::ostream &operator<<(std::ostream &os,
                         const LiteralExpression &literalExpression) {
  os << "LITERAL{" << literalExpression.getValue().getValue() << "}";
  return os;
}

GroupingExpression::GroupingExpression(Expression expression)
    : Expression(GROUPING), expression(expression) {}

Expression GroupingExpression::getExpression() const { return expression; }

std::ostream &operator<<(std::ostream &os,
                         const GroupingExpression &groupingExpression) {
  os << "GROUPING{" << groupingExpression.getExpression() << "}";
  return os;
}

VariableExpression::VariableExpression(Token name)
    : Expression(VARIABLE, name), name(name) {}

Token VariableExpression::getName() const { return name; }

std::ostream &operator<<(std::ostream &os,
                         const VariableExpression &variableExpression) {
  os << "VARIABLE{" << variableExpression.getName().getValue() << "}";
  return os;
}

AssignExpression::AssignExpression(Token name, Expression value)
    : Expression(ASSIGN, name), name(name), value(value) {}

Token AssignExpression::getName() const { return name; }
Expression AssignExpression::getValue() const { return value; }

std::ostream &operator<<(std::ostream &os,
                         const AssignExpression &assignExpression) {
  os << "ASSIGN{" << assignExpression.getName().getValue() << " = ";
  os << assignExpression.getValue() << "}";
  return os;
}

CallExpression::CallExpression(Expression callee, Token paren,
                               List<Expression> arguments)
    : Expression(CALL, paren), callee(callee), paren(paren),
      arguments(arguments) {}

Expression CallExpression::getCallee() const { return callee; }
Token CallExpression::getParen() const { return paren; }
List<Expression> CallExpression::getArguments() const { return arguments; }

std::ostream &operator<<(std::ostream &os,
                         const CallExpression &callExpression) {
  os << "CALL{" << callExpression.getCallee() << "(";
  for (int i = 0; i < callExpression.getArguments().size(); i++) {
    if (i > 0)
      os << ", ";
    os << callExpression.getArguments().get(i);
  }
  os << ")}";
  return os;
}

GetExpression::GetExpression(Expression object, Token name)
    : Expression(GET, name), object(object), name(name) {}

Expression GetExpression::getObject() const { return object; }
Token GetExpression::getName() const { return name; }

std::ostream &operator<<(std::ostream &os, const GetExpression &getExpression) {
  os << "GET{" << getExpression.getObject() << ".";
  os << getExpression.getName().getValue() << "}";
  return os;
}

SetExpression::SetExpression(Expression object, Token name, Expression value)
    : Expression(SET, name), object(object), name(name), value(value) {}

Expression SetExpression::getObject() const { return object; }
Token SetExpression::getName() const { return name; }
Expression SetExpression::getValue() const { return value; }

std::ostream &operator<<(std::ostream &os, const SetExpression &setExpression) {
  os << "SET{" << setExpression.getObject() << ".";
  os << setExpression.getName().getValue() << " = ";
  os << setExpression.getValue() << "}";
  return os;
}