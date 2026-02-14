#ifndef PARSER_H
#define PARSER_H

#include "../../lexer/Token/Token.h"
#include "../../utils/List.h"
#include "../ASTNodes/Expression.h"
#include "../ASTNodes/Statement.h"

class Parser {
private:
  List<Token> tokens;
  int current;
  List<Statement> statements;

public:
  Parser(List<Token> tokens);

  void parse();
  List<Statement> getStatements() const;
  bool isAtEnd() const;
  Statement statement();

  bool checkIdentifier(std::string name);
  bool checkType(TokenType type);
  bool checkOperator(std::string value);
  bool matchOperator(std::string value);
  bool matchType(List<TokenType> types);

  Token peek();
  Token previous();
  Token advance();
  Token peek_ahead(int offset);
  Token consume(TokenType type, std::string value, std::string message);

  Expression getExpression();
  Expression primary();
  Expression assignment();
  Expression logicalOr();
  Expression logicalAnd();
  Expression equality();
  Expression comparison();
  Expression addition_subtraction();
  Expression multiplication_division();
  Expression unary();
  Expression call();

  ExpressionStatement expressionStatement();
  PrintStatement printStatement();
  PrintlnStatement printlnStatement();
};

#endif // PARSER_H