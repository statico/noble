// Copyright 2011 the Noble project authors. All rights reserved.

#include "global.h"

namespace noble {

using namespace v8;

Handle<Value> Exit(const Arguments& args) {
  HandleScope scope;

  int code = 0;
  if (args.Length() > 0) {
    if (!args[0]->IsInt32())
      ThrowException(Exception::TypeError(String::New("Bad argument")));
    else
      code = args[0]->Int32Value();
  }

  exit(code);
}

void Global::Initialize(Handle<Object> target) {
  HandleScope scope;

  NOBLE_SET_METHOD(target, "exit", Exit);
}

} // namespace noble
