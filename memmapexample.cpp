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
//
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <utility>
#include "oscpp.hpp"

using oscpp::File;
using oscpp::StopWatch;
using oscpp::SysException;


unsigned int  fileReadMethod(const char*); 
unsigned int memmapMethod(const char*); 


int main(int argc, char* argv[]) {
  int returnCode = EXIT_FAILURE;
  const char* gozintaname = argc < 2 ? "primes.dat" : argv[1];

  try {
    StopWatch stopwatch;
    std::cout << fileReadMethod(gozintaname) << " primes read in" << std::endl;
    std::cout << "Read file in " << stopwatch.read() << " seconds." << std::endl;

    std::cout << std::endl;
    stopwatch.reset();
    std::cout <<  memmapMethod(gozintaname) << " primes scanned" << std::endl;
    std::cout << "Scanned memory map in " << stopwatch.read() << " seconds" << std::endl;

    returnCode = EXIT_SUCCESS;
  } catch (const std::exception& ex) {
    std::cerr << argv[0] << ": Exception: " << ex.what() << std::endl;
  }
  return returnCode;
}


// Count the number of primes in the file
#include <fstream>
unsigned int  fileReadMethod(const char* infilename) {
  unsigned int census = 0;

  unsigned long prime;
  std::ifstream primesInput(infilename, std::ios::binary);
  while (primesInput.read(reinterpret_cast<char*>(&prime), sizeof(prime))) {
    ++census;
  }

  return census;
}


// Count the numbert of primes in the file by memory mapping the file
 unsigned int  memmapMethod(const char* infilename) {
  File infile{infilename};
  auto mappedArea = infile.map();

  unsigned long* primes = static_cast<unsigned long*>(mappedArea.first);
  unsigned int nprimes = mappedArea.second / sizeof(unsigned long);

  unsigned int census = 0;
  for (auto* p = primes; p != primes + nprimes; ++p) {
    ++census;
  }
  if (nprimes != census) throw std::runtime_error{"Number of mapped primes mismatch"};
  return nprimes;
}
