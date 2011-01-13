// Copyright 2011 the Noble project authors. All rights reserved.

#include <unistd.h>

#include <string>

#include "system.h"

namespace noble {
namespace system {

using namespace v8;
using namespace std;

static Persistent<Array> args;
static Persistent<Object> env;

Handle<Array> MakeArgs(int argc, char** argv) {
  Handle<Array> args = Array::New(argc);

  for (int i = 0; i < argc; i++) {
    args->Set(Integer::New(i), String::New(argv[i]));
  }

  return args;
}

Handle<Object> MakeEnv(char** envp) {
  Handle<Object> env = Object::New();

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

  return env;
}

Handle<Value> Initialize(int argc, char** argv, char** envp) {
  Handle<Object> obj = Object::New();
  obj->Set(String::New("args"), MakeArgs(argc, argv));
  obj->Set(String::New("env"), MakeEnv(envp));
  return obj;
}

} } // namespace noble::system
