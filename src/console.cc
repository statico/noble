// Copyright 2011 the Noble project authors. All rights reserved.

#include <stdio.h>

#include "console.h"

// Needs to be after any v8 headers.
#include "slang.h"

namespace noble {
namespace console {

using namespace v8;
using namespace std;

Handle<Value> Log(const Arguments& args) {
  HandleScope scope;

  int count = args.Length();
  for (int i = 0; i < count; i++) {
    Handle<Value> arg = args[i]->ToString();
    String::Utf8Value value(arg);
    PrintString(string(*value));
    if (i == count - 1) {
      PrintString("\n");
    } else {
      PrintString(" ");
    }
  }

  return Undefined();
}

Handle<Value> MoveCursor(const Arguments& args) {
  HandleScope scope;

  NOBLE_ASSERT_LENGTH(args, 2);
  NOBLE_ASSERT_VALUE(args[0], IsInt32);
  NOBLE_ASSERT_VALUE(args[1], IsInt32);

  int32_t x = args[0]->Int32Value();
  int32_t y = args[1]->Int32Value();
  SLsmg_gotorc(y, x);

  return Undefined();
}

Handle<Value> Clear(const Arguments& args) {
  HandleScope scope;

  NOBLE_ASSERT_LENGTH(args, 0);

  SLsmg_cls();
  SLsmg_gotorc(0, 0);
  SLsmg_refresh();
  return Undefined();
}

Handle<Value> Update(const Arguments& args) {
  HandleScope scope;

  NOBLE_ASSERT_LENGTH(args, 0);

  SLsmg_refresh();

  return Undefined();
}

Handle<Value> SetColor(const Arguments& args) {
  HandleScope scope;

  NOBLE_ASSERT_LENGTH(args, 2);
  NOBLE_ASSERT_VALUE(args[0], IsString);
  NOBLE_ASSERT_VALUE(args[1], IsString);

  String::Utf8Value fg(args[0]);
  String::Utf8Value bg(args[1]);
  SLtt_set_color(0, NULL, *fg, *bg);

  return Undefined();
}

Handle<Value> SetAttribute(const Arguments& args) {
  HandleScope scope;

  // We could do something smarter here such as allowing multiple
  // attribute arguments.
  NOBLE_ASSERT_LENGTH(args, 0);
  NOBLE_ASSERT_VALUE(args[0], IsString);

  String::Utf8Value value(args[0]);
  string attr(*value);

  if (attr == "bold")
    SLtt_add_color_attribute(0, SLTT_BOLD_MASK);
  else if (attr == "blink")
    SLtt_add_color_attribute(0, SLTT_BLINK_MASK);
  else if (attr == "underline")
      SLtt_add_color_attribute(0, SLTT_ULINE_MASK);
  else if (attr == "reverse")
      SLtt_add_color_attribute(0, SLTT_REV_MASK);
  else if (attr == "normal")
      SLtt_add_color_attribute(0, 0);
  else
    NOBLE_THROW(Error, "Invalid attribute");

  return Undefined();
}

void PrintString(const string& message) {
  // const_cast should be okay here, right? Right? Maybe?
  SLsmg_write_string(const_cast<char *>(message.c_str()));
  SLsmg_refresh();
}

void PrintLine(const string& message) {
  PrintString(message + "\n");
  SLsmg_refresh();
}

void PrintException(const string& prefix, const TryCatch& try_catch) {
  HandleScope scope;
  String::Utf8Value trace(try_catch.StackTrace());
  if (trace.length() > 0) {
    PauseAndDisplayMessage(prefix + "\n\n" + string(*trace));
  } else {
    // this really only happens for RangeErrors, since they're the only
    // kind that won't have all this info in the trace.
    Local<Value> er = try_catch.Exception();
    String::Utf8Value msg(!er->IsObject() ? er->ToString()
                         : er->ToObject()->Get(String::New("message"))->ToString());
    PauseAndDisplayMessage(prefix + "\n\nError: " + string(*msg));
  }

}

void PauseAndDisplayMessage(const string& message) {
  SLsmg_cls();
  SLsmg_gotorc(0, 0);
  PrintString(message + "\n\nPress a key...");
  SLsmg_refresh();
  WaitForKeypress();
  SLsmg_cls();
  SLsmg_refresh();
}

int WaitForKeypress() {
  return SLang_getkey();
}

void MainLoop() {
  // TODO: Add periodic timers.
  HandleScope scope;
  Local<Object> global = Context::GetCurrent()->Global();

  SLsmg_refresh(); // Flush anything that might have been written.

  while (int c = WaitForKeypress()) {
    Local<Value> value = global->Get(String::New("onKeypress"));
    if (value->IsFunction()) {
      Handle<Function> callback = Handle<Function>::Cast(value);
      Local<Value> argv[1] = { Integer::New(c) };

      TryCatch try_catch;
      callback->Call(global, 1, argv);
      if (try_catch.HasCaught()) {
        PrintException("Error calling onKeypress()", try_catch);
      }

      SLsmg_refresh();
    } else {
      PauseAndDisplayMessage("onKeypress is not a function");
    }
  }
}

void Finish() {
  SLsmg_reset_smg();
  SLtt_cls();
  SLang_reset_tty();
}

Handle<Value> Initialize() {
  SLtt_get_terminfo();
  SLang_init_tty(-1, 1, 0); // Don't change interrupt key, pass us ^Q/^S.
  SLsmg_init_smg();
  SLsmg_Newline_Behavior = SLSMG_NEWLINE_MOVES;
  SLtt_set_color(0, NULL, (char *) "white", (char *) "default"); // Nicer defaults.

  atexit(Finish);

  Handle<Object> obj = Object::New();
  NOBLE_SET_METHOD(obj, "log", Log);
  NOBLE_SET_METHOD(obj, "moveCursor", MoveCursor);
  NOBLE_SET_METHOD(obj, "clear", Clear);
  NOBLE_SET_METHOD(obj, "update", Update);
  NOBLE_SET_METHOD(obj, "setColor", SetColor);
  NOBLE_SET_METHOD(obj, "setAttribute", SetAttribute);
  return obj;
}

} } // namespace noble::console
