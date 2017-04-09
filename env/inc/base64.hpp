
#include <string>
#include <stdint.h>

namespace b64 {

  std::string encode(const char*, const int len);
  std::string decode(const std::string& s);

}
