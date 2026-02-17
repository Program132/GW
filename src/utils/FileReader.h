#ifndef FILE_READER_H
#define FILE_READER_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <string>

class FileReader {
private:
  std::string filePath;
  std::string content;
  std::set<std::string> includedFiles;

  std::string getDirectory(const std::string &path) {
    size_t found = path.find_last_of("/\\");
    if (found != std::string::npos) {
      return path.substr(0, found);
    }
    return ".";
  }

  void processIncludes(std::string &currentContent,
                       const std::string &currentPath) {
    std::string includeKeyword = "import \"";
    size_t pos = 0;
    while ((pos = currentContent.find(includeKeyword, pos)) !=
           std::string::npos) {
      size_t startQuote = pos + includeKeyword.length();
      size_t endQuote = currentContent.find("\"", startQuote);
      if (endQuote != std::string::npos) {
        std::string filename =
            currentContent.substr(startQuote, endQuote - startQuote);
        std::string fullPath = getDirectory(currentPath) + "/" + filename;

        // Simple normalization (could be improved)
        std::replace(fullPath.begin(), fullPath.end(), '\\', '/');

        std::string replacement = "";
        if (includedFiles.find(fullPath) == includedFiles.end()) {
          includedFiles.insert(fullPath);
          std::ifstream file(fullPath);
          if (file.is_open()) {
            std::string line;
            std::string includedContent;
            while (std::getline(file, line)) {
              includedContent += line + "\n";
            }
            file.close();
            processIncludes(includedContent, fullPath); // Recursive processing
            replacement = includedContent;
          } else {
            std::cerr << "Warning: Could not open included file: " << fullPath
                      << std::endl;
          }
        }

        currentContent.replace(pos, endQuote - pos + 1, replacement);
        pos += replacement.length();
      } else {
        pos += includeKeyword.length();
      }
    }
  }

public:
  FileReader(std::string filePath) {
    this->filePath = filePath;
    // Normalize initial path
    std::replace(this->filePath.begin(), this->filePath.end(), '\\', '/');

    std::ifstream file(this->filePath);
    if (file.is_open()) {
      std::string line;
      while (std::getline(file, line)) {
        content += line + "\n";
      }
      file.close();

      includedFiles.insert(this->filePath);
      processIncludes(content, this->filePath);
    }
  };

  std::string getContent() { return content; }

  std::string getFilePath() { return filePath; }
};

#endif // FILE_READER_H
