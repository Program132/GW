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

    // Comment content processing
    if (this->current.getType() == TokenType::COMMENT && c != '\n') {
      this->current.appendCharacter(c);
      i++;
      continue;
    }

    // Integer / Number Management :
    if (this->isDigit(c)) {
      if (this->current.getType() == TokenType::WHITESPACE) {
        this->current.setType(TokenType::INT);
        this->current.setLine(line);
        this->current.appendCharacter(c);
      } else {
        this->current.appendCharacter(c);
      }
    } else if (c == '.') {
      if (this->current.getType() == TokenType::INT) {
        this->current.setType(TokenType::NUMBER);
        this->current.appendCharacter(c);
      } else if (this->current.getType() == TokenType::POSSIBLE_STRING) {
        this->current.appendCharacter(c);
      } else {
        this->appendToken();
        this->current = Token(TokenType::OPERATOR, std::string(1, c), line);
        this->appendToken();
      }
    }

    // New line Management :
    else if (c == '\n') {
      line = line + 1;
      this->appendToken();
      this->current.addLine();
    }

    else if (c == ' ' || c == '\t') {
      if (this->current.getType() == TokenType::POSSIBLE_STRING) {
        this->current.appendCharacter(c);
      } else {
        this->appendToken();
        this->current = Token();
      }
    }

    // Comment Management :
    else if (c == '#') {
      if (this->current.getType() != TokenType::POSSIBLE_STRING) {
        this->appendToken();
        this->current = Token(TokenType::COMMENT, std::string(1, c), line);
      } else {
        this->current.appendCharacter(c);
      }
    }

    // String Management :
    else if (c == '"') {
      if (this->current.getType() == TokenType::WHITESPACE) {
        this->current.setType(TokenType::POSSIBLE_STRING);
        this->current.setLine(line);
      } else if (this->current.getType() == TokenType::POSSIBLE_STRING) {
        this->current.setType(TokenType::STRING);
        this->appendToken();
      }
    }

    // Math operators Management : +, -, *, /, %, ^
    else if (this->isMathOperator(c)) {
      if (this->current.getType() == TokenType::POSSIBLE_STRING) {
        this->current.appendCharacter(c);
      } else {
        this->appendToken();
        if (i + 1 < this->code.length() && this->code[i + 1] == '=') {
          this->current = Token(TokenType::COMPOUND_ASSIGN_OPERATOR,
                                std::string(1, c), line);
          this->current.appendCharacter(this->code[i + 1]);
          i += 1;
        } else {
          this->current =
              Token(TokenType::MATH_OPERATOR, std::string(1, c), line);
        }
        this->appendToken();
      }
    }

    // Other operators Management : , ( ) { } [ ] : ; ?
    else if (this->isOperator(c)) {
      if (this->current.getType() == TokenType::POSSIBLE_STRING) {
        this->current.appendCharacter(c);
      } else {
        this->appendToken();
        this->current = Token(TokenType::OPERATOR, std::string(1, c), line);
        this->appendToken();
      }
    }

    // Boolean operators Management : & (and), | (or), ! (not)
    else if (c == '&' || c == '|' || c == '!') {
      if (this->current.getType() == TokenType::POSSIBLE_STRING) {
        this->current.appendCharacter(c);
      } else {
        this->appendToken();
        if (c == '&') {
          this->current =
              Token(TokenType::AND_OPERATOR, std::string(1, c), line);
        } else if (c == '|') {
          this->current =
              Token(TokenType::OR_OPERATOR, std::string(1, c), line);
        } else if (c == '!') {
          this->current =
              Token(TokenType::NOT_OPERATOR, std::string(1, c), line);
        }
        this->appendToken();
      }
    }

    // Conditional operators Managamenet : <, >, ==, >=, <=, ~=
    else if (c == '<') {
      if (this->current.getType() != TokenType::POSSIBLE_STRING) {
        this->appendToken();
        this->current =
            Token(TokenType::LESS_OPERATOR, std::string(1, c), line);
      } else {
        this->current.appendCharacter(c);
      }
    } else if (c == '>') {
      if (this->current.getType() != TokenType::POSSIBLE_STRING) {
        this->appendToken();
        this->current =
            Token(TokenType::GREATER_OPERATOR, std::string(1, c), line);
      } else {
        this->current.appendCharacter(c);
      }
    } else if (c == '~') {
      if (this->current.getType() != TokenType::POSSIBLE_STRING) {
        this->appendToken();
        this->current = Token(TokenType::OPERATOR, std::string(1, c), line);
      } else {
        this->current.appendCharacter(c);
      }
    } else if (c == '=') {
      if (this->current.getType() != TokenType::POSSIBLE_STRING) {

        // ==
        if (this->current.getType() == TokenType::EQUAL_OPERATOR) {
          this->current.setType(TokenType::DOUBLE_EQUAL_OPERATOR);
          this->current.appendCharacter(c);
          this->appendToken();
        }
        // <=
        else if (this->current.getType() == TokenType::LESS_OPERATOR) {
          this->current.setType(TokenType::LESS_EQUAL_OPERATOR);
          this->current.appendCharacter(c);
          this->appendToken();
        }
        // >=
        else if (this->current.getType() == TokenType::GREATER_OPERATOR) {
          this->current.setType(TokenType::GREATER_EQUAL_OPERATOR);
          this->current.appendCharacter(c);
          this->appendToken();
        }
        // ~=
        else if (this->current.getType() == TokenType::GREATER_OPERATOR) {
          this->current.setType(TokenType::DIFFERENT_OPERATOR);
          this->current.appendCharacter(c);
          this->appendToken();
        }
        // =
        else {
          this->appendToken();
          this->current =
              Token(TokenType::EQUAL_OPERATOR, std::string(1, c), line);
        }
      } else {
        if (this->current.getType() != TokenType::POSSIBLE_STRING) {
          this->current.setType(TokenType::IDENTIFIER);
          this->current.setLine(line);
        }
        this->current.appendCharacter(c);
      }
    } else {
      if (this->current.getType() == TokenType::WHITESPACE) {
        this->current.setType(TokenType::IDENTIFIER);
        this->current.setLine(line);
      }
      this->current.appendCharacter(c);
    }

    i = i + 1;
  }

  // Append the last token
  this->appendToken();
}

bool Lexer::isDigit(char c) { return c >= '0' && c <= '9'; }

bool Lexer::isAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::isMathOperator(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

bool Lexer::isOperator(char c) {
  return c == ',' || c == '(' || c == ')' || c == '{' || c == '}' || c == '[' ||
         c == ']' || c == ':' || c == ';' || c == '?';
}