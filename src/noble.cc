// Copyright 2011 the Noble project authors. All rights reserved.

#include <string>

#include <curses.h>
#include <stdio.h>
#include <v8.h>

#include "noble.h"
#include "console.h"
#include "fs.h"
#include "global.h"
#include "system.h"

using namespace std;
using namespace v8;
using namespace noble;

void InitGlobal(int argc, char** argv, char** envp) {
  HandleScope scope;
  Local<Object> global = Context::GetCurrent()->Global();

  // Create and initialize globals.
  global::Initialize(global);

  Local<Object> fs = Object::New();
  fs::Initialize(fs);
  global->Set(String::NewSymbol("fs"), fs);

  // Assume that we're using a console UI for now.
  Local<Object> console = Object::New();
  console::Initialize(console);
  global->Set(String::NewSymbol("console"), console);

  // noble:: prefix is required here.
  Handle<Value> system = noble::system::Initialize(argc, argv, envp);
  global->Set(String::NewSymbol("system"), system);
}

int main(int argc, char** argv, char** envp) {
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

  // Populate the global context.
  InitGlobal(argc, argv, envp);

  // Load noble.js
  Handle<String> source = fs::ReadFileIntoString(filename);
  if (source.IsEmpty()) {
    console::PauseAndDisplayMessage("Couldn't read file " + filename);
    return 1;
  }

  // Compile
  Handle<Script> script = Script::Compile(source, String::NewSymbol(filename.c_str()));
  if (script.IsEmpty()) {
    console::PrintException("Couldn't compile init script", try_catch);
    return 1;
  }

  // Run
  try_catch.Reset();
  script->Run();
  if (try_catch.HasCaught()) {
    console::PrintException("Couldn't run init script", try_catch);
  }

  // Loop
  console::MainLoop();

  return 0;
}
