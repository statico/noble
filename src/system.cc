// Copyright 2011 the Noble project authors. All rights reserved.

#include <unistd.h>

#include <string>

#include "system.h"

namespace noble {
namespace system {

using namespace v8;
using namespace std;

void Context::Init(int argc, char** argv, char** envp) {
  // Populate args.
  Persistent<Array> args = Persistent<Array>::New(Array::New(argc));
  for (int i = 0; i < argc; i++) {
    args->Set(Integer::New(i), String::New(argv[i]));
  }
  args_ = &args;

  // Populate env as an object with properties.
  Persistent<Object> env = Persistent<Object>::New(Object::New());
  for (char** ptr = envp; *ptr != 0; ptr++) {
    string pair(*ptr);
    size_t sep = pair.find('=');
    if (sep != string::npos) {
      string key, value;
      key = pair.substr(0, sep);
      value = pair.substr(sep + 1, string::npos);
      env->Set(String::New(key.c_str()), String::New(value.c_str()));
    }
  }
  env_ = &env;
}

Handle<Value> GetArgs(Local<String> property, const AccessorInfo& info) {
  Handle<Object> self = info.Holder();
  Handle<External> wrap = Handle<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  return static_cast<Context*>(ptr)->GetArgs();
}

Handle<Value> GetEnv(Local<String> property, const AccessorInfo& info) {
  Handle<Object> self = info.Holder();
  Handle<External> wrap = Handle<External>::Cast(self->GetInternalField(0));
  void* ptr = wrap->Value();
  return static_cast<Context*>(ptr)->GetEnv();
}

Handle<Value> Initialize(int argc, char** argv, char** envp) {
  HandleScope scope;

  // Args and env should be read-only -- that's why they're object
  // attributes. Maybe this is dumb. It's not like I know what I'm
  // doing, right? Right?
  Handle<ObjectTemplate> tmpl = ObjectTemplate::New();
  tmpl->SetInternalFieldCount(1);
  tmpl->SetAccessor(String::New("args"), GetArgs);
  tmpl->SetAccessor(String::New("env"), GetEnv);

  Local<Object> obj = tmpl->NewInstance();
  Context* context = new Context();
  context->Init(argc, argv, envp);
  obj->SetInternalField(0, External::New(context));
  return obj;
}

} } // namespace noble::system
