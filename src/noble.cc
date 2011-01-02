// Copyright 2011 the Noble project authors. All rights reserved.

#include <string>

#include <curses.h>
#include <stdio.h>
#include <v8.h>

#include "noble.h"
#include "console.h"
#include "file.h"
#include "global.h"

using namespace std;
using namespace v8;
using namespace noble;

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: %s <noble.js>\n", argv[0]);
    return 1;
  }
  string filename(argv[1]);

  HandleScope scope;
  TryCatch try_catch;

  // Create a template for the global object and set the
  // built-in global functions.
  Local<ObjectTemplate> global = ObjectTemplate::New();
  Persistent<Context> context = Context::New(NULL, global);
  Context::Scope context_scope(context);

  // Create and initialize globals.
  Global::Initialize(context->Global());

  Local<Object> file = Object::New();
  File::Initialize(file);
  global->Set(String::NewSymbol("file"), file);

  // Assume that we're using a console UI for now.
  Local<Object> console = Object::New();
  Console::Initialize(console);
  global->Set(String::NewSymbol("console"), console);

  // Load noble.js
  Handle<String> source = File::ReadFileIntoString(filename);
  if (source.IsEmpty()) {
    Console::PrintLine("Error reading init script");
    Console::WaitForKeypress();
    return 1;
  }

  // Compile
  Handle<Script> script = Script::Compile(source);
  if (script.IsEmpty()) {
    Console::PrintException(try_catch);
    Console::WaitForKeypress();
    return 1;
  }

  // Run
  script->Run();

  // Loop
  Console::MainLoop();

  return 0;
}
