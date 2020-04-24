#pragma once

#include <ios>
#include <istream>
#include <fstream>
#include <functional>
#include <string>

class FileUtil {
public:
   virtual std::streamsize size(const std::string& name) {
      std::ifstream stream{name, std::ios::in | std::ios::binary};
      stream.seekg(0, std::ios::end);
      return stream.tellg();
   }

   std::streamsize execute(
         const std::string& name,
         std::function<std::streamsize (std::istream&)> func) {
      std::ifstream stream{name, std::ios::in | std::ios::binary};
      return func(stream);
   }
};
