// Copyright 2011 the Noble project authors. All rights reserved.

#include "global.h"

namespace noble {
namespace global {

using namespace v8;

Handle<Value> Exit(const Arguments& args) {
  HandleScope scope;

  int code = 0;
  if (args.Length() > 0) {
    NOBLE_ASSERT_LENGTH(args, 1);
    NOBLE_ASSERT_VALUE(args[0], IsInt32);
    code = args[0]->Int32Value();
  }

  exit(code);

}

void Initialize(Handle<Object> target) {
  HandleScope scope;

  NOBLE_SET_METHOD(target, "exit", global::Exit);
}

} } // namespace noble::global
