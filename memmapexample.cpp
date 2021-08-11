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
//
//  @return Completion code. Non-zero on errors.
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <utility>


#include <chrono>
class SysException;
unsigned int  fileReadMethod(const char*);
unsigned int  memmapMethod(const char*);


using namespace std::chrono;
class StopWatch {
  public:
  StopWatch(): start{steady_clock::now()}
  { }

  void reset() { start = steady_clock::now(); }

  double read() const;

 private:
  steady_clock::time_point start;
};



#include <cerrno>
class SysException : public std::runtime_error {
 public:
  SysException(int errorNumber = errno) : std::runtime_error(SysException::message(errorNumber)) { }

  static std::string message(int errorNumber);
};




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


#include <unistd.h>
#include <fcntl.h>
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



int File::open(const char* filename, int flags, int mode) {
  int fd = ::open(filename, flags, mode);
  if (fd < 0) throw SysException{};
  return fd;
}


#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
std::pair<void*, size_t> File::map() {
  struct stat stats;

  if (::fstat(fd, &stats) < 0) throw SysException{};

  size_t len = stats.st_size;
  void* mappedArea = ::mmap(0, len, PROT_READ, MAP_FILE | MAP_PRIVATE, fd, 0L);
  if (mappedArea == MAP_FAILED) throw SysException{};

  return std::make_pair(mappedArea, len);
}


double StopWatch::read() const {
  steady_clock::time_point stopwatch_stop = steady_clock::now();
  steady_clock::duration time_span = stopwatch_stop - start;
  return duration_cast<duration<double> >(time_span).count();
}


std::string SysException::message(int errorNumber) {
  static const size_t MESSAGEBUFFERSIZE = 1024;
  char messageBuf[MESSAGEBUFFERSIZE+1];
  ::memset(messageBuf, 0, MESSAGEBUFFERSIZE+1);

  std::string errorMessage;
  
  # ifdef __APPLE__
    int err = ::strerror_r(errorNumber, messageBuf, MESSAGEBUFFERSIZE);
    errorMessage.assign(messageBuf);
  # else
    char *err = ::strerror_r(errorNumber, messageBuf, MESSAGEBUFFERSIZE);
    errorMessage.assign(err);
  #endif
  return errorMessage;
}

