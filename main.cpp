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

void runAllTests() {
  try {
    std::cout << "Running tests..." << std::endl;
    testIntList();
    testStringList();
    testToken();
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
    testExpression();
    testStatement();
    testParserExpressions();
    testParserVariables();
    testParserStatements();
    testParserPrecedence();
    testInterpreterPrint();
    testInterpreterVariables();
    std::cout << "All tests passed!" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Test failed: " << e.what() << std::endl;
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  bool testing = false;
  std::string filename = "";

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "--test") {
      testing = true;
    } else {
      filename = arg;
    }
  }

  if (testing) {
    runAllTests();
    if (filename == "")
      return 0;
  }

  if (filename != "") {
    FileReader fileReader(filename);
    std::string code = fileReader.getContent();
    if (code.empty()) {
      std::cerr << "Error: File is empty or could not be read: " << filename
                << std::endl;
      return 1;
    }

    try {
      Lexer lex(code);
      lex.lex();
      Parser parser(lex.getTokens());
      parser.parse();
      Interpreter interpreter(parser.getStatements());
      interpreter.interpret();
    } catch (const std::exception &e) {
      std::cerr << "Error: " << e.what() << std::endl;
      return 1;
    }
  } else {
    REPL repl = REPL("GW> ");
    repl.start();

    std::cout << "GW Interpreter" << std::endl;
    std::cout << "Type 'exit' or 'quit' to leave." << std::endl;
    std::cout.flush();

    while (true) {
      std::string line = repl.readLine();
      if (line == "exit" || line == "quit")
        break;
      if (line.empty())
        continue;

      try {
        Lexer lex(line);
        lex.lex();
        Parser p(lex.getTokens());
        p.parse();
        Interpreter interp(p.getStatements());
        interp.interpret();
        std::cout << std::endl;
      } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
      }
    }
  }

  return 0;
}
