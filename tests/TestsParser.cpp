#include "TestsParser.h"
#include "../src/lexer/Lexer/Lexer.h"
#include "../src/parser/Parser/Parser.h"
#include "../src/utils/Assert.h"

void testParserExpressions() {
  // Literals
  {
    Lexer lex("42 \"hello\" true false");
    lex.lex();
    Parser p(lex.getTokens());
    GW_ASSERT(p.getExpression().getType() == ExpressionType::LITERAL);
    GW_ASSERT(p.getExpression().getType() == ExpressionType::LITERAL);
    GW_ASSERT(p.getExpression().getType() == ExpressionType::LITERAL);
    GW_ASSERT(p.getExpression().getType() == ExpressionType::LITERAL);
  }

  // Unary
  {
    Lexer lex("-5 !true");
    lex.lex();
    Parser p(lex.getTokens());
    GW_ASSERT(p.getExpression().getType() == ExpressionType::UNARY);
    GW_ASSERT(p.getExpression().getType() == ExpressionType::UNARY);
  }

  // Binary Arithmetic
  {
    Lexer lex("1 + 2 * 3 / 4 - 5 % 6");
    lex.lex();
    Parser p(lex.getTokens());
    Expression expr = p.getExpression();
    GW_ASSERT(expr.getType() == ExpressionType::BINARY);
  }

  // Binary Logic & Comparison
  {
    Lexer lex("a < b & c == d | e >= f");
    lex.lex();
    Parser p(lex.getTokens());
    Expression expr = p.getExpression();
    GW_ASSERT(expr.getType() == ExpressionType::BINARY);
  }

  // Grouping
  {
    Lexer lex("(1 + 2) * 3");
    lex.lex();
    Parser p(lex.getTokens());
    Expression expr = p.getExpression();
    GW_ASSERT(expr.getType() == ExpressionType::BINARY);
    GW_ASSERT(expr.getToken().getValue() == "*");
  }
}

void testParserStatements() {
  // Multiple statements
  {
    Lexer lex("x = 5; y = 10; print(x + y); println();");
    lex.lex();
    Parser p(lex.getTokens());
    p.parse();
    List<Statement> stats = p.getStatements();
    GW_ASSERT(stats.size() == 4);
    GW_ASSERT(stats.get(0).getType() == StatementType::EXPRESSION_STATEMENT);
    GW_ASSERT(stats.get(1).getType() == StatementType::EXPRESSION_STATEMENT);
    GW_ASSERT(stats.get(2).getType() == StatementType::PRINT_STATEMENT);
    GW_ASSERT(stats.get(3).getType() == StatementType::PRINTLN_STATEMENT);
  }

  // Empty print/println
  {
    Lexer lex("print(); println();");
    lex.lex();
    Parser p(lex.getTokens());
    p.parse();
    List<Statement> stats = p.getStatements();
    GW_ASSERT(stats.size() == 2);
  }
}

void testParserPrecedence() {
  // multiplication before addition
  {
    Lexer lex("1 + 2 * 3");
    lex.lex();
    Parser p(lex.getTokens());
    Expression expr = p.getExpression();
    GW_ASSERT(expr.getType() == ExpressionType::BINARY);
    GW_ASSERT(expr.getToken().getValue() == "+"); // + should be root
  }

  // Comparison before logical
  {
    Lexer lex("a < b & c > d");
    lex.lex();
    Parser p(lex.getTokens());
    Expression expr = p.getExpression();
    GW_ASSERT(expr.getType() == ExpressionType::BINARY);
    GW_ASSERT(expr.getToken().getValue() == "&"); // & should be root
  }

  // Logical And before Logical Or
  {
    Lexer lex("a | b & c");
    lex.lex();
    Parser p(lex.getTokens());
    Expression expr = p.getExpression();
    GW_ASSERT(expr.getType() == ExpressionType::BINARY);
    GW_ASSERT(expr.getToken().getValue() == "|"); // | should be root
  }

  // Assignment is right-associative and lowest precedence
  {
    Lexer lex("a = b = 5");
    lex.lex();
    Parser p(lex.getTokens());
    Expression expr = p.getExpression();
    GW_ASSERT(expr.getType() == ExpressionType::ASSIGN);
  }
}
