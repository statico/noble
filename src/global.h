// Copyright 2011 the Noble project authors. All rights reserved.

#ifndef NOBLE_GLOBAL_H_
#define NOBLE_GLOBAL_H_

#include "noble.h"

namespace noble {
namespace global {

using namespace v8;

void Initialize(Handle<Object> target);

} } // namespace noble::global

#endif // NOBLE_GLOBAL_H_
