// Copyright 2011 the Noble project authors. All rights reserved.

#include <stdio.h>

#include "console.h"

// Needs to be after any v8 headers.
#include "slang.h"

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

void Console::MoveCursor(int x, int y) {
  SLsmg_gotorc(y, x);
}

void Console::Clear() {
  SLsmg_cls();
  SLsmg_gotorc(0, 0);
  SLsmg_refresh();
}

void Console::PrintString(const string& message) {
  // const_cast should be okay here, right? Right? Maybe?
  SLsmg_write_string(const_cast<char *>(message.c_str()));
  SLsmg_refresh();
}

void Console::PrintLine(const string& message) {
  Console::PrintString(message + "\n");
  SLsmg_refresh();
}

void Console::PrintException(const string& prefix,
                             const TryCatch& try_catch) {
  HandleScope scope;
  String::Utf8Value trace(try_catch.StackTrace());
  if (trace.length() > 0) {
    Console::PauseAndDisplayMessage(prefix + "\n\n" + string(*trace));
  } else {
    // this really only happens for RangeErrors, since they're the only
    // kind that won't have all this info in the trace.
    Local<Value> er = try_catch.Exception();
    String::Utf8Value msg(!er->IsObject() ? er->ToString()
                         : er->ToObject()->Get(String::New("message"))->ToString());
    Console::PauseAndDisplayMessage(prefix + "\n\nError: " + string(*msg));
  }

}

void Console::PauseAndDisplayMessage(const string& message) {
  Console::Clear();
  Console::PrintString(message + "\n\nPress a key...");
  Console::WaitForKeypress();
  Console::Clear();
}

int Console::WaitForKeypress() {
  return SLang_getkey();
}

void Console::MainLoop() {
  // TODO: Add periodic timers.
  HandleScope scope;
  Local<Object> global = Context::GetCurrent()->Global();

  SLsmg_refresh(); // Flush anything that might have been written.

  while (int c = Console::WaitForKeypress()) {
    Local<Value> value = global->Get(String::New("onKeypress"));
    if (value->IsFunction()) {
      Handle<Function> callback = Handle<Function>::Cast(value);
      Local<Value> argv[1] = { Integer::New(c) };

      TryCatch try_catch;
      callback->Call(global, 1, argv);
      if (try_catch.HasCaught()) {
        Console::PrintException("Error calling onKeypress()", try_catch);
      }

      SLsmg_refresh();
    } else {
      Console::PauseAndDisplayMessage("onKeypress is not a function");
    }
  }
}

void Console::Finish() {
  SLsmg_reset_smg();
  SLtt_cls();
  SLang_reset_tty();
}

void Console::Initialize(Handle<Object> target) {
  HandleScope scope;

  NOBLE_SET_METHOD(target, "log", Log);

  SLtt_get_terminfo();
  SLang_init_tty(-1, 0, 0);
  SLsmg_init_smg();
  SLsmg_Newline_Behavior = SLSMG_NEWLINE_MOVES;
  SLtt_set_color(0, NULL, (char *) "white", (char *) "default"); // Nicer defaults.

  atexit(Console::Finish);
}

} // namespace noble
