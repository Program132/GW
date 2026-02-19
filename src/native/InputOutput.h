#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

#include "../interpreter/value/Value.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

inline Value __nativePrint(const std::vector<Value> &args) {
  if (args.size() != 1) {
    throw std::runtime_error("[GW NATIVE] print() takes 1 argument");
  }
  std::cout << args[0].asString() << std::endl;
  return Value();
}

inline Value __nativeInputString(const std::vector<Value> &args) {
  if (args.size() != 1) {
    throw std::runtime_error("[GW NATIVE] input() takes 1 argument");
  }
  std::cout << args[0].toString();
  std::string input;
  std::getline(std::cin, input);
  return Value(input);
}

inline Value __nativeInputDouble(const std::vector<Value> &args) {
  if (args.size() != 1) {
    throw std::runtime_error("[GW NATIVE] input() takes 1 argument");
  }
  std::cout << args[0].toString();
  std::string input;
  std::getline(std::cin, input);
  try {
    return Value(std::stod(input));
  } catch (const std::exception &e) {
    throw std::runtime_error("[GW NATIVE] __nativeInputDouble takes a double");
  }
}

inline Value __nativeInputInt(const std::vector<Value> &args) {
  if (args.size() != 1) {
    throw std::runtime_error("[GW NATIVE] input() takes 1 argument");
  }
  std::cout << args[0].toString();
  std::string input;
  std::getline(std::cin, input);
  try {
    return Value(std::stoi(input));
  } catch (const std::exception &e) {
    throw std::runtime_error("[GW NATIVE] __nativeInputInt takes an integer");
  }
}

inline Value __nativeFileRead(const std::vector<Value> &args) {
  if (args.size() != 1) {
    throw std::runtime_error("[GW NATIVE] readFile() takes 1 argument");
  }
  std::string path = args[0].asString();
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error("[GW NATIVE] readFile() takes a valid path");
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return Value(buffer.str());
}

inline Value __nativeFileWrite(const std::vector<Value> &args) {
  if (args.size() != 2) {
    throw std::runtime_error("[GW NATIVE] writeFile() takes 2 arguments");
  }
  std::string path = args[0].asString();
  std::string content = args[1].asString();
  std::ofstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error("[GW NATIVE] writeFile() takes a valid path");
  }
  file << content;
  return Value();
}

inline Value __nativeFileAppend(const std::vector<Value> &args) {
  if (args.size() != 2) {
    throw std::runtime_error("[GW NATIVE] appendFile() takes 2 arguments");
  }
  std::string path = args[0].asString();
  std::string content = args[1].asString();
  std::ofstream file(path, std::ios::app);
  if (!file.is_open()) {
    throw std::runtime_error("[GW NATIVE] appendFile() takes a valid path");
  }
  file << content;
  return Value();
}

inline Value __nativeReadStdin(const std::vector<Value> &args) {
  if (args.size() != 0) {
    throw std::runtime_error("[GW NATIVE] readStdin() takes 0 arguments");
  }
  std::string content;
  std::string line;
  while (std::getline(std::cin, line)) {
    content += line + "\n";
  }
  return Value(content);
}

inline Value __nativeWriteStdout(const std::vector<Value> &args) {
  if (args.size() != 1) {
    throw std::runtime_error("[GW NATIVE] writeStdout() takes 1 argument");
  }
  std::cout << args[0].asString();
  return Value();
}

#ifdef _WIN32
#include <windows.h>
#endif

inline Value __nativeClearConsole(const std::vector<Value> &args) {
  if (args.size() != 0) {
    throw std::runtime_error("[GW NATIVE] clearConsole() takes 0 arguments");
  }
#ifdef _WIN32
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD coord = {0, 0};
  DWORD count;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(hStdOut, &csbi);
  FillConsoleOutputCharacter(hStdOut, ' ', csbi.dwSize.X * csbi.dwSize.Y, coord,
                             &count);
  SetConsoleCursorPosition(hStdOut, coord);
#else
  std::cout << "\033[2J\033[1;1H";
#endif
  return Value();
}

inline Value __nativeSetCursorPosition(const std::vector<Value> &args) {
  if (args.size() != 2) {
    throw std::runtime_error(
        "[GW NATIVE] setCursorPosition() takes 2 arguments");
  }
  int x = args[0].asInt();
  int y = args[1].asInt();
#ifdef _WIN32
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD coord;
  coord.X = (SHORT)x;
  coord.Y = (SHORT)y;
  SetConsoleCursorPosition(hStdOut, coord);
#else
  std::cout << "\033[" << y << ";" << x << "H";
#endif
  return Value();
}

inline Value __nativeGetCursorPosition(const std::vector<Value> &args) {
  if (args.size() != 0) {
    throw std::runtime_error(
        "[GW NATIVE] getCursorPosition() takes 0 arguments");
  }
  int x = 0, y = 0;
#ifdef _WIN32
  HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
    x = csbi.dwCursorPosition.X;
    y = csbi.dwCursorPosition.Y;
  }
#else
#endif
  std::map<std::string, Value> members;
  members["x"] = Value(x);
  members["y"] = Value(y);
  return Value("CursorPos", &members);
}

#endif