// Copyright 2011 the Noble project authors. All rights reserved.

#ifndef SRC_SYSTEM_H_
#define SRC_SYSTEM_H_

#include "noble.h"

namespace noble {
namespace system {

using namespace v8;
using namespace std;

class Context {
 public:
  Context() { }

  virtual void Init(int argc, char** argv, char** envp);

  virtual Handle<Array> GetArgs() { return *args_; }
  virtual Handle<Object> GetEnv() { return *env_; }

 private:
  Persistent<Array>* args_;
  Persistent<Object>* env_;

  DISALLOW_COPY_AND_ASSIGN(Context);
};

Handle<Value> Initialize(int argc, char** argv, char** envp);

} } // namespace noble::system

#endif // SRC_SYSTEM_H_
