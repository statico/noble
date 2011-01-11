// Copyright 2011 the Noble project authors. All rights reserved.

#ifndef SRC_FS_H_
#define SRC_FS_H_

#include <string>

#include "noble.h"

namespace noble {
namespace fs {

using namespace v8;
using namespace std;

Handle<Value> Read(const Arguments& args);
Handle<String> ReadFileIntoString(const string& name);
void Initialize(Handle<Object> target);

} } // namespace noble::fs

#endif // SRC_FS_H_
