
#include "tuum_fs.hpp"

#include "core/clab_reader.hpp"

namespace tuum {
namespace ocl {
namespace lab {

  Reader::Reader(const char* path)
  {
    File* script = tuum::NewFile(path);
    script->read(mSource);
    delete(script);

    mBuffer = "";
    mDataPtr = mSource.c_str();
    mEndOfSource = mSource.size();
    mLineNumber = 1;

    RTXLOG(format("Loaded '%s'.", path));
  }

  size_t Reader::getSize() {
    return mSource.size();
  }

  size_t Reader::getUnreadSize() {
    return mSource.size() - (mDataPtr - mSource.c_str());
  }

  size_t val = 0, nval = 1;

  int Reader::readChar(char& out, size_t offset) {
    if(getUnreadSize() <= 0) return -1;

    out = *(char*)((size_t)mDataPtr + offset);
    mDataPtr++;

    if(out == '\n') mLineNumber++;

    return 1;
  }


  int Reader::bufferChar(char& out, size_t offset) {
    if(readChar(out, offset) < 0) return -1;

    mBuffer += out;

    return 1;
  }

  int Reader::bufferChar() {
    char c;
    bufferChar(c);

    return 1;
  }

  void Reader::clearBuffer() {
    mBuffer = "";
  }


}}}
