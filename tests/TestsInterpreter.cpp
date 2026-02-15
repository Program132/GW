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

  std::cout << lex << std::endl;

  Parser p(lex.getTokens());
  p.parse();

  std::cout << p << std::endl;

  Interpreter interpreter(p.getStatements());

  std::streambuf *old = std::cout.rdbuf();
  std::stringstream ss;
  std::cout.rdbuf(ss.rdbuf());

  interpreter.interpret();

  std::cout.rdbuf(old);

  return ss.str();
}

void testInterpreterPrint() {
  std::cout << "Running testInterpreterPrint..." << std::endl;
  {
    std::cout << 1 << std::endl;
    std::string output = captureOutput("print(\"Hello World\"); println(42);");
    GW_ASSERT(output == "Hello World42\n");
  }

  {
    std::cout << 2 << std::endl;
    std::string output = captureOutput("println(\"Test\");");
    GW_ASSERT(output == "Test\n");
  }

  {
    std::cout << 3 << std::endl;
    std::string output = captureOutput("print(123);");
    GW_ASSERT(output == "123");
  }

  {
    std::cout << 4 << std::endl;
    std::string output = captureOutput("println(456);");
    GW_ASSERT(output == "456\n");
  }

  {
    std::cout << 5 << std::endl;
    std::string output = captureOutput("print(3.14);");
    GW_ASSERT(output == "3.14");
  }

  {
    std::cout << 6 << std::endl;
    std::string output = captureOutput("println(2.71);");
    GW_ASSERT(output == "2.71\n");
  }

  {
    std::cout << 7 << std::endl;
    std::string output = captureOutput("print(true);");
    GW_ASSERT(output == "true");
  }

  {
    std::cout << 8 << std::endl;
    std::string output = captureOutput("println(false);");
    GW_ASSERT(output == "false\n");
  }

  {
    std::cout << 9 << std::endl;
    std::string output = captureOutput("println();");
    GW_ASSERT(output == "\n");
  }

  {
    std::cout << 10 << std::endl;
    std::string output = captureOutput("print(5 + 3);");
    std::cout << "Test 10 output: [" << output << "]" << std::endl;
    std::cout << "Test 10 expected: [8]" << std::endl;
    std::cout << "Test 10 output length: " << output.length() << std::endl;
    GW_ASSERT(output == "8");
  }

  {
    std::cout << 11 << std::endl;
    std::string output = captureOutput("println(10 - 4);");
    std::cout << "Test 11 output: [" << output << "]" << std::endl;
    std::cout << "Test 11 expected: [6\\n]" << std::endl;
    std::cout << "Test 11 output length: " << output.length() << std::endl;
    GW_ASSERT(output == "6\n");
  }

  {
    std::cout << 12 << std::endl;
    std::string output = captureOutput("print(6 * 7);");
    GW_ASSERT(output == "42");
  }

  {
    std::cout << 13 << std::endl;
    std::string output = captureOutput("println(20 / 4);");
    GW_ASSERT(output == "5\n");
  }

  {
    std::cout << 14 << std::endl;
    std::string output = captureOutput("print(17 % 5);");
    GW_ASSERT(output == "2");
  }

  {
    std::cout << 15 << std::endl;
    std::string output = captureOutput("println(2 ^ 3);");
    GW_ASSERT(output == "8\n");
  }

  {
    std::cout << 16 << std::endl;
    std::string output = captureOutput("print((5 + 3) * 2);");
    GW_ASSERT(output == "16");
  }

  {
    std::cout << 17 << std::endl;
    std::string output = captureOutput("println(10 / (2 + 3));");
    GW_ASSERT(output == "2\n");
  }

  {
    std::cout << 18 << std::endl;
    std::string output = captureOutput("print(-5);");
    GW_ASSERT(output == "-5");
  }

  {
    std::cout << 19 << std::endl;
    std::string output = captureOutput("println(+42);");
    GW_ASSERT(output == "42\n");
  }

  {
    std::cout << 20 << std::endl;
    std::string output = captureOutput("print(-(3 + 2));");
    GW_ASSERT(output == "-5");
  }

  {
    std::cout << 21 << std::endl;
    std::string output = captureOutput("println(5 == 5);");
    GW_ASSERT(output == "true\n");
  }

  {
    std::cout << 22 << std::endl;
    std::string output = captureOutput("print(3 == 5);");
    GW_ASSERT(output == "false");
  }

  {
    std::cout << 23 << std::endl;
    std::string output = captureOutput("println(3 ~= 5);");
    GW_ASSERT(output == "true\n");
  }

  {
    std::cout << 24 << std::endl;
    std::string output = captureOutput("print(5 ~= 5);");
    GW_ASSERT(output == "false");
  }

  {
    std::cout << 25 << std::endl;
    std::string output = captureOutput("println(3 < 5);");
    GW_ASSERT(output == "true\n");
  }

  {
    std::cout << 26 << std::endl;
    std::string output = captureOutput("print(5 < 3);");
    GW_ASSERT(output == "false");
  }

  {
    std::cout << 27 << std::endl;
    std::string output = captureOutput("println(7 > 4);");
    GW_ASSERT(output == "true\n");
  }

  {
    std::cout << 28 << std::endl;
    std::string output = captureOutput("print(4 > 7);");
    GW_ASSERT(output == "false");
  }

  {
    std::cout << 29 << std::endl;
    std::string output = captureOutput("println(5 <= 5);");
    GW_ASSERT(output == "true\n");
  }

  {
    std::cout << 30 << std::endl;
    std::string output = captureOutput("print(6 <= 5);");
    GW_ASSERT(output == "false");
  }

  {
    std::cout << 31 << std::endl;
    std::string output = captureOutput("println(5 >= 5);");
    GW_ASSERT(output == "true\n");
  }

  {
    std::cout << 32 << std::endl;
    std::string output = captureOutput("print(4 >= 5);");
    GW_ASSERT(output == "false");
  }

  {
    std::cout << 33 << std::endl;
    std::string output = captureOutput("print(\"Hello\" + \"World\");");
    GW_ASSERT(output == "HelloWorld");
  }

  {
    std::cout << 34 << std::endl;
    std::string output = captureOutput("println(\"Test\" + \" \" + \"123\");");
    GW_ASSERT(output == "Test 123\n");
  }

  {
    std::cout << 35 << std::endl;
    std::string output = captureOutput("print(\"Hello\" + 42);");
    GW_ASSERT(output == "Hello42");
  }

  {
    std::cout << 36 << std::endl;
    std::string output = captureOutput("print(\"ab\" * 3);");
    GW_ASSERT(output == "ababab");
  }

  {
    std::cout << 37 << std::endl;
    std::string output = captureOutput("println(3 * \"xy\");");
    GW_ASSERT(output == "xyxyxy\n");
  }

  {
    std::cout << 38 << std::endl;
    std::string output =
        captureOutput("print(\"Result: \"); println(5 + 3 * 2);");
    GW_ASSERT(output == "Result: 11\n");
  }

  {
    std::cout << 39 << std::endl;
    std::string output =
        captureOutput("println(\"First\"); println(\"Second\"); "
                      "println(\"Third\");");
    GW_ASSERT(output == "First\nSecond\nThird\n");
  }

  {
    std::cout << 40 << std::endl;
    std::string output = captureOutput("print(1); print(2); print(3);");
    GW_ASSERT(output == "123");
  }

  {
    std::cout << 41 << std::endl;
    std::string output = captureOutput(
        "print(\"Answer: \"); print((5 + 3) * (10 - 2)); println(\"!\");");
    GW_ASSERT(output == "Answer: 64!\n");
  }

  {
    std::cout << 42 << std::endl;
    std::string output = captureOutput("println(10 / 2 / 2);");
    GW_ASSERT(output == "2.5\n");
  }

  {
    std::cout << 43 << std::endl;
    std::string output = captureOutput("print(2 + 3 * 4 - 5);");
    GW_ASSERT(output == "9");
  }

  {
    std::cout << 44 << std::endl;
    std::string output = captureOutput("println(100 - 50 + 25);");
    GW_ASSERT(output == "75\n");
  }

  {
    std::cout << 45 << std::endl;
    std::string output = captureOutput("print(\"Number: \"); println(999);");
    GW_ASSERT(output == "Number: 999\n");
  }

  {
    std::cout << 46 << std::endl;
    std::string output =
        captureOutput("println(\"a\" + \"b\" + \"c\" + \"d\");");
    GW_ASSERT(output == "abcd\n");
  }

  {
    std::cout << 47 << std::endl;
    std::string output = captureOutput("print(5 + 2);");
    GW_ASSERT(output == "7");
  }

  {
    std::cout << 48 << std::endl;
    std::string output = captureOutput("println(10 - 3);");
    GW_ASSERT(output == "7\n");
  }

  {
    std::cout << 49 << std::endl;
    std::string output = captureOutput("print(2 * 5);");
    GW_ASSERT(output == "10");
  }

  {
    std::cout << 50 << std::endl;
    std::string output = captureOutput("println(\"Test\"); print(123); "
                                       "println(); println(\"Done\");");
    GW_ASSERT(output == "Test\n123\n\nDone\n");
  }

  std::cout << "testInterpreterPrint passed!" << std::endl;
}
