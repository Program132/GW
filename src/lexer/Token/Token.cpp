#include "Token.h"
#include "TokenType.h"

Token::Token(TokenType type, std::string value, int line) {
  this->type = type;
  this->value = value;
  this->line = line;
}

void Token::addLine() { this->line++; }

void Token::appendCharacter(char c) { this->value += c; }

void Token::appendString(std::string str) { this->value += str; }

void Token::setType(TokenType type) { this->type = type; }

void Token::setValue(std::string value) { this->value = value; }

void Token::setLine(int line) { this->line = line; }

TokenType Token::getType() const { return this->type; }

std::string Token::getValue() const { return this->value; }

int Token::getLine() const { return this->line; }

std::string Token::print() {
  std::string str = "TOKEN{" + tokenTypeToString(this->type) + ", " +
                    this->value + ", " + std::to_string(this->line) + "}";
  return str;
}