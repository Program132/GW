#include "Interpreter.h"
#include <iostream>

Interpreter::Interpreter() {}

Interpreter::Interpreter(List<Statement> statements) : statements(statements) {}

List<Statement> Interpreter::getStatements() const { return statements; }

Environment Interpreter::getEnvironment() const { return environment; }

void Interpreter::interpret() {
  for (int i = 0; i < statements.size(); i++) {
    execute(statements.get(i));
  }
}

void Interpreter::execute(Statement statement) {
  switch (statement.getType()) {
  case PRINT_STATEMENT: {
    if (statement.getExpression().getType() == EMPTY_EXPR) {
      return;
    }

    Value evaluation = evaluate(statement.getExpression());
    ExpressionType exprType = statement.getExpression().getType();

    if (exprType == ASSIGN || exprType == SET) {
      std::cerr
          << "Only literals, math (binary and unary), groups, variables and "
             "function calls are allowed"
          << std::endl;
      return;
    }

    std::cout << evaluation.toString();
    break;
  }
  case PRINTLN_STATEMENT: {
    if (statement.getExpression().getType() == EMPTY_EXPR) {
      std::cout << std::endl;
      return;
    }

    Value evaluation = evaluate(statement.getExpression());
    ExpressionType exprType = statement.getExpression().getType();

    if (exprType == ASSIGN || exprType == SET) {
      std::cerr
          << "Only literals, math (binary and unary), groups, variables and "
             "function calls are allowed"
          << std::endl;
      return;
    }

    std::cout << evaluation.toString() << std::endl;
    break;
  }
  default:
    break;
  }
}

Value Interpreter::evaluate(Expression expression) {
  switch (expression.getType()) {
  case LITERAL: {
    return tokenToValue(expression.getToken());
  }
  case VARIABLE: {
    std::string varName = expression.getToken().getValue();
    return Value(std::string("[Variable: ") + varName + "]");
  }
  case GROUPING: {
    GroupingExpression &grouping = dynamic_cast<GroupingExpression &>(
        const_cast<Expression &>(expression));
    return evaluate(grouping.getExpression());
  }
  case UNARY: {
    UnaryExpression &unary =
        dynamic_cast<UnaryExpression &>(const_cast<Expression &>(expression));
    Value right = evaluate(unary.getRight());
    std::string op = unary.getOp().getValue();

    if (op == "-") {
      return -right;
    } else if (op == "+") {
      return +right;
    } else if (op == "!") {
      if (!right.isBool()) {
        throw std::runtime_error(
            "Logical NOT operator requires boolean operand");
      }
      return Value(!right.asBool());
    }
    return right;
  }
  case BINARY: {
    BinaryExpression &binary =
        dynamic_cast<BinaryExpression &>(const_cast<Expression &>(expression));
    Value left = evaluate(binary.getLeft());
    Value right = evaluate(binary.getRight());
    std::string op = binary.getOp().getValue();

    if (op == "+") {
      return left + right;
    } else if (op == "-") {
      return left - right;
    } else if (op == "*") {
      return left * right;
    } else if (op == "/") {
      return left / right;
    } else if (op == "%") {
      return left % right;
    } else if (op == "^") {
      return left.pow(right);
    } else if (op == "==") {
      return Value(left == right);
    } else if (op == "~=" || op == "!=") {
      return Value(left != right);
    } else if (op == "<") {
      return Value(left < right);
    } else if (op == ">") {
      return Value(left > right);
    } else if (op == "<=") {
      return Value(left <= right);
    } else if (op == ">=") {
      return Value(left >= right);
    } else if (op == "&" || op == "&&") {
      if (!left.isBool() || !right.isBool()) {
        throw std::runtime_error(
            "Logical AND operator requires boolean operands");
      }
      return Value(left.asBool() && right.asBool());
    } else if (op == "|" || op == "||") {
      if (!left.isBool() || !right.isBool()) {
        throw std::runtime_error(
            "Logical OR operator requires boolean operands");
      }
      return Value(left.asBool() || right.asBool());
    }

    throw std::runtime_error("Unknown binary operator: " + op);
  }
  case CALL: {
    CallExpression &call =
        dynamic_cast<CallExpression &>(const_cast<Expression &>(expression));
    return Value("[Function Call]");
  }
  case GET: {
    GetExpression &getExpr =
        dynamic_cast<GetExpression &>(const_cast<Expression &>(expression));
    return Value("[Get Property]");
  }
  case EMPTY_EXPR:
    return Value();
  default:
    return Value("[Complex Expression]");
  }
}

Value Interpreter::tokenToValue(const Token &token) {
  TokenType tokenType = token.getType();
  std::string value = token.getValue();

  switch (tokenType) {
  case TokenType::INT: {
    try {
      return Value(std::stoi(value));
    } catch (...) {
      throw std::runtime_error("Invalid integer: " + value);
    }
  }
  case TokenType::NUMBER: {
    try {
      return Value(std::stod(value));
    } catch (...) {
      throw std::runtime_error("Invalid number: " + value);
    }
  }
  case TokenType::STRING:
    return Value(value);
  case TokenType::BOOLEAN:
    return Value(value == "true");
  default:
    return Value();
  }
}