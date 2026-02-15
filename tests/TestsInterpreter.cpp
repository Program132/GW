#include "TestsInterpreter.h"
#include "../src/interpreter/Interpreter/Interpreter.h"
#include "../src/lexer/Lexer/Lexer.h"
#include "../src/parser/Parser/Parser.h"
#include "../src/utils/Assert.h"
#include <iostream>
#include <sstream>

std::string captureOutput(const std::string &code) {
  Lexer lex(code);
  lex.lex();

  Parser p(lex.getTokens());
  p.parse();

  Interpreter interpreter(p.getStatements());

  std::streambuf *old = std::cout.rdbuf();
  std::stringstream ss;
  std::cout.rdbuf(ss.rdbuf());

  interpreter.interpret();

  std::cout.rdbuf(old);

  return ss.str();
}

void testInterpreterPrint() {
  {
    std::string output = captureOutput("print(\"Hello World\"); println(42);");
    GW_ASSERT(output == "Hello World42\n");
  }

  {
    std::string output = captureOutput("println(\"Test\");");
    GW_ASSERT(output == "Test\n");
  }

  {
    std::string output = captureOutput("print(123);");
    GW_ASSERT(output == "123");
  }

  {
    std::string output = captureOutput("println(456);");
    GW_ASSERT(output == "456\n");
  }

  {
    std::string output = captureOutput("print(3.14);");
    GW_ASSERT(output == "3.14");
  }

  {
    std::string output = captureOutput("println(2.71);");
    GW_ASSERT(output == "2.71\n");
  }

  {
    std::string output = captureOutput("print(true);");
    GW_ASSERT(output == "true");
  }

  {
    std::string output = captureOutput("println(false);");
    GW_ASSERT(output == "false\n");
  }

  {
    std::string output = captureOutput("println();");
    GW_ASSERT(output == "\n");
  }

  {
    std::string output = captureOutput("print(5 + 3);");
    GW_ASSERT(output == "8");
  }

  {
    std::string output = captureOutput("println(10 - 4);");
    GW_ASSERT(output == "6\n");
  }

  {
    std::string output = captureOutput("print(6 * 7);");
    GW_ASSERT(output == "42");
  }

  {
    std::string output = captureOutput("println(20 / 4);");
    GW_ASSERT(output == "5\n");
  }

  {
    std::string output = captureOutput("print(17 % 5);");
    GW_ASSERT(output == "2");
  }

  {
    std::string output = captureOutput("println(2 ^ 3);");
    GW_ASSERT(output == "8\n");
  }

  {
    std::string output = captureOutput("print((5 + 3) * 2);");
    GW_ASSERT(output == "16");
  }

  {
    std::string output = captureOutput("println(10 / (2 + 3));");
    GW_ASSERT(output == "2\n");
  }

  {
    std::string output = captureOutput("print(-5);");
    GW_ASSERT(output == "-5");
  }

  {
    std::string output = captureOutput("println(+42);");
    GW_ASSERT(output == "42\n");
  }

  {
    std::string output = captureOutput("print(-(3 + 2));");
    GW_ASSERT(output == "-5");
  }

  {
    std::string output = captureOutput("println(5 == 5);");
    GW_ASSERT(output == "true\n");
  }

  {
    std::string output = captureOutput("print(3 == 5);");
    GW_ASSERT(output == "false");
  }

  {
    std::string output = captureOutput("println(3 ~= 5);");
    GW_ASSERT(output == "true\n");
  }

  {
    std::string output = captureOutput("print(5 ~= 5);");
    GW_ASSERT(output == "false");
  }

  {
    std::string output = captureOutput("println(3 < 5);");
    GW_ASSERT(output == "true\n");
  }

  {
    std::string output = captureOutput("print(5 < 3);");
    GW_ASSERT(output == "false");
  }

  {
    std::string output = captureOutput("println(7 > 4);");
    GW_ASSERT(output == "true\n");
  }

  {
    std::string output = captureOutput("print(4 > 7);");
    GW_ASSERT(output == "false");
  }

  {
    std::string output = captureOutput("println(5 <= 5);");
    GW_ASSERT(output == "true\n");
  }

  {
    std::string output = captureOutput("print(6 <= 5);");
    GW_ASSERT(output == "false");
  }

  {
    std::string output = captureOutput("println(5 >= 5);");
    GW_ASSERT(output == "true\n");
  }

  {
    std::string output = captureOutput("print(4 >= 5);");
    GW_ASSERT(output == "false");
  }

  {
    std::string output = captureOutput("print(\"Hello\" + \"World\");");
    GW_ASSERT(output == "HelloWorld");
  }

  {
    std::string output = captureOutput("println(\"Test\" + \" \" + \"123\");");
    GW_ASSERT(output == "Test 123\n");
  }

  {
    std::string output = captureOutput("print(\"Hello\" + 42);");
    GW_ASSERT(output == "Hello42");
  }

  {
    std::string output = captureOutput("print(\"ab\" * 3);");
    GW_ASSERT(output == "ababab");
  }

  {
    std::string output = captureOutput("println(3 * \"xy\");");
    GW_ASSERT(output == "xyxyxy\n");
  }

  {
    std::string output =
        captureOutput("print(\"Result: \"); println(5 + 3 * 2);");
    GW_ASSERT(output == "Result: 11\n");
  }

  {
    std::string output =
        captureOutput("println(\"First\"); println(\"Second\"); "
                      "println(\"Third\");");
    GW_ASSERT(output == "First\nSecond\nThird\n");
  }

  {
    std::string output = captureOutput("print(1); print(2); print(3);");
    GW_ASSERT(output == "123");
  }

  {
    std::string output = captureOutput(
        "print(\"Answer: \"); print((5 + 3) * (10 - 2)); println(\"!\");");
    GW_ASSERT(output == "Answer: 64!\n");
  }

  {
    std::string output = captureOutput("println(10 / 2 / 2);");
    GW_ASSERT(output == "2.5\n");
  }

  {
    std::string output = captureOutput("print(2 + 3 * 4 - 5);");
    GW_ASSERT(output == "9");
  }

  {
    std::string output = captureOutput("println(100 - 50 + 25);");
    GW_ASSERT(output == "75\n");
  }

  {
    std::string output = captureOutput("print(\"Number: \"); println(999);");
    GW_ASSERT(output == "Number: 999\n");
  }

  {
    std::string output =
        captureOutput("println(\"a\" + \"b\" + \"c\" + \"d\");");
    GW_ASSERT(output == "abcd\n");
  }

  {
    std::string output = captureOutput("print(5 + 2);");
    GW_ASSERT(output == "7");
  }

  {
    std::string output = captureOutput("println(10 - 3);");
    GW_ASSERT(output == "7\n");
  }

  {
    std::string output = captureOutput("print(2 * 5);");
    GW_ASSERT(output == "10");
  }

  {
    std::string output = captureOutput("println(\"Test\"); print(123); "
                                       "println(); println(\"Done\");");
    GW_ASSERT(output == "Test\n123\nDone\n");
  }
}
