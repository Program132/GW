#include "Parser.h"
#include <string>

Parser::Parser(List<Token> tokens) : tokens(tokens), current(0) {}

void Parser::parse() {
  while (!isAtEnd()) {
    statements.append(statement());
  }
}

std::ostream &operator<<(std::ostream &os, const Parser &parser) {
  os << "Statements:" << std::endl;
  for (int i = 0; i < parser.statements.size(); i++) {
    os << "  " << *parser.statements.get(i) << std::endl;
  }
  return os;
}

List<Statement *> Parser::getStatements() const { return statements; }

bool Parser::isAtEnd() const { return this->current >= this->tokens.size(); }

bool Parser::checkIdentifier(std::string name) {
  if (isAtEnd())
    return false;
  return peek().getType() == TokenType::IDENTIFIER && peek().getValue() == name;
}

bool Parser::checkType(TokenType type) {
  if (isAtEnd())
    return false;
  return peek().getType() == type;
}

bool Parser::checkOperator(std::string value) {
  if (isAtEnd())
    return false;
  TokenType type = peek().getType();
  return (type == TokenType::OPERATOR || type == TokenType::MATH_OPERATOR ||
          type == TokenType::EQUAL_OPERATOR ||
          type == TokenType::DOUBLE_EQUAL_OPERATOR ||
          type == TokenType::LESS_OPERATOR ||
          type == TokenType::GREATER_OPERATOR ||
          type == TokenType::LESS_EQUAL_OPERATOR ||
          type == TokenType::GREATER_EQUAL_OPERATOR ||
          type == TokenType::DIFFERENT_OPERATOR ||
          type == TokenType::AND_OPERATOR || type == TokenType::OR_OPERATOR ||
          type == TokenType::NOT_OPERATOR) &&
         peek().getValue() == value;
}

bool Parser::matchOperator(std::string value) {
  if (checkOperator(value)) {
    advance();
    return true;
  }
  return false;
}

bool Parser::matchType(List<TokenType> types) {
  for (int i = 0; i < types.size(); i++) {
    if (checkType(types.get(i))) {
      advance();
      return true;
    }
  }
  return false;
}

Token Parser::peek() { return this->tokens.get(this->current); }

Token Parser::previous() { return this->tokens.get(this->current - 1); }

Token Parser::advance() { return this->tokens.get(this->current++); }

Token Parser::peek_ahead(int offset) {
  return this->tokens.get(this->current + offset);
}

Token Parser::consume(TokenType type, std::string value, std::string message) {
  if (this->peek().getType() == type &&
      (value == "" || this->peek().getValue() == value)) {
    return this->advance();
  }
  throw std::runtime_error(message + " (got '" + this->peek().getValue() +
                           "' of type " +
                           std::to_string(this->peek().getType()) + ")");
}

Expression *Parser::getExpression() { return this->assignment(); }

Expression *Parser::assignment() {
  Expression *expression = this->logicalOr();

  if (this->checkType(TokenType::COMPOUND_ASSIGN_OPERATOR)) {
    Token op = this->advance();
    Expression *value = this->assignment();
    std::string base_op_value = op.getValue().substr(
        0, op.getValue().size() - 1); // first character of the operator
    Token baseOp = Token(TokenType::MATH_OPERATOR, base_op_value, op.getLine());

    if (expression->getType() == ExpressionType::VARIABLE) {
      BinaryExpression *nvalue =
          new BinaryExpression(expression, baseOp, value);
      return new AssignExpression(expression->getToken(), nvalue);
    } else if (expression->getType() == ExpressionType::GET) {
      GetExpression *expr = (GetExpression *)expression;
      BinaryExpression *nvalue =
          new BinaryExpression(expression, baseOp, value);
      return new SetExpression(expr->getObject(), expression->getToken(),
                               nvalue);
    } else {
      throw std::runtime_error("Error: Invalid target assignation line " +
                               std::to_string(op.getLine()));
    }
  }

  if (this->matchOperator("=")) {
    Token op = this->previous();
    Expression *value = this->assignment();

    if (expression->getType() == ExpressionType::VARIABLE) {
      return new AssignExpression(expression->getToken(), value);
    } else if (expression->getType() == ExpressionType::GET) {
      GetExpression *expr = (GetExpression *)expression;
      return new SetExpression(expr->getObject(), expression->getToken(),
                               value);
    }

    throw std::runtime_error("Error: Invalid target assignation line " +
                             std::to_string(op.getLine()));
  }

  return expression;
}

Expression *Parser::logicalOr() {
  Expression *expression = this->logicalAnd();

  while (this->matchOperator("|") || this->matchOperator("||")) {
    Token op = this->previous();
    Expression *right = this->logicalAnd();
    expression = new BinaryExpression(expression, op, right);
  }

  return expression;
}

Expression *Parser::logicalAnd() {
  Expression *expression = this->equality();

  while (this->matchOperator("&") || this->matchOperator("&&")) {
    Token op = this->previous();
    Expression *right = this->equality();
    expression = new BinaryExpression(expression, op, right);
  }

  return expression;
}

Expression *Parser::equality() {
  Expression *expression = this->comparison();

  while (this->matchOperator("==") || this->matchOperator("~=")) {
    Token op = this->previous();
    Expression *right = this->comparison();
    expression = new BinaryExpression(expression, op, right);
  }

  return expression;
}

Expression *Parser::comparison() {
  Expression *expression = this->addition_subtraction();

  while (this->matchOperator("<") || this->matchOperator(">") ||
         this->matchOperator("<=") || this->matchOperator(">=")) {
    Token op = this->previous();
    Expression *right = this->addition_subtraction();
    expression = new BinaryExpression(expression, op, right);
  }

  return expression;
}

Expression *Parser::addition_subtraction() {
  Expression *expression = this->multiplication_division();

  while (this->matchOperator("+") || this->matchOperator("-")) {
    Token op = this->previous();
    Expression *right = this->multiplication_division();
    expression = new BinaryExpression(expression, op, right);
  }

  return expression;
}

Expression *Parser::multiplication_division() {
  Expression *expression = this->exponentiation();

  while (this->matchOperator("*") || this->matchOperator("/") ||
         this->matchOperator("%")) {
    Token op = this->previous();
    Expression *right = this->exponentiation();
    expression = new BinaryExpression(expression, op, right);
  }

  return expression;
}

Expression *Parser::exponentiation() {
  Expression *expression = this->unary();

  while (this->matchOperator("^")) {
    Token op = this->previous();
    Expression *right = this->unary();
    expression = new BinaryExpression(expression, op, right);
  }

  return expression;
}

Expression *Parser::unary() {
  if (this->matchOperator("-") || this->matchOperator("!") ||
      this->matchOperator("+")) {
    Token op = this->previous();
    Expression *right = this->unary();
    return new UnaryExpression(op, right);
  }
  return this->call();
}

Expression *Parser::call() {
  Expression *expression = this->primary();

  while (true) {
    if (this->matchOperator("(")) {
      List<Expression *> arguments;
      if (!this->checkOperator(")")) {
        do {
          arguments.append(this->getExpression());
        } while (this->matchOperator(","));
      }

      Token paren = this->consume(TokenType::OPERATOR, ")", "Expected ')'");
      expression = new CallExpression(expression, paren, arguments);
    } else if (this->matchOperator(".")) {
      Token name =
          this->consume(TokenType::IDENTIFIER, "", "Expected property name");
      expression = new GetExpression(expression, name);
    } else {
      break;
    }
  }

  return expression;
}

Expression *Parser::primary() {
  TokenType t = peek().getType();
  if (t == TokenType::BOOLEAN_TOKEN || t == TokenType::INT_TOKEN ||
      t == TokenType::NUMBER_TOKEN || t == TokenType::STRING ||
      t == TokenType::CHAR_TOKEN || t == TokenType::NULL_TOKEN) {
    advance();
    return new LiteralExpression(this->previous());
  }

  if (checkType(TokenType::IDENTIFIER)) {
    // Handle 'new ClassName<T>()' — skip optional generic type params
    if (peek().getValue() == "new") {
      advance(); // consume 'new'
      if (!checkType(TokenType::IDENTIFIER)) {
        throw std::runtime_error("Expected class name after 'new' at line " +
                                 std::to_string(peek().getLine()));
      }
      Token className = advance(); // consume class name
      // Skip optional generic type params: <Integer>, <T>, etc.
      if (checkType(TokenType::LESS_OPERATOR)) {
        advance(); // consume '<'
        int depth = 1;
        while (depth > 0 && !isAtEnd()) {
          if (checkType(TokenType::LESS_OPERATOR))
            depth++;
          else if (checkType(TokenType::GREATER_OPERATOR))
            depth--;
          advance();
        }
      }
      return new VariableExpression(className);
    }
    advance();
    return new VariableExpression(this->previous());
  }

  if (this->matchOperator("(")) {
    Expression *expression = this->getExpression();
    this->consume(TokenType::OPERATOR, ")", "Expected ')'");
    return new GroupingExpression(expression);
  }

  throw std::runtime_error("Expected expression line " +
                           std::to_string(peek().getLine()) + " got '" +
                           peek().getValue() + "'");
}

Statement *Parser::statement() {
  if (checkOperator("{")) {
    advance();
    return blockStatement();
  }

  if (checkIdentifier("print")) {
    return printStatement();
  } else if (checkIdentifier("println")) {
    return printlnStatement();
  } else if (checkIdentifier("var")) {
    return varDeclarationStatement();
  } else if (checkIdentifier("func")) {
    return functionDeclarationStatement();
  } else if (checkIdentifier("return")) {
    return returnStatement();
  } else if (checkIdentifier("break")) {
    return breakStatement();
  } else if (checkIdentifier("continue")) {
    return continueStatement();
  } else if (checkIdentifier("if")) {
    return ifStatement();
  } else if (checkIdentifier("while")) {
    return whileStatement();
  } else if (checkIdentifier("for")) {
    return forStatement();
  } else if (checkIdentifier("struct")) {
    return structDeclarationStatement();
  } else if (checkIdentifier("class")) {
    return classDeclarationStatement();
  } else {
    return expressionStatement();
  }
}

ExpressionStatement *Parser::expressionStatement() {
  Expression *expression = this->getExpression();
  this->consume(TokenType::OPERATOR, ";", "Expected ';'");
  return new ExpressionStatement(expression);
}

PrintStatement *Parser::printStatement() {
  this->advance(); // consume 'print'
  this->consume(TokenType::OPERATOR, "(", "Expected '(' after print");
  Expression *expression = this->getExpression();
  this->consume(TokenType::OPERATOR, ")", "Expected ')' after expression");
  this->consume(TokenType::OPERATOR, ";", "Expected ';' after print statement");
  return new PrintStatement(expression);
}

PrintlnStatement *Parser::printlnStatement() {
  this->advance(); // consume 'println'
  this->consume(TokenType::OPERATOR, "(", "Expected '(' after println");
  Expression *expression = this->getExpression();
  this->consume(TokenType::OPERATOR, ")", "Expected ')' after expression");
  this->consume(TokenType::OPERATOR, ";",
                "Expected ';' after println statement");
  return new PrintlnStatement(expression);
}

VarDeclarationStatement *Parser::varDeclarationStatement() {
  this->advance(); // consume 'var'
  Token name = this->consume(TokenType::IDENTIFIER, "", "Expected identifier");

  Expression *initializer = nullptr;
  if (this->matchOperator("=")) {
    initializer = this->getExpression();
  }

  this->consume(TokenType::OPERATOR, ";",
                "Expected ';' after variable declaration");
  return new VarDeclarationStatement(name, initializer);
}

FunctionDeclarationStatement *
Parser::functionDeclarationStatement(bool isStatic) {
  this->advance(); // consume 'func'

  if (!this->checkType(TokenType::IDENTIFIER)) {
    throw std::runtime_error("Error: Expected identifier");
  }

  Token name = this->advance();

  this->consume(TokenType::OPERATOR, "(",
                "Expected '(' line " + std::to_string(name.getLine()));

  List<List<Token>> parameters;
  if (!this->checkOperator(")")) {
    while (true) {
      if (!this->checkType(TokenType::IDENTIFIER)) {
        throw std::runtime_error("Expected parameter name line " +
                                 std::to_string(peek().getLine()));
      }
      Token paramName = this->advance();
      this->consume(TokenType::OPERATOR, ":",
                    "Expected ':' after parameter name");

      Token paramTypeToken = this->advance();
      List<Token> parameter;
      parameter.append(paramTypeToken);
      parameter.append(paramName);
      parameters.append(parameter);

      if (!this->matchOperator(",")) {
        break;
      }
    }
  }

  this->consume(TokenType::OPERATOR, ")", "Expected ')'");

  DataTypes returnType = DataTypes::Null;
  if (this->matchOperator("->")) {
    Token returnTypeToken = this->advance();
    returnType = toDataTypes(returnTypeToken);
  }

  this->consume(TokenType::OPERATOR, "{",
                "Expected '{' line " + std::to_string(name.getLine()));

  BlockStatement *block = this->blockStatement();

  return new FunctionDeclarationStatement(name, parameters, block, returnType,
                                          isStatic);
}

ReturnStatement *Parser::returnStatement() {
  this->advance(); // consume 'return'
  Expression *value = nullptr;
  if (!this->checkOperator(";")) {
    value = this->getExpression();
  }
  this->consume(TokenType::OPERATOR, ";", "Expected ';' after return value");
  return new ReturnStatement(value);
}

BreakStatement *Parser::breakStatement() {
  this->advance(); // consume 'break'
  this->consume(TokenType::OPERATOR, ";", "Expected ';' after break");
  return new BreakStatement();
}

ContinueStatement *Parser::continueStatement() {
  this->advance(); // consume 'continue'
  this->consume(TokenType::OPERATOR, ";", "Expected ';' after continue");
  return new ContinueStatement();
}

BlockStatement *Parser::blockStatement() {
  List<Statement *> blockStatements;
  while (!this->checkOperator("}") && !this->isAtEnd()) {
    blockStatements.append(this->statement());
  }

  this->consume(TokenType::OPERATOR, "}", "Expected '}' after block");

  return new BlockStatement(blockStatements);
}

IfStatement *Parser::ifStatement() {
  this->advance(); // consume 'if'

  this->consume(TokenType::OPERATOR, "(", "Expected '(' after 'if'");
  Expression *condition = this->getExpression();
  this->consume(TokenType::OPERATOR, ")", "Expected ')' after condition");

  this->consume(TokenType::OPERATOR, "{", "Expected '{' after condition");
  BlockStatement *then_branch = this->blockStatement();

  BlockStatement *else_branch = nullptr;
  if (this->checkIdentifier("else")) {
    this->advance(); // consume 'else'
    this->consume(TokenType::OPERATOR, "{", "Expected '{' after 'else'");
    else_branch = this->blockStatement();
  }
  return new IfStatement(condition, then_branch, else_branch);
}

WhileStatement *Parser::whileStatement() {
  this->advance(); // consume 'while'

  this->consume(TokenType::OPERATOR, "(", "Expected '(' after 'while'");
  Expression *condition = this->getExpression();
  this->consume(TokenType::OPERATOR, ")", "Expected ')' after condition");

  this->consume(TokenType::OPERATOR, "{", "Expected '{' after condition");
  BlockStatement *body = this->blockStatement();

  return new WhileStatement(condition, body);
}

ForStatement *Parser::forStatement() {
  this->advance(); // consume 'for'

  this->consume(TokenType::OPERATOR, "(", "Expected '(' after 'for'");

  Statement *initializer = nullptr;
  if (this->matchOperator(";")) {
    initializer = nullptr;
  } else if (this->checkIdentifier("var")) {
    initializer = this->varDeclarationStatement();
  } else {
    initializer = this->expressionStatement();
  }

  Expression *condition = nullptr;
  if (!this->checkOperator(";")) {
    condition = this->getExpression();
  }
  this->consume(TokenType::OPERATOR, ";", "Expected ';' after loop condition");

  Expression *increment = nullptr;
  if (!this->checkOperator(")")) {
    increment = this->getExpression();
  }
  this->consume(TokenType::OPERATOR, ")", "Expected ')' after for clauses");

  this->consume(TokenType::OPERATOR, "{", "Expected '{' after for clauses");
  BlockStatement *body = this->blockStatement();

  return new ForStatement(initializer, condition, increment, body);
}

StructDeclarationStatement *Parser::structDeclarationStatement() {
  this->advance(); // consume 'struct'

  if (!this->checkType(TokenType::IDENTIFIER)) {
    throw std::runtime_error("Expected identifier line " +
                             std::to_string(peek().getLine()));
  }
  Token name = this->advance();

  this->consume(TokenType::OPERATOR, "{", "Expected '{' after struct name");

  List<List<Token>> fields;
  if (!this->checkOperator("}")) {
    while (!this->isAtEnd() && !this->checkOperator("}")) {
      Token fieldName =
          this->consume(TokenType::IDENTIFIER, "", "Expected field name");
      this->consume(TokenType::OPERATOR, ":", "Expected ':' after field name");

      if (!this->checkType(TokenType::IDENTIFIER)) {
        throw std::runtime_error("Expected type after field name");
      }
      Token typeToken = this->advance();

      List<Token> field;
      field.append(typeToken);
      field.append(fieldName);
      fields.append(field);

      if (!this->matchOperator(",")) {
        break;
      }
    }
  }

  this->consume(TokenType::OPERATOR, "}", "Expected '}' after struct body");

  return new StructDeclarationStatement(name, fields);
}

ClassDeclarationStatement *Parser::classDeclarationStatement() {
  this->advance(); // consume 'class'

  if (!this->checkType(TokenType::IDENTIFIER)) {
    throw std::runtime_error("Expected identifier line " +
                             std::to_string(peek().getLine()));
  }
  Token name = this->advance();

  Token superclass = Token();
  List<Token> typeParams;

  // Parse optional generic type parameters: class Foo<T, K> { ... }
  if (this->checkType(TokenType::LESS_OPERATOR)) {
    this->advance(); // consume '<'
    while (!this->checkType(TokenType::GREATER_OPERATOR) && !this->isAtEnd()) {
      if (!this->checkType(TokenType::IDENTIFIER)) {
        throw std::runtime_error("Expected type parameter name at line " +
                                 std::to_string(peek().getLine()));
      }
      typeParams.append(this->advance());
      if (this->checkOperator(","))
        this->advance();
    }
    this->consume(TokenType::GREATER_OPERATOR, ">",
                  "Expected '>' after type parameters");
  }

  if (this->checkType(TokenType::EXTENDS)) {
    this->advance();
    if (peek().getType() != TokenType::IDENTIFIER) {
      throw std::runtime_error("Expected identifier after extends at line " +
                               std::to_string(peek().getLine()));
    }
    superclass = this->advance();
  } else if (peek().getType() == TokenType::IDENTIFIER &&
             peek().getValue() == "extends") {
    // Fallback for manually checked identifier, though match(EXTENDS) should
    // work if lexer is correct. Lexer maps "extends" to EXTENDS token type.
    this->advance();
    if (peek().getType() != TokenType::IDENTIFIER) {
      throw std::runtime_error("Expected identifier after extends at line " +
                               std::to_string(peek().getLine()));
    }
    superclass = this->advance();
  }

  this->consume(TokenType::OPERATOR, "{", "Expected '{' after class name");

  List<List<Token>> fields;
  List<FunctionDeclarationStatement *> methods;
  List<ConstructorDeclarationStatement *> constructors;
  List<OperatorDeclarationStatement *> operators;

  while (!this->checkOperator("}") && !this->isAtEnd()) {
    bool isStatic = false;
    if (this->checkType(TokenType::STATIC_KEYWORD)) {
      this->advance();
      isStatic = true;
    } else if (this->checkType(TokenType::IDENTIFIER) &&
               peek().getValue() == "static") {
      this->advance();
      isStatic = true;
    }

    if (this->checkIdentifier("func")) {
      methods.append(this->functionDeclarationStatement(isStatic));
    } else if (this->checkIdentifier("constructor")) {
      if (isStatic)
        throw std::runtime_error("Constructor cannot be static");
      constructors.append(this->constructorDeclarationStatement());
    } else if (this->checkIdentifier("operator")) {
      if (isStatic)
        throw std::runtime_error("Operator cannot be static");
      operators.append(this->operatorDeclarationStatement());
    } else {
      if (isStatic) {
        // Static fields logic?
        // For now user requested static methods. Fields maybe later.
        // Let's implement static fields too if easy.
        // But strict adherence: "method en static".
        // If isStatic is true here, and we fall through to field parsing...
      }

      if (!this->checkType(TokenType::IDENTIFIER)) {
        throw std::runtime_error("Expected field or method declaration line " +
                                 std::to_string(peek().getLine()));
      }
      Token fieldName = this->advance();
      // Check if it's "constructor" without checking identifier specifically
      // (if lexer didn't map it) Actually, constructor is handled above by
      // checkIdentifier.

      this->consume(TokenType::OPERATOR, ":", "Expected ':' after field name");

      if (!this->checkType(TokenType::IDENTIFIER)) {
        throw std::runtime_error("Expected type after field name");
      }
      Token typeToken = this->advance();
      // Skip optional generic type params on field type: e.g. next: ListNode<T>
      if (this->checkType(TokenType::LESS_OPERATOR)) {
        this->advance(); // consume '<'
        int depth = 1;
        while (depth > 0 && !this->isAtEnd()) {
          if (this->checkType(TokenType::LESS_OPERATOR))
            depth++;
          else if (this->checkType(TokenType::GREATER_OPERATOR))
            depth--;
          this->advance();
        }
      }

      List<Token> field;
      field.append(typeToken);
      field.append(fieldName);
      fields.append(field);

      if (this->checkOperator(",")) {
        this->advance();
      }
    }
  }

  this->consume(TokenType::OPERATOR, "}", "Expected '}' after class body");

  return new ClassDeclarationStatement(name, superclass, typeParams, fields,
                                       methods, constructors, operators);
}

ConstructorDeclarationStatement *Parser::constructorDeclarationStatement() {
  this->advance(); // consume 'constructor'
  this->consume(TokenType::OPERATOR, "(", "Expected '(' after constructor");

  List<List<Token>> parameters;
  if (!this->checkOperator(")")) {
    while (true) {
      if (!this->checkType(TokenType::IDENTIFIER)) {
        throw std::runtime_error("Expected parameter name line " +
                                 std::to_string(peek().getLine()));
      }
      Token paramName = this->advance();
      this->consume(TokenType::OPERATOR, ":",
                    "Expected ':' after parameter name");

      Token paramTypeToken = this->advance();
      List<Token> parameter;
      parameter.append(paramTypeToken);
      parameter.append(paramName);
      parameters.append(parameter);

      if (!this->matchOperator(",")) {
        break;
      }
    }
  }

  this->consume(TokenType::OPERATOR, ")",
                "Expected ')' after constructor parameters");
  this->consume(TokenType::OPERATOR, "{", "Expected '{' after constructor");

  BlockStatement *body = this->blockStatement();
  return new ConstructorDeclarationStatement(parameters, body);
}

OperatorDeclarationStatement *Parser::operatorDeclarationStatement() {
  this->advance();            // consume 'operator'
  Token op = this->advance(); // consume operator

  this->consume(TokenType::OPERATOR, "(", "Expected '(' after operator");

  List<List<Token>> parameters;
  if (!this->checkOperator(")")) {
    while (true) {
      if (!this->checkType(TokenType::IDENTIFIER)) {
        throw std::runtime_error("Expected parameter name line " +
                                 std::to_string(peek().getLine()));
      }
      Token paramName = this->advance();
      this->consume(TokenType::OPERATOR, ":",
                    "Expected ':' after parameter name");

      Token paramTypeToken = this->advance();
      List<Token> parameter;
      parameter.append(paramTypeToken);
      parameter.append(paramName);
      parameters.append(parameter);

      if (!this->matchOperator(",")) {
        break;
      }
    }
  }

  this->consume(TokenType::OPERATOR, ")",
                "Expected ')' after operator parameters");

  DataTypes returnType = DataTypes::Null;
  if (this->matchOperator("->")) {
    Token returnTypeToken = this->advance();
    returnType = toDataTypes(returnTypeToken);
  }

  this->consume(TokenType::OPERATOR, "{", "Expected '{' after operator");

  BlockStatement *body = this->blockStatement();

  return new OperatorDeclarationStatement(op, parameters, body, returnType);
}