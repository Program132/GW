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
  if (this->peek().getType() == type && this->peek().getValue() == value) {
    return this->advance();
  }
  throw std::runtime_error(message);
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
  if (this->matchOperator("-") || this->matchOperator("!")) {
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
  if (t == TokenType::BOOLEAN || t == TokenType::INT ||
      t == TokenType::NUMBER || t == TokenType::STRING) {
    advance();
    return new LiteralExpression(this->previous());
  }

  if (checkType(TokenType::IDENTIFIER)) {
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

FunctionDeclarationStatement *Parser::functionDeclarationStatement() {
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

  return new FunctionDeclarationStatement(name, parameters, block, returnType);
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