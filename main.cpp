#include "tests/TestsLexer.h"
#include "tests/TestsList.h"
#include "tests/TestsToken.h"

int main() {
  /* =================== TESTS =================== */
  /* =================== Lists =================== */
  testIntList();
  testStringList();
  /* ============================================= */
  /* ============================================= */

  /* =================== TESTS =================== */
  /* =================== Token =================== */
  testToken();
  /* ============================================= */
  /* ============================================= */

  /* =================== TESTS =================== */
  /* =================== Lexer =================== */
  testLexerIntegers();
  testLexerNumbers();
  testLexerBooleans();
  testLexerStrings();
  testLexerMathOperators();
  testLexerComparisonOperators();
  testLexerBooleanOperators();
  testLexerComments();
  testLexerIdentifiers();
  testLexerComplexExpression();
  testLexerMultipleLines();
  testLexerStringWithSpecialChars();
  testLexerParenthesesAndBrackets();
  testLexerEmptyString();
  testLexerWhitespaceOnly();
  /* ============================================= */
  /* ============================================= */

  return 0;
}