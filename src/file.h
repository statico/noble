// Copyright 2011 the Noble project authors. All rights reserved.

#ifndef SRC_FILE_H_
#define SRC_FILE_H_

#include <string>

#include "noble.h"

namespace noble {
namespace file {

using namespace v8;
using namespace std;

void Initialize(Handle<Object> target);
Handle<String> ReadFileIntoString(const string& name);

} } // namespace noble::file

#endif // SRC_FILE_H_
