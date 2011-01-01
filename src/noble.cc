// Copyright 2010 the Noble project authors. All rights reserved.

#include <curses.h>
#include <stdio.h>
#include <v8.h>

#include <string>

using namespace std;
using namespace v8;

// Reads a file into a v8 string.
// (from v8/samples/process.css)
Handle<String> ReadFile(const string& name) {
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

Handle<Value> PutString(const Arguments& args) {
  if (args.Length() < 1) return Undefined();
  HandleScope scope;
  Handle<Value> arg = args[0];
  String::Utf8Value value(arg);
  string str(*value);
  addstr(str.c_str());
  return Undefined();
}

Handle<Value> Exit(const Arguments& args) {
  int code = 0;
  if (args.Length() > 0) {
    if (!args[0]->IsInt32())
      ThrowException(Exception::TypeError(String::New("Bad argument")));
    else
      code = args[0]->Int32Value();
  }

  endwin();
  exit(code);
}

void PrintException(const TryCatch& try_catch) {
  HandleScope scope;

  String::Utf8Value trace(try_catch.StackTrace());
  if (trace.length() > 0) {
    fprintf(stderr, "Traceback:\n%s\n", *trace);
  } else {
    // this really only happens for RangeErrors, since they're the only
    // kind that won't have all this info in the trace.
    Local<Value> er = try_catch.Exception();
    String::Utf8Value msg(!er->IsObject() ? er->ToString()
                         : er->ToObject()->Get(String::New("message"))->ToString());
    fprintf(stderr, "Error: %s\n", *msg);
  }

  fflush(stderr);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <noble.js>\n", argv[0]);
    return 1;
  }
  string filename(argv[1]);

  initscr();
  noecho();

  HandleScope scope;
  TryCatch try_catch;

  // Create a template for the global object and set the
  // built-in global functions.
  Local<ObjectTemplate> global = ObjectTemplate::New();
  global->Set(String::New("puts"), FunctionTemplate::New(PutString));
  global->Set(String::New("exit"), FunctionTemplate::New(Exit));
  Persistent<Context> context = Context::New(NULL, global);

  Context::Scope context_scope(context);

  // Load noble.js
  Handle<String> source = ReadFile(filename);
  if (source.IsEmpty()) {
    fprintf(stderr, "Error reading init script\n");
    return 1;
  }

  // Compile
  Handle<Script> script = Script::Compile(source);
  if (script.IsEmpty()) {
    PrintException(try_catch);
    return 1;
  }

  // Run
  addstr("Running ");
  addstr(filename.c_str());
  addstr("...\n");
  Handle<Value> result = script->Run();
  if (result.IsEmpty()) {
    PrintException(try_catch);
    return 1;
  }

  // Listen for keypresses
  while (int c = getch()) {
    Local<Value> value = context->Global()->Get(String::New("handleKeypress"));
    if (value->IsFunction()) {
      Handle<Function> callback = Handle<Function>::Cast(value);
      Local<Value> argv[1] = { Integer::New(c) };
      callback->Call(context->Global(), 1, argv);
    } else {
      addstr("handleKeypress is not a function\n");
    }
  }

  endwin();
  return 0;
}
