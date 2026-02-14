#include "Parser.h"

Parser::Parser(List<Token> tokens) : tokens(tokens), current(0) {}

void Parser::parse() {
  while (!isAtEnd()) {
    statements.append(statement());
  }
}

List<Statement> Parser::getStatements() const { return statements; }

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

Expression Parser::getExpression() { return this->assignment(); }

Expression Parser::assignment() {
  Expression expression = this->logicalOr();

  if (this->checkType(TokenType::COMPOUND_ASSIGN_OPERATOR)) {
    Token op = this->advance();
    Expression value = this->assignment();
    std::string base_op_value = op.getValue().substr(
        0, op.getValue().size() - 1); // first character of the operator
    Token baseOp = Token(TokenType::MATH_OPERATOR, base_op_value, op.getLine());

    if (expression.getType() == ExpressionType::VARIABLE) {
      BinaryExpression nvalue = BinaryExpression(expression, baseOp, value);
      return AssignExpression(expression.getToken(), nvalue);
    } else if (expression.getType() == ExpressionType::GET) {
      GetExpression &expr = (GetExpression &)expression;
      BinaryExpression nvalue = BinaryExpression(expression, baseOp, value);
      return SetExpression(expr.getObject(), expression.getToken(), nvalue);
    } else {
      throw std::runtime_error("Error: Invalid target assignation line " +
                               std::to_string(op.getLine()));
    }
  } else if (this->matchOperator("=")) {
    Token previous = this->previous();
    Expression value = this->assignment();

    if (expression.getType() == ExpressionType::VARIABLE) {
      return AssignExpression(expression.getToken(), value);
    } else if (expression.getType() == ExpressionType::GET) {
      GetExpression &expr = (GetExpression &)expression;
      return SetExpression(expr.getObject(), expression.getToken(), value);
    } else {
      throw std::runtime_error("Error: Invalid target assignation line " +
                               std::to_string(previous.getLine()));
    }
  }
  return expression;
}

Expression Parser::logicalOr() {
  Expression expression = this->logicalAnd();

  while (this->matchOperator("|")) {
    Token op = this->previous();
    Expression right = this->logicalAnd();
    expression = BinaryExpression(expression, op, right);
  }

  return expression;
}

Expression Parser::logicalAnd() {
  Expression expression = this->equality();

  while (this->matchOperator("&")) {
    Token op = this->previous();
    Expression right = this->equality();
    expression = BinaryExpression(expression, op, right);
  }

  return expression;
}

Expression Parser::equality() {
  Expression expression = this->comparison();

  while (this->matchOperator("==") || this->matchOperator("~=")) {
    Token op = this->previous();
    Expression right = this->comparison();
    expression = BinaryExpression(expression, op, right);
  }

  return expression;
}

Expression Parser::comparison() {
  Expression expression = this->addition_subtraction();

  while (this->matchOperator("<") || this->matchOperator("<=") ||
         this->matchOperator(">") || this->matchOperator(">=")) {
    Token op = this->previous();
    Expression right = this->addition_subtraction();
    expression = BinaryExpression(expression, op, right);
  }

  return expression;
}

Expression Parser::addition_subtraction() {
  Expression expression = this->multiplication_division();

  while (this->matchOperator("+") || this->matchOperator("-")) {
    Token op = this->previous();
    Expression right = this->multiplication_division();
    expression = BinaryExpression(expression, op, right);
  }

  return expression;
}

Expression Parser::multiplication_division() {
  Expression expression = this->unary();

  while (this->matchOperator("*") || this->matchOperator("/") ||
         this->matchOperator("%")) {
    Token op = this->previous();
    Expression right = this->unary();
    expression = BinaryExpression(expression, op, right);
  }

  return expression;
}

Expression Parser::unary() {
  if (this->matchOperator("!") || this->matchOperator("-")) {
    Token op = this->previous();
    Expression right = this->unary();
    return UnaryExpression(op, right);
  }
  return this->call();
}

Expression Parser::call() {
  Expression expression = this->primary();

  while (this->matchOperator("(")) {
    Token paren = this->previous();
    List<Expression> arguments;
    if (!this->checkOperator(")")) {
      arguments.append(this->getExpression());
      while (this->matchOperator(",")) {
        arguments.append(this->getExpression());
      }
    }
    this->consume(TokenType::OPERATOR, ")", "Expected ')'");
    expression = CallExpression(expression, paren, arguments);
  }

  return expression;
}

Expression Parser::primary() {
  if (this->matchOperator("(")) {
    Expression expression = this->getExpression();
    this->consume(TokenType::OPERATOR, ")", "Expected ')'");
    return GroupingExpression(expression);
  } else if (this->matchType(TokenType::INT) ||
             this->matchType(TokenType::NUMBER) ||
             this->matchType(TokenType::BOOLEAN) ||
             this->matchType(TokenType::STRING)) {
    return LiteralExpression(this->previous());
  } else if (this->matchType(TokenType::IDENTIFIER)) {
    return VariableExpression(this->previous());
  } else {
    throw std::runtime_error("Error: Unexpected token \"" +
                             this->peek().getValue() + "\" type " +
                             tokenTypeToString(this->peek().getType()) +
                             " line " + std::to_string(this->peek().getLine()));
  }
}

Statement Parser::statement() {
  if (checkIdentifier("print")) {
    return printStatement();
  } else if (checkIdentifier("println")) {
    return printlnStatement();
  } else {
    return expressionStatement();
  }
}

ExpressionStatement Parser::expressionStatement() {
  Expression expression = this->getExpression();
  this->consume(TokenType::OPERATOR, ";", "Expected ';'");
  return ExpressionStatement(expression);
}

PrintStatement Parser::printStatement() {
  this->advance();

  consume(TokenType::OPERATOR, "(", "Expected '('");
  Expression expression = EmptyExpression();
  if (!this->checkOperator(")")) {
    expression = this->getExpression();
  }

  consume(TokenType::OPERATOR, ")", "Expected ')'");
  this->consume(TokenType::OPERATOR, ";", "Expected ';'");

  return PrintStatement(expression);
}

PrintlnStatement Parser::printlnStatement() {
  this->advance();

  consume(TokenType::OPERATOR, "(", "Expected '('");
  Expression expression = EmptyExpression();
  if (!this->checkOperator(")")) {
    expression = this->getExpression();
  }

  consume(TokenType::OPERATOR, ")", "Expected ')'");
  this->consume(TokenType::OPERATOR, ";", "Expected ';'");

  return PrintlnStatement(expression);
}