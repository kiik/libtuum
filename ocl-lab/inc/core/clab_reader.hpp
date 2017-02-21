
#ifndef CLAB_READER_H
#define CLAB_READER_H

#include "core/clab_types.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  class Reader
  {
  public:
    Reader(const char*);

    std::string* getBuffer() { return &mBuffer; }

    size_t getSize();
    size_t getUnreadSize();

    int readChar(char&, size_t = 0);
    int bufferChar(char&, size_t = 0);
    int bufferChar();

    void clearBuffer();

  private:
    std::string mSource, mBuffer;
    const char* mDataPtr;
    size_t mEndOfSource;
  };

}}}

#endif
