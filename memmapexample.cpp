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
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include "stopwatch.hpp"

auto fileReadMethod(const char*) -> unsigned int;
auto memoryMapMethod(const char*) -> unsigned int;


auto main(int argc, char* argv[]) -> int {
  int returnCode = EXIT_FAILURE;
  const char* input_file_name = argc < 2 ? "primes.dat" : argv[1];

  try {
    StopWatch stopwatch;
    std::cout << fileReadMethod(input_file_name) << " primes read in" << std::endl;
    auto fileReadTime = stopwatch.read();
    std::cout << "Read file in " << fileReadTime << " seconds." << std::endl;

    std::cout << std::endl;
    stopwatch.reset();
    std::cout << memoryMapMethod(input_file_name) << " primes scanned" << std::endl;
    auto memoryReadTime = stopwatch.read();
    std::cout << "Scanned memory map in " << memoryReadTime << " seconds" << std::endl;

    std::cout << std::endl;
    std::cout << "Memory read time is " << fileReadTime/memoryReadTime << " times faster"  << std::endl;

    returnCode = EXIT_SUCCESS;
  } catch (const std::exception& ex) {
    std::cerr << argv[0] << ": Exception: " << ex.what() << std::endl;
  }
  return returnCode;
}
