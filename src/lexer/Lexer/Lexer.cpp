#include "Lexer.h"

Lexer::Lexer(std::string code) { this->code = code; }

List<Token> Lexer::getTokens() { return this->tokens; }

void Lexer::appendToken() {
  TokenType type = this->current.getType();
  std::string value = this->current.getValue();

  if (type == TokenType::IDENTIFIER && (value == "true" || value == "false")) {
    this->current.setType(TokenType::BOOLEAN);
    this->tokens.append(this->current);
    this->current = Token();
    return;
  }

  if (type == TokenType::WHITESPACE || type == TokenType::COMMENT) {
    this->current = Token();
    return;
  }

  if (type == TokenType::STRING) {
    this->tokens.append(this->current);
    this->current = Token();
    return;
  }

  if (value != "" && !value.empty()) {
    this->tokens.append(this->current);
  }

  this->current = Token();
}

void Lexer::lex() {
  int line = 1;
  this->current = Token();
  int i = 0;
  while (i != this->code.length()) {
    char c = this->code[i];

    if (this->isDigit(c)) {
    }
    i = i + 1;
  }
}

bool Lexer::isDigit(char c) { return c >= '0' && c <= '9'; }

bool Lexer::isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
