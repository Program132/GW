#include "TestsLexer.h"
#include "../src/lexer/Lexer/Lexer.h"
#include "../src/lexer/Token/Token.h"
#include "../src/lexer/Token/TokenType.h"
#include "../src/utils/Assert.h"
#include "../src/utils/List.h"

void testLexerIntegers() {
  Lexer lexer("42");
  lexer.lex();
  List<Token> tokens = lexer.getTokens();

  GW_ASSERT(tokens.size() == 1);
  GW_ASSERT(tokens.get(0).getType() == TokenType::INT);
  GW_ASSERT(tokens.get(0).getValue() == "42");
}

void testLexerNumbers() {
  Lexer lexer("3.14");
  lexer.lex();
  List<Token> tokens = lexer.getTokens();

  GW_ASSERT(tokens.size() == 1);
  GW_ASSERT(tokens.get(0).getType() == TokenType::NUMBER);
  GW_ASSERT(tokens.get(0).getValue() == "3.14");
}

void testLexerBooleans() {
  Lexer lexer1("true");
  lexer1.lex();
  List<Token> tokens1 = lexer1.getTokens();

  GW_ASSERT(tokens1.size() == 1);
  GW_ASSERT(tokens1.get(0).getType() == TokenType::BOOLEAN);
  GW_ASSERT(tokens1.get(0).getValue() == "true");

  Lexer lexer2("false");
  lexer2.lex();
  List<Token> tokens2 = lexer2.getTokens();

  GW_ASSERT(tokens2.size() == 1);
  GW_ASSERT(tokens2.get(0).getType() == TokenType::BOOLEAN);
  GW_ASSERT(tokens2.get(0).getValue() == "false");
}

void testLexerStrings() {
  Lexer lexer("\"Hello World\"");
  lexer.lex();
  List<Token> tokens = lexer.getTokens();

  GW_ASSERT(tokens.size() == 1);
  GW_ASSERT(tokens.get(0).getType() == TokenType::STRING);
  GW_ASSERT(tokens.get(0).getValue() == "Hello World");
}

void testLexerMathOperators() {
  Lexer lexer1("1 + 2");
  lexer1.lex();
  List<Token> tokens1 = lexer1.getTokens();

  GW_ASSERT(tokens1.size() == 3);
  GW_ASSERT(tokens1.get(0).getType() == TokenType::INT);
  GW_ASSERT(tokens1.get(1).getType() == TokenType::MATH_OPERATOR);
  GW_ASSERT(tokens1.get(1).getValue() == "+");
  GW_ASSERT(tokens1.get(2).getType() == TokenType::INT);

  Lexer lexer2("5 - 3");
  lexer2.lex();
  List<Token> tokens2 = lexer2.getTokens();

  GW_ASSERT(tokens2.size() == 3);
  GW_ASSERT(tokens2.get(1).getType() == TokenType::MATH_OPERATOR);
  GW_ASSERT(tokens2.get(1).getValue() == "-");

  Lexer lexer3("4 * 6");
  lexer3.lex();
  List<Token> tokens3 = lexer3.getTokens();

  GW_ASSERT(tokens3.size() == 3);
  GW_ASSERT(tokens3.get(1).getType() == TokenType::MATH_OPERATOR);
  GW_ASSERT(tokens3.get(1).getValue() == "*");

  Lexer lexer4("10 / 2");
  lexer4.lex();
  List<Token> tokens4 = lexer4.getTokens();

  GW_ASSERT(tokens4.size() == 3);
  GW_ASSERT(tokens4.get(1).getType() == TokenType::MATH_OPERATOR);
  GW_ASSERT(tokens4.get(1).getValue() == "/");
}

void testLexerComparisonOperators() {
  Lexer lexer1("a == b");
  lexer1.lex();
  List<Token> tokens1 = lexer1.getTokens();

  GW_ASSERT(tokens1.size() == 3);
  GW_ASSERT(tokens1.get(1).getType() == TokenType::DOUBLE_EQUAL_OPERATOR);
  GW_ASSERT(tokens1.get(1).getValue() == "==");

  Lexer lexer2("x < y");
  lexer2.lex();
  List<Token> tokens2 = lexer2.getTokens();

  GW_ASSERT(tokens2.size() == 3);
  GW_ASSERT(tokens2.get(1).getType() == TokenType::LESS_OPERATOR);
  GW_ASSERT(tokens2.get(1).getValue() == "<");

  Lexer lexer3("x > y");
  lexer3.lex();
  List<Token> tokens3 = lexer3.getTokens();

  GW_ASSERT(tokens3.size() == 3);
  GW_ASSERT(tokens3.get(1).getType() == TokenType::GREATER_OPERATOR);
  GW_ASSERT(tokens3.get(1).getValue() == ">");

  Lexer lexer4("x <= y");
  lexer4.lex();
  List<Token> tokens4 = lexer4.getTokens();

  GW_ASSERT(tokens4.size() == 3);
  GW_ASSERT(tokens4.get(1).getType() == TokenType::LESS_EQUAL_OPERATOR);
  GW_ASSERT(tokens4.get(1).getValue() == "<=");

  Lexer lexer5("x >= y");
  lexer5.lex();
  List<Token> tokens5 = lexer5.getTokens();

  GW_ASSERT(tokens5.size() == 3);
  GW_ASSERT(tokens5.get(1).getType() == TokenType::GREATER_EQUAL_OPERATOR);
  GW_ASSERT(tokens5.get(1).getValue() == ">=");
}

void testLexerBooleanOperators() {
  Lexer lexer1("a & b");
  lexer1.lex();
  List<Token> tokens1 = lexer1.getTokens();

  GW_ASSERT(tokens1.size() == 3);
  GW_ASSERT(tokens1.get(1).getType() == TokenType::AND_OPERATOR);
  GW_ASSERT(tokens1.get(1).getValue() == "&");

  Lexer lexer2("a | b");
  lexer2.lex();
  List<Token> tokens2 = lexer2.getTokens();

  GW_ASSERT(tokens2.size() == 3);
  GW_ASSERT(tokens2.get(1).getType() == TokenType::OR_OPERATOR);
  GW_ASSERT(tokens2.get(1).getValue() == "|");

  Lexer lexer3("!a");
  lexer3.lex();
  List<Token> tokens3 = lexer3.getTokens();

  GW_ASSERT(tokens3.size() == 2);
  GW_ASSERT(tokens3.get(0).getType() == TokenType::NOT_OPERATOR);
  GW_ASSERT(tokens3.get(0).getValue() == "!");
}

void testLexerComments() {
  Lexer lexer("# This is a comment");
  lexer.lex();
  List<Token> tokens = lexer.getTokens();

  GW_ASSERT(tokens.size() == 0);
}

void testLexerIdentifiers() {
  Lexer lexer("variable");
  lexer.lex();
  List<Token> tokens = lexer.getTokens();

  GW_ASSERT(tokens.size() == 1);
  GW_ASSERT(tokens.get(0).getType() == TokenType::IDENTIFIER);
  GW_ASSERT(tokens.get(0).getValue() == "variable");
}

void testLexerComplexExpression() {
  Lexer lexer("x = 5 + 3");
  lexer.lex();
  List<Token> tokens = lexer.getTokens();

  GW_ASSERT(tokens.size() == 5);
  GW_ASSERT(tokens.get(0).getType() == TokenType::IDENTIFIER);
  GW_ASSERT(tokens.get(0).getValue() == "x");
  GW_ASSERT(tokens.get(1).getType() == TokenType::EQUAL_OPERATOR);
  GW_ASSERT(tokens.get(1).getValue() == "=");
  GW_ASSERT(tokens.get(2).getType() == TokenType::INT);
  GW_ASSERT(tokens.get(2).getValue() == "5");
  GW_ASSERT(tokens.get(3).getType() == TokenType::MATH_OPERATOR);
  GW_ASSERT(tokens.get(3).getValue() == "+");
  GW_ASSERT(tokens.get(4).getType() == TokenType::INT);
  GW_ASSERT(tokens.get(4).getValue() == "3");
}

void testLexerMultipleLines() {
  Lexer lexer("x = 5\ny = 10");
  lexer.lex();
  List<Token> tokens = lexer.getTokens();

  GW_ASSERT(tokens.size() == 6);
  GW_ASSERT(tokens.get(0).getLine() == 1);
  GW_ASSERT(tokens.get(1).getLine() == 1);
  GW_ASSERT(tokens.get(2).getLine() == 1);
  GW_ASSERT(tokens.get(3).getLine() == 2);
  GW_ASSERT(tokens.get(4).getLine() == 2);
  GW_ASSERT(tokens.get(5).getLine() == 2);
}

void testLexerStringWithSpecialChars() {
  Lexer lexer("\"Hello + World!\"");
  lexer.lex();
  List<Token> tokens = lexer.getTokens();

  GW_ASSERT(tokens.size() == 1);
  GW_ASSERT(tokens.get(0).getType() == TokenType::STRING);
  GW_ASSERT(tokens.get(0).getValue() == "Hello + World!");
}

void testLexerParenthesesAndBrackets() {
  Lexer lexer("(1 + 2) * [3, 4]");
  lexer.lex();
  List<Token> tokens = lexer.getTokens();

  GW_ASSERT(tokens.size() == 11);
  GW_ASSERT(tokens.get(0).getType() == TokenType::OPERATOR);
  GW_ASSERT(tokens.get(0).getValue() == "(");
  GW_ASSERT(tokens.get(4).getType() == TokenType::OPERATOR);
  GW_ASSERT(tokens.get(4).getValue() == ")");
}

void testLexerEmptyString() {
  Lexer lexer("");
  lexer.lex();
  List<Token> tokens = lexer.getTokens();

  GW_ASSERT(tokens.size() == 0);
}

void testLexerWhitespaceOnly() {
  Lexer lexer("   \n  \n   ");
  lexer.lex();
  List<Token> tokens = lexer.getTokens();

  GW_ASSERT(tokens.size() == 0);
}
