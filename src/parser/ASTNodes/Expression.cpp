#include "Expression.h"

Expression::Expression() { this->type = EMPTY_EXPR; }
Expression::Expression(ExpressionType type) { this->type = type; }

ExpressionType Expression::getType() const { return type; }

EmptyExpression::EmptyExpression() : Expression(EMPTY_EXPR) {}

BinaryExpression::BinaryExpression(Expression left, Token op, Expression right)
    : Expression(BINARY), left(left), op(op), right(right) {}

Expression BinaryExpression::getLeft() const { return left; }
Token BinaryExpression::getOp() const { return op; }
Expression BinaryExpression::getRight() const { return right; }

UnaryExpression::UnaryExpression(Token op, Expression right)
    : Expression(UNARY), op(op), right(right) {}

Token UnaryExpression::getOp() const { return op; }
Expression UnaryExpression::getRight() const { return right; }

LiteralExpression::LiteralExpression(Token value)
    : Expression(LITERAL), value(value) {}

Token LiteralExpression::getValue() const { return value; }

GroupingExpression::GroupingExpression(Expression expression)
    : Expression(GROUPING), expression(expression) {}

Expression GroupingExpression::getExpression() const { return expression; }

VariableExpression::VariableExpression(Token name)
    : Expression(VARIABLE), name(name) {}

Token VariableExpression::getName() const { return name; }

AssignExpression::AssignExpression(Token name, Expression value)
    : Expression(ASSIGN), name(name), value(value) {}

Token AssignExpression::getName() const { return name; }
Expression AssignExpression::getValue() const { return value; }

CallExpression::CallExpression(Expression callee, Token paren,
                               List<Expression> arguments)
    : Expression(CALL), callee(callee), paren(paren), arguments(arguments) {}

Expression CallExpression::getCallee() const { return callee; }
Token CallExpression::getParen() const { return paren; }
List<Expression> CallExpression::getArguments() const { return arguments; }

GetExpression::GetExpression(Expression object, Token name)
    : Expression(GET), object(object), name(name) {}

Expression GetExpression::getObject() const { return object; }
Token GetExpression::getName() const { return name; }

SetExpression::SetExpression(Expression object, Token name, Expression value)
    : Expression(SET), object(object), name(name), value(value) {}

Expression SetExpression::getObject() const { return object; }
Token SetExpression::getName() const { return name; }
Expression SetExpression::getValue() const { return value; }