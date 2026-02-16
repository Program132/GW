#ifndef REPL_H
#define REPL_H

#include <iostream>
#include <string>

class REPL {
private:
  std::string prompt;
  bool running;

public:
  REPL(std::string prompt = ">>> ") {
    this->prompt = prompt;
    this->running = true;
  }

  void start() { this->running = true; }

  void stop() { this->running = false; }

  bool isRunning() { return running; }

  std::string getPrompt() { return prompt; }

  void setPrompt(std::string prompt) { this->prompt = prompt; }

  std::string readLine() {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
      return "exit";
    }
    return line;
  }

  void print(const std::string &message) { std::cout << message << std::endl; }
};

#endif // REPL_H