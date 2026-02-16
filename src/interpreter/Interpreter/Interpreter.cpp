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
  case IF: {
    IfStatement *stmt = (IfStatement *)statement;
    Value condition = evaluate(stmt->getCondition());
    if (condition.asBool()) {
      execute(stmt->getThenBranch());
    } else if (stmt->getElseBranch() != nullptr) {
      execute(stmt->getElseBranch());
    }
    break;
  }
  case WHILE: {
    WhileStatement *stmt = (WhileStatement *)statement;
    while (evaluate(stmt->getCondition()).asBool()) {
      execute(stmt->getBody());
    }
    break;
  }
  case FOR: {
    ForStatement *stmt = (ForStatement *)statement;
    Environment *previousEnv = environment;
    environment = new Environment(previousEnv);

    if (stmt->getInitializer() != nullptr) {
      execute(stmt->getInitializer());
    }

    while (stmt->getCondition() == nullptr ||
           evaluate(stmt->getCondition()).asBool()) {
      execute(stmt->getBody());
      if (stmt->getIncrement() != nullptr) {
        evaluate(stmt->getIncrement());
      }
    }

    Environment *loopEnv = environment;
    environment = previousEnv;
    delete loopEnv;
    break;
  }
  case STRUCT_DECLARATION: {
    StructDeclarationStatement *stmt = (StructDeclarationStatement *)statement;
    environment->addStruct(stmt->getName().getValue(), stmt);
    break;
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
    if (environment->variableExist(varName)) {
      return environment->getVariableValue(varName);
    }
    if (environment->structExist(varName)) {
      StructDeclarationStatement *stmt = environment->getStruct(varName);
      std::map<std::string, Value> members;
      for (int i = 0; i < stmt->getFields().size(); i++) {
        List<Token> field = stmt->getFields().get(i);
        std::string fieldName = field.get(1).getValue();
        members[fieldName] = Value();
      }
      return Value(varName, &members);
    }
    throw std::runtime_error("Undefined variable '" + varName + "'");
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
    } else if (op == "+") {
      return +right;
    }
    throw std::runtime_error("Unknown unary operator: " + op);
  }
  case BINARY: {
    Value left = evaluate(expression->getChildren().get(0));
    Value right = evaluate(expression->getChildren().get(1));
    std::string op = expression->getToken().getValue();

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
    } else if (op == "!=" || op == "~=") {
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
      return Value(left.asBool() && right.asBool());
    } else if (op == "|" || op == "||") {
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
    GetExpression *expr = (GetExpression *)expression;
    Value object = evaluate(expr->getObject());

    if (object.getType() != VAL_STRUCT) {
      throw std::runtime_error("Only instances have properties.");
    }

    std::string name = expr->getName().getValue();
    if (object.structMembers->find(name) != object.structMembers->end()) {
      return (*object.structMembers)[name];
    }

    throw std::runtime_error("Undefined property '" + name + "'.");
  }
  case SET: {
    SetExpression *expr = (SetExpression *)expression;
    Value object = evaluate(expr->getObject());

    if (object.getType() != VAL_STRUCT) {
      throw std::runtime_error("Only instances have properties.");
    }

    std::string name = expr->getName().getValue();
    Value value = evaluate(expr->getValue());

    if (object.structMembers->find(name) != object.structMembers->end()) {
      (*object.structMembers)[name] = value;
      return value;
    }

    throw std::runtime_error("Undefined property '" + name + "'.");
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