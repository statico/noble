// Copyright 2011 the Noble project authors. All rights reserved.

#ifndef SRC_FILE_H_
#define SRC_FILE_H_

#include <string>

#include "noble.h"

namespace noble {

using namespace v8;
using namespace std;

class File {
 public:
  static void Initialize(Handle<Object> target);
  static Handle<String> ReadFileIntoString(const string& name);
};

} // namespace noble

#endif // SRC_FILE_H_
