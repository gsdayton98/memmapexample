// -*- mode: c++ -*-
////
// @copyright 2022 Glen S. Dayton.  Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following
// conditions:
//
//  The above copyright notice and this permission notice shall be included in all copies or substantial portions of
// the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
#ifndef FILE_HPP
#define FILE_HPP

#include <fcntl.h>
#include <unistd.h>
#include <utility>

namespace oscpp {

class File {
 public:
  File(const char* filename, int flag = O_RDONLY | O_CLOEXEC, int mode = 0)
      : fd{File::open(filename, flag, mode)}
  { }

  // Concrete class, so don't inherit from it. Destructor is intentionally not virtual.
  ~File() { (void)::close(fd);};

  std::pair<void*, size_t> map();

 private:
  int fd;

  File(const File&) = delete;
  File& operator=(const File&) = delete;

  static int open(const char* filename, int flag, int mode);
};
}
 #endif // FILE_HPP
