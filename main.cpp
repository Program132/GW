#include "tests/TestsExpression.h"
#include "tests/TestsInterpreter.h"
#include "tests/TestsLexer.h"
#include "tests/TestsList.h"
#include "tests/TestsParser.h"
#include "tests/TestsStatement.h"
#include "tests/TestsToken.h"

#include "src/utils/FileReader.h"
#include "src/utils/REPL.h"

#include "src/interpreter/Interpreter/Interpreter.h"
#include "src/lexer/Lexer/Lexer.h"
#include "src/parser/Parser/Parser.h"

int main(int argc, char *argv[]) {
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

  /* =================== TESTS =================== */
  /* =================== Expressions ============= */
  testExpression();
  /* ============================================= */
  /* ============================================= */

  /* =================== TESTS =================== */
  /* =================== Statements ============= */
  testStatement();
  /* ============================================= */
  /* ============================================= */

  /* =================== TESTS =================== */
  /* =================== Parser ================== */
  testParserExpressions();
  testParserStatements();
  testParserPrecedence();
  /* ============================================= */
  /* ============================================= */

  /* =================== TESTS =================== */
  /* =================== Interpreter ============= */
  testInterpreterPrint();
  /* ============================================= */
  /* ============================================= */

  if (argc > 1) {
    FileReader fileReader(argv[1]);
    Lexer lex = Lexer(fileReader.getContent());
    Parser parser = Parser(lex.getTokens());
    Interpreter interpreter = Interpreter(parser.getStatements());
    interpreter.interpret();
  } else {
    REPL repl = REPL("GW> ");
    repl.start();

    while (true) {
      std::string line = repl.readLine();
      if (line == "exit" || line == "quit")
        break;

      Lexer lex(line);
      lex.lex();
      Parser p(lex.getTokens());
      p.parse();
      Interpreter interp(p.getStatements());
      interp.interpret();
      std::cout << std::endl;
    }
  }

  return 0;
}