// Copyright 2011 the Noble project authors. All rights reserved.

#ifndef NOBLE_H_
#define NOBLE_H_

#ifdef DEBUG
#include <ctime>
#include <iostream>
#include <fstream>
#endif // DEBUG

#include <v8.h>

// A convenience macro stolen from Node.
#define NOBLE_SET_METHOD(obj, name, callback) \
  obj->Set(String::NewSymbol(name), \
           FunctionTemplate::New(callback)->GetFunction())

// Reusable snippets.
#define NOBLE_THROW(type, message) \
  return ThrowException(Exception::type(String::New(message)));

#define NOBLE_ASSERT_LENGTH(value, length) \
  if (value.Length() != length) \
    NOBLE_THROW(Error, "Incorrect number of arguments");

#define NOBLE_ASSERT_VALUE(value, method) \
  if (!value->method()) \
    NOBLE_THROW(TypeError, "Bad argument");

#define NOBLE_CSTR(value) (*(String::Utf8Value(value)))

// Useful for debugging.
#define NOBLE_TOSTRING(handle) \
  (handle.IsEmpty() ? (char *) "(empty)" : NOBLE_CSTR(handle->ToString()))

// Other handy macros stolen from Node.
#define MIN(a, b) ((a) < (b) ? (a) : (b))

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class. (From
// the Google C++ Style Guide.)
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#ifdef DEBUG
static std::ofstream logger__("noble.log", std::fstream::app);
#define DLOG logger__
#define INIT_DLOG() \
  time_t t = time(0); \
  struct tm* now = localtime(&t); \
  DLOG << "--------------------------------------------------------" << std::endl; \
  DLOG << "Starting log at " << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) \
       << '-' << now->tm_mday << ' ' << now->tm_hour << ':' << now->tm_min \
       << ':' << now->tm_sec << std::endl;
#endif // DEBUG

#endif // NOBLE_H_
