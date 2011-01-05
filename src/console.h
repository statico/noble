// Copyright 2011 the Noble project authors. All rights reserved.

#ifndef NOBLE_CONSOLE_H_
#define NOBLE_CONSOLE_H_

#include <string>

#include "noble.h"

namespace noble {

using namespace std;
using namespace v8;

class Console {
 public:
  static void Clear();
  static void MoveCursor(int x, int y);
  static void PrintString(const string& message);
  static void PrintLine(const string& message);
  static void PrintException(const string& prefix,
                             const TryCatch& try_catch);
  static void PauseAndDisplayMessage(const string& message);
  static int WaitForKeypress();
  static void MainLoop();
  static void Finish();
  static void Initialize(Handle<Object> target);
};

} // namespace noble

#endif // NOBLE_CONSOLE_H_
