// Copyright 2011 the Noble project authors. All rights reserved.

#include <curses.h>
#include <stdio.h>

#include "console.h"

namespace noble {

using namespace v8;
using namespace std;

Handle<Value> Log(const Arguments& args) {
  HandleScope scope;

  int count = args.Length();
  if (count < 1)
    return Undefined();

  for (int i = 0; i < count; i++) {
    Handle<Value> arg = args[0];
    String::Utf8Value value(arg);
    Console::PrintString(string(*value));
    if (i == count - 1)
      Console::PrintString("\n");
  }

  return Undefined();
}

void Console::PrintString(const string& message) {
  addstr(message.c_str());
}

void Console::PrintLine(const string& message) {
  Console::PrintString(message + "\n");
}

void Console::PrintException(const TryCatch& try_catch) {
  HandleScope scope;

  String::Utf8Value trace(try_catch.StackTrace());
  if (trace.length() > 0) {
    Console::PrintLine("Traceback:");
    Console::PrintLine(*trace);
  } else {
    // this really only happens for RangeErrors, since they're the only
    // kind that won't have all this info in the trace.
    Local<Value> er = try_catch.Exception();
    String::Utf8Value msg(!er->IsObject() ? er->ToString()
                         : er->ToObject()->Get(String::New("message"))->ToString());
    Console::PrintLine("Error: " + string(*msg));
  }
}

int Console::WaitForKeypress() {
  return getch();
}

void Console::MainLoop() {
  // TODO: Add periodic timers.
  HandleScope scope;
  Local<Object> global = Context::GetCurrent()->Global();

  while (int c = getch()) {
    Local<Value> value = global->Get(String::New("onKeypress"));
    if (value->IsFunction()) {
      Handle<Function> callback = Handle<Function>::Cast(value);
      Local<Value> argv[1] = { Integer::New(c) };
      callback->Call(global, 1, argv);
      doupdate();
    } else {
      Console::PrintLine("onKeypress is not a function");
    }
  }
}

void Console::Finish() {
  endwin();
}

void Console::Initialize(Handle<Object> target) {
  HandleScope scope;

  NOBLE_SET_METHOD(target, "log", Log);

  initscr();
  noecho();
  atexit(Console::Finish);
}

} // namespace noble
