#ifndef TOKEN_H
#define TOKEN_H

#include "TokenType.h"
#include <iostream>
#include <string>

class Token {
private:
  TokenType type;
  std::string value;
  int line;

public:
  Token() = default;
  Token(TokenType type, std::string value, int line);

  void addLine();
  void appendCharacter(char c);
  void appendString(std::string str);

  void setType(TokenType type);
  void setValue(std::string value);
  void setLine(int line);

  TokenType getType() const;
  std::string getValue() const;
  int getLine() const;

  std::string print();
};

#endif