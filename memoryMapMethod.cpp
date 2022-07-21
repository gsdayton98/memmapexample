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
// THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
#include <fcntl.h>
#include <stdexcept>
#include <sys/mman.h>
#include <sys/stat.h>
#include "systemexception.hpp"

// Count the number of primes in the file by memory mapping the file
unsigned int memoryMapMethod(const char* inputFilename) {

    int fd = ::open(inputFilename, O_RDONLY | O_CLOEXEC, 0);
    if (fd < 0) throw SystemException{};

    struct stat stats;  //NOLINT
    if (::fstat(fd, &stats) < 0) throw SystemException{};

    size_t len = stats.st_size;
    void* mappedArea = ::mmap(nullptr, len, PROT_READ, MAP_FILE | MAP_PRIVATE, fd, 0L);
    if (mappedArea == MAP_FAILED) throw SystemException{};
    auto* primes = static_cast<unsigned long*>(mappedArea);
    unsigned int countOfPrimes = len/sizeof(unsigned long);

    unsigned int census = 0;
    for (auto* p = primes; p != primes + countOfPrimes; ++p) {
        ++census;
    }
    if (countOfPrimes != census) throw std::runtime_error{"Number of mapped primes mismatch"};
    return countOfPrimes;
}