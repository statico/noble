// Copyright 2011 the Noble project authors. All rights reserved.

#include <stdio.h>

#include <map>
#include <iostream>
#include <sstream>

#include "term.h"

// Needs to be after any v8 headers.
#include "slang.h"

namespace noble {
namespace term {

using namespace v8;
using namespace std;

// Apparently terminals support 'color pairs', and ncurses and SLang
// want you to create a color pair before printing text in a given
// color combination. Like mutt, I want to support arbitrary fg/bg
// colors, so the way to do this is allocate new pairs and save them
// as we go.
typedef pair<string, string> color_tuple;
static map<color_tuple, int> color_map;

Handle<Value> PutString(const Arguments& args) {
  HandleScope scope;

  NOBLE_ASSERT_LENGTH(args, 1);

  Handle<Value> arg = args[0]->ToString();
  String::Utf8Value value(arg);
  PrintString(string(*value));

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

  // Normally, this is where we should check color_map.size() being
  // greater than COLOR_PAIRS, but including slcurses.h (part of
  // SLang) does all sorts of weird stuff. Instead, hope and pray.

  String::Utf8Value first(args[0]);
  String::Utf8Value second(args[1]);

  string fg(*first);
  string bg(*second);
  color_tuple tuple(fg, bg);

  int cid;
  map<color_tuple, int>::const_iterator it = color_map.find(tuple);
  if (it != color_map.end()) {
    cid = it->second;
  } else {
    cid = color_map.size() + 2; // Colors 0 and 1 are reserved.
    color_map.insert(pair<color_tuple, int>(tuple, cid));
  }

  SLtt_set_color(cid, NULL, *first, *second);
  SLsmg_set_color(cid);

  return Undefined();
}

Handle<Value> SetAttribute(const Arguments& args) {
  HandleScope scope;

  // We could do something smarter here such as allowing multiple
  // attribute arguments.
  NOBLE_ASSERT_LENGTH(args, 1);
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

Handle<Value> GetWidth(const Arguments& args) {
  HandleScope scope;
  NOBLE_ASSERT_LENGTH(args, 0);
  return Integer::New(SLtt_Screen_Cols);
}

Handle<Value> GetHeight(const Arguments& args) {
  HandleScope scope;
  NOBLE_ASSERT_LENGTH(args, 0);
  return Integer::New(SLtt_Screen_Rows);
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
  SLsmg_refresh();

  atexit(Finish);

  Handle<Object> obj = Object::New();
  // I've decided to go with a width-height/x-y metaphor instead of
  // rows-cols/y-x. We're working with pixels, not a spreadsheet.
  //
  // TODO: widthOfUtf8Char
  NOBLE_SET_METHOD(obj, "puts", PutString);
  NOBLE_SET_METHOD(obj, "moveCursor", MoveCursor);
  NOBLE_SET_METHOD(obj, "clear", Clear);
  NOBLE_SET_METHOD(obj, "update", Update);
  NOBLE_SET_METHOD(obj, "setColor", SetColor);
  NOBLE_SET_METHOD(obj, "setAttribute", SetAttribute);
  NOBLE_SET_METHOD(obj, "width", GetWidth);
  NOBLE_SET_METHOD(obj, "height", GetHeight);
  return obj;
}

} } // namespace noble::term
