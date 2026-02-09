#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

#include <iostream>
#include <string>

enum TokenType {
  WHITESPACE,
  IDENTIFIER,
  COMMENT,

  INT,
  NUMBER,
  BOOLEAN,
  POSSIBLE_STRING,
  STRING,

  OPERATOR,
  MATH_OPERATOR,

  EQUAL_OPERATOR,
  DOUBLE_EQUAL_OPERATOR,
  GREATER_OPERATOR,
  LESS_OPERATOR,
  GREATER_EQUAL_OPERATOR,
  LESS_EQUAL_OPERATOR,
  DIFFERENT_OPERATOR,

  AND_OPERATOR,
  OR_OPERATOR,
  NOT_OPERATOR,
  COMPOUND_ASSIGN_OPERATOR
};

inline std::string tokenTypeToString(TokenType type) {
  switch (type) {
  case WHITESPACE:
    return "WHITESPACE";
  case IDENTIFIER:
    return "IDENTIFIER";
  case COMMENT:
    return "COMMENT";
  case INT:
    return "INT";
  case NUMBER:
    return "NUMBER";
  case BOOLEAN:
    return "BOOLEAN";
  case POSSIBLE_STRING:
    return "POSSIBLE_STRING";
  case STRING:
    return "STRING";
  case OPERATOR:
    return "OPERATOR";
  case MATH_OPERATOR:
    return "MATH_OPERATOR";
  case EQUAL_OPERATOR:
    return "EQUAL_OPERATOR";
  case DOUBLE_EQUAL_OPERATOR:
    return "DOUBLE_EQUAL_OPERATOR";
  case GREATER_OPERATOR:
    return "GREATER_OPERATOR";
  case LESS_OPERATOR:
    return "LESS_OPERATOR";
  case GREATER_EQUAL_OPERATOR:
    return "GREATER_EQUAL_OPERATOR";
  case LESS_EQUAL_OPERATOR:
    return "LESS_EQUAL_OPERATOR";
  case DIFFERENT_OPERATOR:
    return "DIFFERENT_OPERATOR";
  case AND_OPERATOR:
    return "AND_OPERATOR";
  case OR_OPERATOR:
    return "OR_OPERATOR";
  case NOT_OPERATOR:
    return "NOT_OPERATOR";
  case COMPOUND_ASSIGN_OPERATOR:
    return "COMPOUND_ASSIGN_OPERATOR";
  default:
    return "UNKNOWN";
  }
}

#endif