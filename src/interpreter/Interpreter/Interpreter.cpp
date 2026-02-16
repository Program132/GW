#include "Interpreter.h"
#include "ReturnException.h"
#include <iostream>

Interpreter::Interpreter() { environment = new Environment(); }

Interpreter::Interpreter(List<Statement *> statements)
    : statements(statements) {
  environment = new Environment();
}

Interpreter::~Interpreter() {
  delete environment;
  for (int i = 0; i < statements.size(); i++) {
    delete statements.get(i);
  }
}

List<Statement *> Interpreter::getStatements() const { return statements; }

Environment *Interpreter::getEnvironment() const { return environment; }

void Interpreter::interpret() {
  for (int i = 0; i < statements.size(); i++) {
    execute(statements.get(i));
  }
}

void Interpreter::execute(Statement *statement) {
  switch (statement->getType()) {
  case PRINT_STATEMENT: {
    if (statement->getExpression()->getType() == EMPTY_EXPR) {
      return;
    }

    Value evaluation = evaluate(statement->getExpression());
    ExpressionType exprType = statement->getExpression()->getType();

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
    if (statement->getExpression()->getType() == EMPTY_EXPR) {
      std::cout << std::endl;
      return;
    }

    Value evaluation = evaluate(statement->getExpression());
    ExpressionType exprType = statement->getExpression()->getType();

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
  case VAR_DECLARATION: {
    std::string varName = statement->getToken().getValue();
    Value value; // Null

    if (statement->getExpression() != nullptr &&
        statement->getExpression()->getType() != EMPTY_EXPR) {
      value = evaluate(statement->getExpression());
    }

    if (environment->variableExist(varName)) {
      std::cerr << "[ERROR] Variable '" << varName << "' already defined"
                << std::endl;
      return;
    }

    environment->addVariable(varName, value, "Unknown");
    break;
  }
  case BLOCK: {
    BlockStatement *stmt = (BlockStatement *)statement;
    for (int i = 0; i < stmt->getStatements().size(); i++) {
      execute(stmt->getStatements().get(i));
    }
    break;
  }
  case FUNCTION_DECLARATION: {
    FunctionDeclarationStatement *stmt =
        (FunctionDeclarationStatement *)statement;
    std::string funcName = stmt->getToken().getValue();
    Function func(funcName, stmt->getParams(), BlockStatement(stmt->getBody()),
                  stmt->getReturnType());
    environment->addFunction(funcName, func);
    break;
  }
  case RETURN: {
    Value value;
    if (statement->getExpression() != nullptr &&
        statement->getExpression()->getType() != EMPTY_EXPR) {
      value = evaluate(statement->getExpression());
    }
    throw ReturnException(value);
  }
  case EXPRESSION_STATEMENT: {
    evaluate(statement->getExpression());
    break;
  }
  default:
    break;
  }
}

Value Interpreter::evaluate(Expression *expression) {
  switch (expression->getType()) {
  case LITERAL: {
    return tokenToValue(expression->getToken());
  }
  case VARIABLE: {
    std::string varName = expression->getToken().getValue();
    if (!environment->variableExist(varName)) {
      throw std::runtime_error("Undefined variable '" + varName + "'");
    }
    return environment->getVariableValue(varName);
  }
  case ASSIGN: {
    std::string varName = expression->getToken().getValue();
    Value value = evaluate(expression->getChildren().get(0));

    if (!environment->variableExist(varName)) {
      throw std::runtime_error("Undefined variable '" + varName + "'");
    }

    environment->updateVariable(varName, value);
    return value;
  }
  case GROUPING: {
    return evaluate(expression->getChildren().get(0));
  }
  case UNARY: {
    Value right = evaluate(expression->getChildren().get(0));
    std::string op = expression->getToken().getValue();

    if (op == "-") {
      if (right.isNumber())
        return Value(-right.asDouble());
      else
        throw std::runtime_error("Unary minus requires number operand");
    } else if (op == "!") {
      return Value(!right.asBool());
    }
    throw std::runtime_error("Unknown unary operator: " + op);
  }
  case BINARY: {
    Value left = evaluate(expression->getChildren().get(0));
    Value right = evaluate(expression->getChildren().get(1));
    std::string op = expression->getToken().getValue();

    if (op == "+") {
      if (left.isNumber() && right.isNumber())
        return Value(left.asDouble() + right.asDouble());
      else if (left.isString() && right.isString())
        return Value(left.asString() + right.asString());
      else if (left.isString())
        return Value(left.asString() + right.toString());
      else if (right.isString())
        return Value(left.toString() + right.asString());
      else
        throw std::runtime_error("Invalid operands for '+' operator");
    } else if (op == "-") {
      return Value(left.asDouble() - right.asDouble());
    } else if (op == "*") {
      return Value(left.asDouble() * right.asDouble());
    } else if (op == "/") {
      if (right.asDouble() == 0)
        throw std::runtime_error("Division by zero");
      return Value(left.asDouble() / right.asDouble());
    } else if (op == "==") {
      return Value(left == right);
    } else if (op == "!=") {
      return Value(!(left == right));
    } else if (op == "<") {
      return Value(left.asDouble() < right.asDouble());
    } else if (op == ">") {
      return Value(left.asDouble() > right.asDouble());
    } else if (op == "<=") {
      return Value(left.asDouble() <= right.asDouble());
    } else if (op == ">=") {
      return Value(left.asDouble() >= right.asDouble());
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
    Expression *calleeExpr = expression->getChildren().get(0);
    if (calleeExpr->getType() != VARIABLE) {
      throw std::runtime_error("Only direct function calls are supported");
    }

    std::string funcName = calleeExpr->getToken().getValue();
    if (!environment->functionExist(funcName)) {
      throw std::runtime_error("Function '" + funcName + "' not defined");
    }

    Function func = environment->getFunction(funcName);
    List<Expression *> argExprs =
        ((CallExpression *)expression)->getArguments();

    if (argExprs.size() != func.params.size()) {
      throw std::runtime_error("Function '" + funcName + "' expects " +
                               std::to_string(func.params.size()) +
                               " arguments but got " +
                               std::to_string(argExprs.size()));
    }

    List<Value> evaluatedArgs;
    for (int i = 0; i < argExprs.size(); i++) {
      evaluatedArgs.append(evaluate(argExprs.get(i)));
    }

    Environment *funcEnv = new Environment(environment);

    for (int i = 0; i < evaluatedArgs.size(); i++) {
      Token paramNameToken = func.params.get(i).get(1);
      Token paramTypeToken = func.params.get(i).get(0);
      funcEnv->addVariable(paramNameToken.getValue(), evaluatedArgs.get(i),
                           paramTypeToken.getValue());
    }

    Environment *previousEnv = environment;
    environment = funcEnv;

    Value returnValue;
    try {
      for (int i = 0; i < func.body.getStatements().size(); i++) {
        execute(func.body.getStatements().get(i));
      }
    } catch (const ReturnException &e) {
      returnValue = e.value;
    }

    environment = previousEnv;
    delete funcEnv;

    return returnValue;
  }
  case GET: {
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