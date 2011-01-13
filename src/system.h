// Copyright 2011 the Noble project authors. All rights reserved.

#ifndef SRC_SYSTEM_H_
#define SRC_SYSTEM_H_

#include "noble.h"

namespace noble {
namespace system {

using namespace v8;
using namespace std;

Handle<Value> Initialize(int argc, char** argv, char** envp);

} } // namespace noble::system

#endif // SRC_SYSTEM_H_
