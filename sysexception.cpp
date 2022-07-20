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

#include <cstring>
#include "sysexception.hpp" 


std::string oscpp::SysException::message(int errorNumber) {
  static const size_t MESSAGEBUFFERSIZE = 1024;
  char messageBuf[MESSAGEBUFFERSIZE + 1];
  ::memset(messageBuf, 0, MESSAGEBUFFERSIZE + 1);

  std::string errorMessage;

  #ifdef __APPLE__
  int err = ::strerror_r(errorNumber, messageBuf, MESSAGEBUFFERSIZE);
  errorMessage.assign(messageBuf);
  #else
  char* err = ::strerror_r(errorNumber, messageBuf, MESSAGEBUFFERSIZE);
  errorMessage.assign(err);
  #endif
  return errorMessage;
}



