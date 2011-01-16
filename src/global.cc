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

#ifdef DEBUG
Handle<Value> DebugLog(const Arguments& args) {
  HandleScope scope;

  OPEN_LOG;
  int len = args.Length();
  for (int i = 0; i < len; i++) {
    X << NOBLE_CSTR(args[i]->ToString());
    X << ((i == len - 1) ? "\n" : " ");
  }
  CLOSE_LOG;

  return Undefined();
}
#endif // DEBUG

void Initialize(Handle<Object> target) {
  HandleScope scope;

  NOBLE_SET_METHOD(target, "exit", global::Exit);

#ifdef DEBUG
  NOBLE_SET_METHOD(target, "LOG", global::DebugLog);
#endif // DEBUG
}

} } // namespace noble::global
