#pragma once

#include <string>

namespace data {

class ConfigReader {
 public:
  ConfigReader(const std::string& filename);

 private:
  std::string filename_;
};

}  // namespace data