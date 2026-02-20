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

#include "src/native/InputOutput.h"
#include "src/native/Network.h"
#include "src/native/String.h"
#include "src/native/System.h"
#include "src/native/Time.h"

void registerNatives(Interpreter &interpreter) {
  // Time.h
  interpreter.addNativeFunction("__native_time", __nativeTime);
  interpreter.addNativeFunction("__native_time_double", __nativeTimeDouble);
  interpreter.addNativeFunction("__native_sleep", __nativeSleep);
  interpreter.addNativeFunction("__native_sleep_double", __nativeSleepDouble);
  interpreter.addNativeFunction("__native_ctime", __nativeCTime);

  // InputOutput.h
  interpreter.addNativeFunction("__native_print", __nativePrint);
  interpreter.addNativeFunction("__native_input_string", __nativeInputString);
  interpreter.addNativeFunction("__native_input_int", __nativeInputInt);
  interpreter.addNativeFunction("__native_input_double", __nativeInputDouble);

  interpreter.addNativeFunction("__native_file_read", __nativeFileRead);
  interpreter.addNativeFunction("__native_file_write", __nativeFileWrite);
  interpreter.addNativeFunction("__native_file_append", __nativeFileAppend);

  interpreter.addNativeFunction("__native_read_stdin", __nativeReadStdin);
  interpreter.addNativeFunction("__native_write_stdout", __nativeWriteStdout);

  interpreter.addNativeFunction("__native_clear_console", __nativeClearConsole);
  interpreter.addNativeFunction("__native_set_cursor",
                                __nativeSetCursorPosition);
  interpreter.addNativeFunction("__native_get_cursor",
                                __nativeGetCursorPosition);

  // System.h
  interpreter.addNativeFunction("__native_exec_cmd", __nativeExecCmd);
  interpreter.addNativeFunction("__native_is_windows", __nativeIsWindows);
  interpreter.addNativeFunction("__native_is_linux", __nativeIsLinux);
  interpreter.addNativeFunction("__native_is_mac", __nativeIsMac);
  interpreter.addNativeFunction("__native_exit", __nativeExit);
  interpreter.addNativeFunction("__native_set_env", __nativeSetEnv);

  // Network.h
  interpreter.addNativeFunction("__native_socket_create", __nativeSocketCreate);
  interpreter.addNativeFunction("__native_socket_close", __nativeSocketClose);
  interpreter.addNativeFunction("__native_socket_connect",
                                __nativeSocketConnect);
  interpreter.addNativeFunction("__native_socket_send", __nativeSocketSend);
  interpreter.addNativeFunction("__native_socket_receive",
                                __nativeSocketReceive);
  interpreter.addNativeFunction("__native_socket_bind", __nativeSocketBind);
  interpreter.addNativeFunction("__native_socket_listen", __nativeSocketListen);
  interpreter.addNativeFunction("__native_socket_accept", __nativeSocketAccept);

  // System Args
  interpreter.addNativeFunction("__native_get_arg_count", __nativeGetArgCount);
  interpreter.addNativeFunction("__native_get_arg", __nativeGetArg);

  // String.h
  interpreter.addNativeFunction("__native_string_length", __nativeStringLength);
  interpreter.addNativeFunction("__native_string_at", __nativeStringAt);
}

void runAllTests() {
  try {
    std::cout << "Running tests..." << std::endl;
    std::cout << "testIntList" << std::endl;
    testIntList();
    std::cout << "testStringList" << std::endl;
    testStringList();
    std::cout << "testToken" << std::endl;
    testToken();
    std::cout << "testLexerIntegers" << std::endl;
    testLexerIntegers();
    std::cout << "testLexerNumbers" << std::endl;
    testLexerNumbers();
    std::cout << "testLexerBooleans" << std::endl;
    testLexerBooleans();
    std::cout << "testLexerStrings" << std::endl;
    testLexerStrings();
    std::cout << "testLexerMathOperators" << std::endl;
    testLexerMathOperators();
    std::cout << "testLexerComparisonOperators" << std::endl;
    testLexerComparisonOperators();
    std::cout << "testLexerBooleanOperators" << std::endl;
    testLexerBooleanOperators();
    std::cout << "testLexerComments" << std::endl;
    testLexerComments();
    std::cout << "testLexerIdentifiers" << std::endl;
    testLexerIdentifiers();
    std::cout << "testLexerComplexExpression" << std::endl;
    testLexerComplexExpression();
    std::cout << "testLexerMultipleLines" << std::endl;
    testLexerMultipleLines();
    std::cout << "testLexerStringWithSpecialChars" << std::endl;
    testLexerStringWithSpecialChars();
    std::cout << "testLexerParenthesesAndBrackets" << std::endl;
    testLexerParenthesesAndBrackets();
    std::cout << "testLexerEmptyString" << std::endl;
    testLexerEmptyString();
    std::cout << "testLexerWhitespaceOnly" << std::endl;
    testLexerWhitespaceOnly();
    std::cout << "testExpression" << std::endl;
    testExpression();
    std::cout << "testStatement" << std::endl;
    testStatement();
    std::cout << "testParserExpressions" << std::endl;
    testParserExpressions();
    std::cout << "testParserVariables" << std::endl;
    testParserVariables();
    std::cout << "testParserStatements" << std::endl;
    testParserStatements();
    std::cout << "testParserPrecedence" << std::endl;
    testParserPrecedence();
    std::cout << "testInterpreterPrint" << std::endl;
    testInterpreterPrint();
    std::cout << "testInterpreterVariables" << std::endl;
    testInterpreterVariables();
    std::cout << "testInterpreterFunctions" << std::endl;
    testInterpreterFunctions();
    std::cout << "testInterpreterLoops" << std::endl;
    testInterpreterLoops();
    std::cout << "All tests passed!" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Test failed: " << e.what() << std::endl;
    exit(1);
  }
}

int main(int argc, char *argv[]) {
  bool testing = false;
  bool help = false;
  std::string filename = "";
  int scriptArgsIndex = argc; // Default: no args

  for (int i = 1; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "--test" || arg == "--tests" || arg == "-t") {
      testing = true;
    } else if (arg == "--help" || arg == "-h") {
      help = true;
    } else {
      filename = arg;
      scriptArgsIndex = i + 1; // All args AFTER filename are for the script
      break; // Stop parsing options for GW, rest is for script
    }
  }

  // Pass args to System module
  GW_setArgs(argc, argv, scriptArgsIndex);

  if (testing) {
    runAllTests();
    if (filename == "")
      return 0;
  }
  if (help) {
    std::cout << "GW Informations: \n"
              << "- GitHub: https://github.com/Program132/GW/ \n"
              << "- Version: 1.1.0\n"
              << "How to use: \n"
              << "- ./GW <filename> : Run the program and interpret the code "
                 "from the file\n"
              << "- ./GW --test : Run all tests\n"
              << "- ./GW --help : Show this message\n"
              << "- ./GW : Start the REPL" << std::endl;

    if (filename == "")
      return 0;
  }

  if (filename != "") {
    FileReader fileReader(filename);
    std::string code = fileReader.getContent();
    if (code.empty() && !testing) {
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

      registerNatives(interpreter);

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

        registerNatives(interp);

        interp.interpret();
        std::cout << std::endl;
      } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
      }
    }
  }

  return 0;
}
