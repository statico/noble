// Copyright 2011 the Noble project authors. All rights reserved.

#ifndef NOBLE_H_
#define NOBLE_H_

#include <v8.h>

// A convenience macro stolen from Node.
#define NOBLE_SET_METHOD(obj, name, callback) \
  obj->Set(String::NewSymbol(name),          \
           FunctionTemplate::New(callback)->GetFunction())

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class. (From
// the Google C++ Style Guide.)
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#endif // NOBLE_H_
