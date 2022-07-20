// -*- mode: c++ -*-
////
// @copyright 2021 Glen S. Dayton.  Permission is hereby granted, free of charge, to any person obtaining a copy of
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
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <utility>
#include "file.hpp"
#include "sysexception.hpp"


std::pair<void*, size_t> oscpp::File::map() {
  struct stat stats;

  if (::fstat(fd, &stats) < 0) throw oscpp::SysException{};

  size_t len = stats.st_size;
  void* mappedArea = ::mmap(0, len, PROT_READ, MAP_FILE | MAP_PRIVATE, fd, 0L);
  if (mappedArea == MAP_FAILED) throw oscpp::SysException{};

  return std::make_pair(mappedArea, len);
}

