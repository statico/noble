// Copyright 2011 the Noble project authors. All rights reserved.

#ifndef NOBLE_GLOBAL_H_
#define NOBLE_GLOBAL_H_

#include "noble.h"

namespace noble {

using namespace v8;

class Global {
 public:
  static void Initialize(Handle<Object> target);
};

} // namespace noble

#endif // NOBLE_GLOBAL_H_
