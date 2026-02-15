#ifndef FILE_READER_H
#define FILE_READER_H

#include <fstream>
#include <string>

class FileReader {
private:
  std::string filePath;
  std::string content;

public:
  FileReader(std::string filePath) {
    this->filePath = filePath;
    std::ifstream file(filePath);
    if (file.is_open()) {
      std::string line;
      while (std::getline(file, line)) {
        content += line + "\n";
      }
      file.close();
    }
  };

  std::string getContent() { return content; }

  std::string getFilePath() { return filePath; }
};

#endif // FILE_READER_H
