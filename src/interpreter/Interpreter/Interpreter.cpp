#include "Interpreter.h"
#include "FlowControlExceptions.h"
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
    try {
      while (evaluate(stmt->getCondition()).asBool()) {
        try {
          execute(stmt->getBody());
        } catch (const ContinueException &) {
          // Continue loop
        }
      }
    } catch (const BreakException &) {
      // Break loop
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

    try {
      while (stmt->getCondition() == nullptr ||
             evaluate(stmt->getCondition()).asBool()) {
        try {
          execute(stmt->getBody());
        } catch (const ContinueException &) {
          // Continue loop
        }
        if (stmt->getIncrement() != nullptr) {
          evaluate(stmt->getIncrement());
        }
      }
    } catch (const BreakException &) {
      // Break loop
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
  case CLASS_DECLARATION: {
    ClassDeclarationStatement *stmt = (ClassDeclarationStatement *)statement;
    environment->addClass(stmt->getName().getValue(), stmt);
    break;
  }
  case BREAK:
    throw BreakException();
  case CONTINUE:
    throw ContinueException();
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
    if (environment->classExist(varName)) {
      // Return a Value representing the Class itself
      return Value(VAL_CLASS, varName);
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

    if (right.getType() == VAL_STRUCT &&
        environment->classExist(right.structName)) {
      ClassDeclarationStatement *cls = environment->getClass(right.structName);
      for (int i = 0; i < cls->getOperators().size(); i++) {
        OperatorDeclarationStatement *opDecl = cls->getOperators().get(i);
        if (opDecl->getOperator().getValue() == op &&
            opDecl->getParams().size() == 0) {
          Environment *opEnv = new Environment(environment);
          opEnv->addVariable("this", right, right.structName);

          Environment *previousEnv = environment;
          environment = opEnv;

          Value returnValue;
          try {
            for (int j = 0; j < opDecl->getBody()->getStatements().size();
                 j++) {
              execute(opDecl->getBody()->getStatements().get(j));
            }
          } catch (const ReturnException &e) {
            returnValue = e.value;
          }

          environment = previousEnv;
          delete opEnv;

          return returnValue;
        }
      }
    }

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

    if (left.getType() == VAL_STRUCT &&
        environment->classExist(left.structName)) {
      ClassDeclarationStatement *cls = environment->getClass(left.structName);
      for (int i = 0; i < cls->getOperators().size(); i++) {
        OperatorDeclarationStatement *opDecl = cls->getOperators().get(i);
        if (opDecl->getOperator().getValue() == op) {
          if (opDecl->getParams().size() == 1) {
            std::string paramType =
                opDecl->getParams().get(0).get(0).getValue();
            bool match = false;

            if (paramType == "Int" || paramType == "Integer") {
              if (right.isInt())
                match = true;
            } else if (paramType == "Number") {
              if (right.isNumber())
                match = true;
            } else if (paramType == "String") {
              if (right.isString())
                match = true;
            } else if (paramType == "Boolean") {
              if (right.isBool())
                match = true;
            } else if (right.getType() == VAL_STRUCT &&
                       right.structName == paramType) {
              match = true;
            }

            if (match) {
              Environment *opEnv = new Environment(environment);
              opEnv->addVariable("this", left, left.structName);

              Token paramName = opDecl->getParams().get(0).get(1);
              Token paramTypeToken = opDecl->getParams().get(0).get(0);
              opEnv->addVariable(paramName.getValue(), right,
                                 paramTypeToken.getValue());

              Environment *previousEnv = environment;
              environment = opEnv;

              Value returnValue;
              try {
                for (int j = 0; j < opDecl->getBody()->getStatements().size();
                     j++) {
                  execute(opDecl->getBody()->getStatements().get(j));
                }
              } catch (const ReturnException &e) {
                returnValue = e.value;
              }

              environment = previousEnv;
              delete opEnv;

              return returnValue;
            }
          } else if (opDecl->getParams().size() == 2) {
            std::string leftParamType =
                opDecl->getParams().get(0).get(0).getValue();
            std::string rightParamType =
                opDecl->getParams().get(1).get(0).getValue();

            // Check if first param matches 'this' struct type (left operand)
            if (leftParamType != left.structName) {
              continue;
            }

            bool match = false;
            // Check right param compatibility
            if (rightParamType == "Int" || rightParamType == "Integer") {
              if (right.isInt())
                match = true;
            } else if (rightParamType == "Number") {
              if (right.isNumber())
                match = true;
            } else if (rightParamType == "String") {
              if (right.isString())
                match = true;
            } else if (rightParamType == "Boolean") {
              if (right.isBool())
                match = true;
            } else if (right.getType() == VAL_STRUCT &&
                       right.structName == rightParamType) {
              match = true;
            }

            if (match) {
              Environment *opEnv = new Environment(environment);
              opEnv->addVariable("this", left, left.structName);

              // Param 1 -> Left (also 'this', but named as param)
              Token leftParamName = opDecl->getParams().get(0).get(1);
              Token leftParamTypeToken = opDecl->getParams().get(0).get(0);
              opEnv->addVariable(leftParamName.getValue(), left,
                                 leftParamTypeToken.getValue());

              // Param 2 -> Right
              Token rightParamName = opDecl->getParams().get(1).get(1);
              Token rightParamTypeToken = opDecl->getParams().get(1).get(0);
              opEnv->addVariable(rightParamName.getValue(), right,
                                 rightParamTypeToken.getValue());

              Environment *previousEnv = environment;
              environment = opEnv;

              Value returnValue;
              try {
                for (int j = 0; j < opDecl->getBody()->getStatements().size();
                     j++) {
                  execute(opDecl->getBody()->getStatements().get(j));
                }
              } catch (const ReturnException &e) {
                returnValue = e.value;
              }

              environment = previousEnv;
              delete opEnv;

              return returnValue;
            }
          }
        }
      }
    }

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

    if (calleeExpr->getType() == VARIABLE) {
      std::string name = calleeExpr->getToken().getValue();

      if (environment->functionExist(name)) {
        Function func = environment->getFunction(name);
        List<Expression *> argExprs =
            ((CallExpression *)expression)->getArguments();

        if (argExprs.size() != func.params.size()) {
          throw std::runtime_error("Function '" + name + "' expects " +
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
      } else if (environment->classExist(name)) {
        ClassDeclarationStatement *cls = environment->getClass(name);
        List<Expression *> argExprs =
            ((CallExpression *)expression)->getArguments();

        // Find constructor
        ConstructorDeclarationStatement *constructor = nullptr;
        for (int i = 0; i < cls->getConstructors().size(); i++) {
          if (cls->getConstructors().get(i)->getParams().size() ==
              argExprs.size()) {
            constructor = cls->getConstructors().get(i);
            break;
          }
        }

        if (constructor == nullptr) {
          // Allow default constructor if 0 args and no constructors defined?
          // Or stricter check.
          if (argExprs.size() == 0 && cls->getConstructors().size() == 0) {
            // Default init fields but no constructor logic
          } else {
            throw std::runtime_error(
                "No matching constructor for class '" + name + "' with " +
                std::to_string(argExprs.size()) + " arguments");
          }
        }

        std::map<std::string, Value> members;

        // Init fields with default Null, including superclasses
        ClassDeclarationStatement *currentCls = cls;
        while (currentCls != nullptr) {
          for (int i = 0; i < currentCls->getFields().size(); i++) {
            List<Token> field = currentCls->getFields().get(i);
            std::string fieldName = field.get(1).getValue();
            if (members.find(fieldName) == members.end()) {
              members[fieldName] = Value();
            }
          }

          if (currentCls->getSuperclass().getType() == TokenType::IDENTIFIER) {
            std::string superName = currentCls->getSuperclass().getValue();
            if (environment->classExist(superName)) {
              currentCls = environment->getClass(superName);
            } else {
              throw std::runtime_error("Superclass '" + superName +
                                       "' not found");
            }
          } else {
            currentCls = nullptr;
          }
        }

        Value instance(name, &members);

        if (constructor != nullptr) {
          List<Value> evaluatedArgs;
          for (int i = 0; i < argExprs.size(); i++) {
            evaluatedArgs.append(evaluate(argExprs.get(i)));
          }

          Environment *ctorEnv = new Environment(environment);
          ctorEnv->addVariable("this", instance, name); // Allow binding 'this'

          for (int i = 0; i < evaluatedArgs.size(); i++) {
            Token paramNameToken = constructor->getParams().get(i).get(1);
            Token paramTypeToken = constructor->getParams().get(i).get(0);
            ctorEnv->addVariable(paramNameToken.getValue(),
                                 evaluatedArgs.get(i),
                                 paramTypeToken.getValue());
          }

          Environment *previousEnv = environment;
          environment = ctorEnv;

          try {
            // Execute constructor body
            BlockStatement *body = constructor->getBody();
            for (int i = 0; i < body->getStatements().size(); i++) {
              execute(body->getStatements().get(i));
            }
          } catch (const ReturnException &e) {
          }

          environment = previousEnv;
          delete ctorEnv;
        }

        return instance;
      } else if (environment->structExist(name)) {
        StructDeclarationStatement *str = environment->getStruct(name);
        List<Expression *> argExprs =
            ((CallExpression *)expression)->getArguments();

        if (argExprs.size() != str->getFields().size()) {
          throw std::runtime_error("Struct '" + name + "' expects " +
                                   std::to_string(str->getFields().size()) +
                                   " arguments.");
        }

        std::map<std::string, Value> members;
        for (int i = 0; i < str->getFields().size(); i++) {
          List<Token> field = str->getFields().get(i);
          std::string fieldName = field.get(1).getValue();
          members[fieldName] = evaluate(argExprs.get(i));
        }

        return Value(name, &members);
      }
    } else if (calleeExpr->getType() == GET) {
      // Method call
      GetExpression *getExpr = (GetExpression *)calleeExpr;
      Value object = evaluate(getExpr->getObject());
      std::string methodName = getExpr->getName().getValue();

      if (object.getType() == VAL_STRUCT) {
        std::string className = object.structName;

        if (!environment->classExist(className)) {
          throw std::runtime_error("Class '" + className + "' not found.");
        }

        ClassDeclarationStatement *cls = environment->getClass(className);

        // Find method (instance or static? usually instance on instance)
        FunctionDeclarationStatement *method = nullptr;
        ClassDeclarationStatement *currentCls = cls;

        while (currentCls != nullptr && method == nullptr) {
          for (int i = 0; i < currentCls->getMethods().size(); i++) {
            if (currentCls->getMethods().get(i)->getName().getValue() ==
                methodName) {
              method = currentCls->getMethods().get(i);
              break;
            }
          }

          if (method == nullptr) {
            if (currentCls->getSuperclass().getType() ==
                TokenType::IDENTIFIER) {
              std::string superName = currentCls->getSuperclass().getValue();
              if (environment->classExist(superName)) {
                currentCls = environment->getClass(superName);
              } else {
                break;
              }
            } else {
              currentCls = nullptr;
            }
          }
        }

        if (method == nullptr) {
          throw std::runtime_error("Method '" + methodName +
                                   "' not found in class '" + className + "'.");
        }

        if (method->getIsStatic()) {
          throw std::runtime_error("Cannot call static method '" + methodName +
                                   "' on an instance.");
        }

        List<Expression *> argExprs =
            ((CallExpression *)expression)->getArguments();
        if (argExprs.size() != method->getParams().size()) {
          throw std::runtime_error("Method '" + methodName + "' expects " +
                                   std::to_string(method->getParams().size()) +
                                   " arguments.");
        }

        List<Value> evaluatedArgs;
        for (int i = 0; i < argExprs.size(); i++) {
          evaluatedArgs.append(evaluate(argExprs.get(i)));
        }

        Environment *methodEnv = new Environment(environment);
        methodEnv->addVariable("this", object, className);

        for (int i = 0; i < evaluatedArgs.size(); i++) {
          Token paramNameToken = method->getParams().get(i).get(1);
          Token paramTypeToken = method->getParams().get(i).get(0);
          methodEnv->addVariable(paramNameToken.getValue(),
                                 evaluatedArgs.get(i),
                                 paramTypeToken.getValue());
        }

        Environment *previousEnv = environment;
        environment = methodEnv;

        Value returnValue;
        try {
          BlockStatement *body = method->getBody();
          for (int i = 0; i < body->getStatements().size(); i++) {
            execute(body->getStatements().get(i));
          }
        } catch (const ReturnException &e) {
          returnValue = e.value;
        }

        environment = previousEnv;
        delete methodEnv;

        return returnValue;
      } else if (object.getType() == VAL_CLASS) {
        std::string className =
            object.asString(); // VAL_CLASS stores class name in stringValue?
                               // Check Value.cpp constructor.
        // Value(string value) sets VAL_STRING.
        // We need to verify how VAL_CLASS is created.
        // Assuming VAL_CLASS stores name in stringValue.

        if (!environment->classExist(className)) {
          throw std::runtime_error("Class '" + className + "' not found.");
        }
        ClassDeclarationStatement *cls = environment->getClass(className);

        // Find static method
        FunctionDeclarationStatement *method = nullptr;
        // Static methods do not inherit? Or do they? In Java they do.
        // Let's assume no inheritance for static first, or implement if easy.
        // Inheritance makes sense.
        ClassDeclarationStatement *currentCls = cls;
        while (currentCls != nullptr && method == nullptr) {
          for (int i = 0; i < currentCls->getMethods().size(); i++) {
            if (currentCls->getMethods().get(i)->getName().getValue() ==
                methodName) {
              method = currentCls->getMethods().get(i);
              break;
            }
          }
          if (method == nullptr) {
            if (currentCls->getSuperclass().getType() ==
                TokenType::IDENTIFIER) {
              std::string superName = currentCls->getSuperclass().getValue();
              if (environment->classExist(superName)) {
                currentCls = environment->getClass(superName);
              } else {
                break;
              }
            } else {
              currentCls = nullptr;
            }
          }
        }

        if (method == nullptr) {
          throw std::runtime_error("Static method '" + methodName +
                                   "' not found in class '" + className + "'.");
        }

        if (!method->getIsStatic()) {
          throw std::runtime_error("Method '" + methodName +
                                   "' is not static.");
        }

        List<Expression *> argExprs =
            ((CallExpression *)expression)->getArguments();
        if (argExprs.size() != method->getParams().size()) {
          throw std::runtime_error("Method '" + methodName + "' expects " +
                                   std::to_string(method->getParams().size()) +
                                   " arguments.");
        }

        List<Value> evaluatedArgs;
        for (int i = 0; i < argExprs.size(); i++) {
          evaluatedArgs.append(evaluate(argExprs.get(i)));
        }

        Environment *methodEnv = new Environment(environment);
        // No 'this' for static methods!

        for (int i = 0; i < evaluatedArgs.size(); i++) {
          Token paramNameToken = method->getParams().get(i).get(1);
          Token paramTypeToken = method->getParams().get(i).get(0);
          methodEnv->addVariable(paramNameToken.getValue(),
                                 evaluatedArgs.get(i),
                                 paramTypeToken.getValue());
        }

        Environment *previousEnv = environment;
        environment = methodEnv;

        Value returnValue;
        try {
          BlockStatement *body = method->getBody();
          for (int i = 0; i < body->getStatements().size(); i++) {
            execute(body->getStatements().get(i));
          }
        } catch (const ReturnException &e) {
          returnValue = e.value;
        }
        environment = previousEnv;
        delete methodEnv;
        return returnValue;

      } else {
        throw std::runtime_error("Only instances or classes have methods.");
      }

    } else {
      throw std::runtime_error(
          "Only direct function calls or method calls are supported");
    }
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
  case TokenType::CHAR:
    if (value.length() > 0) {
      return Value(value[0]);
    }
    return Value(); // Should not happen for valid CHAR tokens
  case TokenType::NULL_TOKEN:
    return Value();
  default:
    return Value();
  }
}