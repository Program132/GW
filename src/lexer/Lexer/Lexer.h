#ifndef LEXER_H
#define LEXER_H

#include "../../utils/List.h"
#include "../Token/Token.h"
#include "../Token/TokenType.h"
#include <iostream>
#include <string>

class Lexer {
private:
  std::string code;
  List<Token> tokens;
  Token current;

public:
  Lexer(std::string code);

  List<Token> getTokens();
  void appendToken();
  void lex();
  bool isDigit(char c);
  bool isAlpha(char c);
};

#endif