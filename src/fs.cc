// Copyright 2011 the Noble project authors. All rights reserved.

#include "fs.h"

namespace noble {
namespace fs {

using namespace v8;
using namespace std;

Handle<Value> Read(const Arguments& args) {
  HandleScope scope;

  NOBLE_ASSERT_LENGTH(args, 1);

  String::Utf8Value filename(args[0]);
  return ReadFileIntoString(string(*filename));
}

// Reads a file into a v8 string.
// (Very naive. Stolen from v8/samples/process.css)
Handle<String> ReadFileIntoString(const string& name) {
  FILE* file = fopen(name.c_str(), "rb");
  if (file == NULL) return Handle<String>();

  fseek(file, 0, SEEK_END);
  int size = ftell(file);
  rewind(file);

  char* chars = new char[size + 1];
  chars[size] = '\0';
  for (int i = 0; i < size;) {
    int read = fread(&chars[i], 1, size - i, file);
    i += read;
  }
  fclose(file);
  Handle<String> result = String::New(chars, size);
  delete[] chars;
  return result;
}

void Initialize(Handle<Object> target) {
  HandleScope scope;

  NOBLE_SET_METHOD(target, "read", fs::Read);
}

} } // namespace noble::fs
